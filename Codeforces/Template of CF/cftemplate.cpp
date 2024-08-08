#include <bits/stdc++.h>
#define int long long
using namespace std;

const int mod = 1e9+7;
const int N = 1e6+5; // Adjust as per problem constraints

int fact[N];
bool isPrimeArr[N];

// Iterative Modular Exponentiation
int modExp(int base, int exp, int m) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % m;
        }
        base = (base * base) % m;
        exp /= 2;
    }
    return result;
}

// Iterative Modular Inverse using Fermat's Little Theorem
int modInverse(int a, int m) {
    return modExp(a, m-2, m);
}

// Iterative Factorial Calculation and Precomputation
void computeFactorials(int n) {
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
        fact[i] = (fact[i-1] * i) % mod;
    }
}

// Iterative Sieve of Eratosthenes for Prime Numbers
void sieve(int n) {
    fill(isPrimeArr, isPrimeArr + n + 1, true);
    isPrimeArr[0] = isPrimeArr[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (isPrimeArr[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrimeArr[j] = false;
            }
        }
    }
}

// Iterative Function to Check if a Number is Prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Iterative GCD Function
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Iterative LCM Function
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// Function to calculate (n choose r) % mod
int nCr(int n, int r) {
    if (r > n) return 0;
    return (fact[n] * modInverse(fact[r], mod) % mod) * modInverse(fact[n-r], mod) % mod;
}

// Function to return Euler's Totient Function value for n
int phi(int n) {
    int result = n;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// Function to calculate the sum of divisors of n
int sumOfDivisors(int n) {
    int sum = 0;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i)
                sum += n / i;
        }
    }
    return sum;
}

// Solve function
int solve(vector<int> &nums){
    int n = nums.size(), ans = 0;
    // Solve the problem here

    return ans;
}

// Main function to handle multiple test cases
signed main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);

    int t = 1;
    cin >> t; // Take number of test cases

    while (t--) {
        // Case 1 : Input and Output
        int n;
        cin >> n;
        vector<int> nums(n);
        for(int i=0; i<n; i++) cin>>nums[i];

        int ans = solve(nums);
        cout << ans << endl;

        

        // Case 2 : Input and Output
        // int n;
        // cin>>n;

        // Solve the problem here
    }

    return 0;
}
