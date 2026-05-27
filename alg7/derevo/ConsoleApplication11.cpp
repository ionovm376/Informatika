#include <iostream>
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

int dfs(TreeNode* root, int& res) {
    if (!root) return 0;
    int l = max(0, dfs(root->left, res));
    int r = max(0, dfs(root->right, res));
    res = max(res, l + r + root->val);
    return root->val + max(l, r);
}

int maxPathSum(TreeNode* root) {
    int res = root->val;
    dfs(root, res);
    return res;
}

int main() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    cout << maxPathSum(root) << endl;
    return 0;
}