void sib(Node* node , vector<int>&ans)
{
    if(node==nullptr) return;
    if((node->left==nullptr) ^ (node->right == nullptr))
    {
        if(node->left)ans.push_back(node->left->data);
        else ans.push_back(node->right->data);
    }
    sib(node->left,ans);
    sib(node->right,ans);
}
vector<int> noSibling(Node* node)
{
    // code here
    vector<int> ans;
    sib(node,ans);
    if(ans.size()==0) return {-1};
    sort(ans.begin(),ans.end());
    return ans;
}
