#include <iostream>
#include <vector>
using namespace std;

bool check(int v, int c, vector<int>& color, vector<vector<int>>& g) {
    for (int i = 0; i < g.size(); i++) {
        if (g[v][i] && color[i] == c) return false;
    }
    return true;
}

bool backtrack(vector<int>& color, int v, int n, int k, vector<vector<int>>& g) {
    if (v == n) return true;
    for (int c = 1; c <= k; c++) {
        if (check(v, c, color, g)) {
            color[v] = c;
            if (backtrack(color, v + 1, n, k, g)) return true;
            color[v] = 0;
        }
    }
    return false;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < n; j++) {
            g[i][j] = s[j] - '0';
        }
    }
    vector<int> color(n, 0);
    if (backtrack(color, 0, n, k, g)) {
        cout << "YES" << endl;
        for (int i = 0; i < n; i++) cout << color[i] << " ";
        cout << endl;
    }
    else {
        cout << "NO" << endl;
    }
    return 0;
}