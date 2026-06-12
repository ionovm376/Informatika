#include <iostream>
#include <vector>
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
    int n;
    cin >> n;
    vector<int> val(n), l(n), r(n);
    for (int i = 0; i < n; i++) {
        cin >> val[i] >> l[i] >> r[i];
    }
    vector<TreeNode*> nodes(n);
    for (int i = 0; i < n; i++) {
        nodes[i] = new TreeNode(val[i]);
    }
    for (int i = 0; i < n; i++) {
        if (l[i] != -1) nodes[i]->left = nodes[l[i]];
        if (r[i] != -1) nodes[i]->right = nodes[r[i]];
    }
    TreeNode* root = nodes[0];
    cout << maxPathSum(root) << endl;
    return 0;
}
