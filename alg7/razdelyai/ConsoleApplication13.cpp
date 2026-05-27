#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

pair<int, int> minMax(vector<int>& a, int l, int r) {
    if (l == r) return { a[l], a[l] };
    int m = (l + r) / 2;
    pair<int, int> left = minMax(a, l, m);
    pair<int, int> right = minMax(a, m + 1, r);
    return { min(left.first, right.first), max(left.second, right.second) };
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    pair<int, int> ans = minMax(a, 0, n - 1);
    cout << "The minimum array element is " << ans.first << endl;
    cout << "The maximum array element is " << ans.second << endl;
    return 0;
}