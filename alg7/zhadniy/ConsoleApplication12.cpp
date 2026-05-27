#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int x, k;
    cin >> x >> k;
    vector<int> a(k);
    for (int i = 0; i < k; i++) cin >> a[i];
    vector<int> dp(x + 1, 1e9);
    dp[0] = 0;
    for (int i = 0; i <= x; i++) {
        for (int j = 0; j < k; j++) {
            if (i + a[j] <= x) {
                dp[i + a[j]] = min(dp[i + a[j]], dp[i] + 1);
            }
        }
    }
    if (dp[x] == 1e9) cout << -1 << endl;
    else cout << dp[x] << endl;
    return 0;
}