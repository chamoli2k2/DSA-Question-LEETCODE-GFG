#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>
#include <cmath>
using namespace std;

// ----------------------------- USER -----------------------------
class User {
public:
    string id;
    string name;
    User(string _id, string _name) : id(_id), name(_name) {}
};

// ------------------------ SPLIT STRATEGY ------------------------
class SplitStrategy {
public:
    virtual unordered_map<string, double> split(double total, const vector<shared_ptr<User>>& users) = 0;
    virtual ~SplitStrategy() = default;
};

class EqualSplit : public SplitStrategy {
public:
    unordered_map<string, double> split(double total, const vector<shared_ptr<User>>& users) override {
        unordered_map<string, double> result;
        double each = round((total / users.size()) * 100.0) / 100.0;
        for (auto& u : users) {
            result[u->id] = each;
        }
        return result;
    }
};

// ------------------------ EXPENSE ------------------------
class Expense {
public:
    string paidBy;
    double amount;
    unordered_map<string, double> splitDetails;

    Expense(string _paidBy, double _amount, const unordered_map<string, double>& _split)
        : paidBy(_paidBy), amount(_amount), splitDetails(_split) {}
};

// ------------------------ GROUP ------------------------
class Group {
public:
    string groupId;
    string groupName;
    vector<string> memberIds;

    Group(string id, string name) : groupId(id), groupName(name) {}

    void addMember(string uid) {
        memberIds.push_back(uid);
    }

    vector<string> getMembers() const {
        return memberIds;
    }
};

// ------------------------ EXPENSE MANAGER ------------------------
class ExpenseManager {
    unordered_map<string, shared_ptr<User>> users;
    unordered_map<string, shared_ptr<Group>> groups;
    vector<Expense> allExpenses;

    unordered_map<string, unordered_map<string, double>> balanceSheet;

public:
    void addUser(const shared_ptr<User>& user) {
        users[user->id] = user;
    }

    void createGroup(string groupId, string groupName, const vector<string>& memberIds) {
        auto grp = make_shared<Group>(groupId, groupName);
        for (const auto& m : memberIds)
            grp->addMember(m);
        groups[groupId] = grp;
    }

    void addGroupExpense(string groupId, string paidBy, double amount, shared_ptr<SplitStrategy> strat) {
        if (groups.find(groupId) == groups.end()) {
            cout << "❌ Group not found.\n";
            return;
        }

        auto grp = groups[groupId];
        vector<shared_ptr<User>> usersInGroup;
        for (auto& id : grp->getMembers()) {
            usersInGroup.push_back(users[id]);
        }

        auto splitMap = strat->split(amount, usersInGroup);
        Expense exp(paidBy, amount, splitMap);
        allExpenses.push_back(exp);

        for (auto& [userId, share] : splitMap) {
            if (userId == paidBy) continue;

            balanceSheet[userId][paidBy] += share;
            balanceSheet[paidBy][userId] -= share;
        }
    }

    void showBalances() {
        cout << "\n📊 User Balances:\n";
        for (auto& [u1, mp] : balanceSheet) {
            for (auto& [u2, amt] : mp) {
                if (amt > 0.01) {
                    cout << users[u1]->name << " owes " << users[u2]->name << " ₹" << round(amt * 100.0) / 100.0 << "\n";
                }
            }
        }
    }

    void simplifyDebts() {
        unordered_map<string, double> net;
        for (auto& [u1, mp] : balanceSheet) {
            for (auto& [u2, amt] : mp) {
                net[u1] -= amt;
                net[u2] += amt;
            }
        }

        auto cmp = [](pair<string, double> a, pair<string, double> b) { return a.second > b.second; };
        priority_queue<pair<string, double>, vector<pair<string, double>>, decltype(cmp)> creditors(cmp);
        priority_queue<pair<string, double>> debtors;

        for (auto& [uid, amt] : net) {
            if (amt > 0.01) creditors.push({uid, amt});
            else if (amt < -0.01) debtors.push({uid, -amt});
        }

        cout << "\n✅ Simplified Transactions:\n";
        while (!creditors.empty() && !debtors.empty()) {
            auto [cred, crAmt] = creditors.top(); creditors.pop();
            auto [debt, dbAmt] = debtors.top(); debtors.pop();

            double settled = min(crAmt, dbAmt);
            cout << users[debt]->name << " pays " << users[cred]->name << " ₹" << round(settled * 100.0) / 100.0 << "\n";

            if (crAmt - settled > 0.01)
                creditors.push({cred, crAmt - settled});
            if (dbAmt - settled > 0.01)
                debtors.push({debt, dbAmt - settled});
        }
    }
};

int main() {
    auto manager = make_unique<ExpenseManager>();

    auto u1 = make_shared<User>("u1", "Gaurav");
    auto u2 = make_shared<User>("u2", "Suman");
    auto u3 = make_shared<User>("u3", "Ajay");
    auto u4 = make_shared<User>("u4", "Ravi");

    manager->addUser(u1);
    manager->addUser(u2);
    manager->addUser(u3);
    manager->addUser(u4);

    manager->createGroup("g1", "TripToGoa", {"u1", "u2", "u3", "u4"});

    auto equal = make_shared<EqualSplit>();

    // Gaurav paid ₹4000 for group trip
    manager->addGroupExpense("g1", "u1", 4000, equal);

    // Suman paid ₹2000
    manager->addGroupExpense("g1", "u2", 2000, equal);

    // Ajay paid ₹1000
    manager->addGroupExpense("g1", "u3", 1000, equal);

    manager->showBalances();

    manager->simplifyDebts();

    return 0;
}
