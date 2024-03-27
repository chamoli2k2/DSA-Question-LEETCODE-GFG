class Solution {
public:
    vector<vector<int>> findMatrix(vector<int>& nums) {
        vector<vector<int>> ans;
        unordered_map<int,int> mp;
        int maxi = 1;
        
        for(int i=0;i<nums.size();i++){
            mp[nums[i]]++;
            maxi = max(maxi,mp[nums[i]]);
        }
        
        for(int i=0;i<maxi;i++){
            vector<int> temp;
            for(auto& it : mp){
                if(it.second > 0){
                    temp.push_back(it.first);
                    it.second--;
                }
            }
            ans.push_back(temp);
        }
        
        return ans;
    }
};
