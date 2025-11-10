#include <bits/stdc++.h>
using namespace std;


//  Recursive (Exponential)
int knapsackRec(int idx, int capacity, vector<int>& wt, vector<int>& val) {
    if (idx == 0) {
        return (wt[0] <= capacity) ? val[0] : 0;
    }

    int skip = knapsackRec(idx - 1, capacity, wt, val);
    int take = INT_MIN;

    if (wt[idx] <= capacity)
        take = val[idx] + knapsackRec(idx - 1, capacity - wt[idx], wt, val);

    return max(take, skip);
}


//  Memoization (Top-Down DP)
int knapsackMemo(int idx, int capacity, vector<int>& wt, vector<int>& val,vector<vector<int>>& dp) {
    if (idx == 0) return (wt[0] <= capacity) ? val[0] : 0;
    if (dp[idx][capacity] != -1) return dp[idx][capacity];

    int skip = knapsackMemo(idx - 1, capacity, wt, val, dp);
    int take = INT_MIN;

    if (wt[idx] <= capacity)
        take = val[idx] + knapsackMemo(idx - 1, capacity - wt[idx], wt, val, dp);

    return dp[idx][capacity] = max(take, skip);
}

int knapSackMemoization(vector<int>& wt, vector<int>& val, int capacity) {
    int n = wt.size();
    vector<vector<int>> dp(n, vector<int>(capacity + 1, -1));
    return knapsackMemo(n - 1, capacity, wt, val, dp);
}


//  Tabulation (Bottom-Up)
int knapSackTabulation(vector<int>& wt, vector<int>& val, int capacity) {
    int n = wt.size();
    vector<vector<int>> dp(n, vector<int>(capacity + 1, 0));

    for (int w = wt[0]; w <= capacity; w++)
        dp[0][w] = val[0];

    for (int i = 1; i < n; i++) {
        for (int w = 0; w <= capacity; w++) {
            int skip = dp[i - 1][w];
            int take = INT_MIN;
            if (wt[i] <= w) take = val[i] + dp[i - 1][w - wt[i]];
            dp[i][w] = max(take, skip);
        }
    }
    return dp[n - 1][capacity];
}


//  Space Optimized (1D DP)
int knapSackSpaceOptimized(vector<int>& wt, vector<int>& val, int capacity) {
    int n = wt.size();
    vector<int> prev(capacity + 1, 0);

    for (int w = wt[0]; w <= capacity; w++)
        prev[w] = val[0];

    for (int i = 1; i < n; i++) {
        for (int w = capacity; w >= 0; w--) {
            int skip = prev[w];
            int take = INT_MIN;
            if (wt[i] <= w)
                take = val[i] + prev[w - wt[i]];
            prev[w] = max(take, skip);
        }
    }
    return prev[capacity];
}


//  Branch and Bound Method
struct Item {
    int value, weight;
    double ratio;
};

struct Node {
    int level, profit, weight;
    double bound;
};

bool cmp(Item a, Item b) { 
    return a.ratio > b.ratio; 
}

double bound(Node u, int n, int capacity, vector<Item>& arr) {
    if (u.weight >= capacity)
        return 0;

    double profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    while (j < n && totweight + arr[j].weight <= capacity) {
        totweight += arr[j].weight;
        profit_bound += arr[j].value;
        j++;
    }

    if (j < n)
        profit_bound += (capacity - totweight) * arr[j].ratio;

    return profit_bound;
}

int knapSackBnB(int capacity, vector<int>& val, vector<int>& wt, int n) {
    vector<Item> arr(n);

    for (int i = 0; i < n; i++)
        arr[i] = {val[i], wt[i], (double)val[i] / wt[i]};

    sort(arr.begin(), arr.end(), cmp);

    queue<Node> Q;
    Node u, v;
    u.level = -1, u.profit = 0, u.weight = 0;
    Q.push(u);
    int maxProfit = 0;

    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        if (u.level == n - 1)
            continue;

        v.level = u.level + 1;

        // Take item
        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;

        if (v.weight <= capacity && v.profit > maxProfit)
            maxProfit = v.profit;

        v.bound = bound(v, n, capacity, arr);
        if (v.bound > maxProfit)
            Q.push(v);

        // Skip item
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, capacity, arr);
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;
}

int main() {
    int n, capacity;
    cout << "Enter number of items: ";
    cin >> n;

    vector<int> val(n), wt(n);

    cout << "Enter values of items:\n";
    for (int i = 0; i < n; i++) cin >> val[i];

    cout << "Enter weights of items:\n";
    for (int i = 0; i < n; i++) cin >> wt[i];

    cout << "Enter knapsack capacity: ";
    cin >> capacity;

    cout << "\nSelect Method:\n";
    cout << "1. Recursive\n";
    cout << "2. Memoization\n";
    cout << "3. Tabulation\n";
    cout << "4. Space Optimized\n";
    cout << "5. Branch and Bound\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    int result = 0;

    switch (choice) {
    case 1:
        result = knapsackRec(n - 1, capacity, wt, val);
        break;
    case 2:
        result = knapSackMemoization(wt, val, capacity);
        break;
    case 3:
        result = knapSackTabulation(wt, val, capacity);
        break;
    case 4:
        result = knapSackSpaceOptimized(wt, val, capacity);
        break;
    case 5:
        result = knapSackBnB(capacity, val, wt, n);
        break;
    default:
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\nMaximum value in Knapsack = " << result << endl;
    return 0;
}

// | Approach        | Time Complexity    | Space  | Notes                |
// | --------------- | ------------------ | ------ | -------------------- |
// | Recursive       | O(2ⁿ)              | O(n)   | Exponential          |
// | Memoization     | O(n×W)             | O(n×W) | Top-Down             |
// | Tabulation      | O(n×W)             | O(n×W) | Bottom-Up            |
// | Space Optimized | O(n×W)             | O(W)   | Efficient            |
// | Branch & Bound  | Depends on pruning | O(n)   | Optimal with pruning |

class Solution1 {
public:
    double fractionalKnapsack(vector<int>& val, vector<int>& wt, int
    capacity) {
        int n = val.size();

        // Store pair of (value, weight)
        vector<pair<int, int>> vec(n);
        for (int i = 0; i < n; i++) {
            vec[i] = {val[i], wt[i]};
        }

        // Sort by profit/weight ratio in decreasing order
        auto lambda = [](pair<int, int>& p1, pair<int, int>& p2) {
            double r1 = (1.0 * p1.first) / p1.second;
            double r2 = (1.0 * p2.first) / p2.second;
            return r1 > r2;
        };

        sort(vec.begin(), vec.end(), lambda); // O(n log n)

        double result = 0.0;

        for (int i = 0; i < n; i++) {
            if (vec[i].second <= capacity) {
                // Take the full item
                result += vec[i].first;
                capacity -= vec[i].second;
            } else {
                // Take fractional part
                result += (1.0 * vec[i].first / vec[i].second) * capacity;
                break;
            }
        }

        return result;
    }
};

int main() {
    int n, capacity;
    cout << "Enter number of items: ";
    cin >> n;

    vector<int> val(n), wt(n);
    cout << "Enter values: ";
    for (int i = 0; i < n; i++) cin >> val[i];

    cout << "Enter weights: ";
    for (int i = 0; i < n; i++) cin >> wt[i];

    cout << "Enter knapsack capacity: ";
    cin >> capacity;

    Solution1 obj;
    double maxProfit = obj.fractionalKnapsack(val, wt, capacity);
    cout << fixed << setprecision(2);
    cout << "Maximum profit = " << maxProfit << endl;

    return 0;
}
