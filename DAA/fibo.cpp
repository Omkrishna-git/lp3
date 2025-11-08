#include <bits/stdc++.h>
using namespace std;

// ---------------- Recursive ----------------
int fib_rec(int n) {
    if (n <= 1) return n;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

// ---------------- Memoization ----------------
int fib_memo_helper(int n, vector<int> &dp) {
    if (n <= 1) return n;
    if (dp[n] != -1) return dp[n];
    return dp[n] = fib_memo_helper(n - 1, dp) + fib_memo_helper(n - 2, dp);
}

int fib_memo(int n) {
    vector<int> dp(n + 1, -1);
    return fib_memo_helper(n, dp);
}

// ---------------- Tabulation (Bottom-Up) ----------------
int fib_tab(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1, 0);
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// ---------------- Space Optimized ----------------
int fib_space_opt(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// ---------------- Menu Driven ----------------
int main() {
    int choice, n;
    while (true) {
        cout << "\n===== Fibonacci Computation Menu =====\n";
        cout << "1. Recursive Method\n";
        cout << "2. Memoization (Top-Down DP)\n";
        cout << "3. Tabulation (Bottom-Up DP)\n";
        cout << "4. Space Optimized DP\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5) {
            cout << "Exiting program...\n";
            break;
        }

        cout << "Enter value of n: ";
        cin >> n;

        int result = 0;
        switch (choice) {
            case 1:
                result = fib_rec(n);
                cout << "Fibonacci(" << n << ") = " << result << " (Recursive)\n";
                break;
            case 2:
                result = fib_memo(n);
                cout << "Fibonacci(" << n << ") = " << result << " (Memoization)\n";
                break;
            case 3:
                result = fib_tab(n);
                cout << "Fibonacci(" << n << ") = " << result << " (Tabulation)\n";
                break;
            case 4:
                result = fib_space_opt(n);
                cout << "Fibonacci(" << n << ") = " << result << " (Space Optimized)\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
