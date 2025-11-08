#include <bits/stdc++.h>
using namespace std;

int arr[100];

int partition(int l, int h) {
    int pivot = arr[l];
    int i = l, j = h;

    while (i < j) {
        // Move i forward until arr[i] > pivot
        do {
            i++;
        } while (arr[i] <= pivot && i < h);

        // Move j backward until arr[j] <= pivot
        do {
            j--;
        } while (arr[j] > pivot);

        if (i < j)
            swap(arr[i], arr[j]);
    }

    swap(arr[l], arr[j]);
    return j;
}

void quick_sort(int l, int h) {
    if (l < h) {
        int j = partition(l, h);
        quick_sort(l, j);
        quick_sort(j + 1, h);
    }
}

int randomized_partition(int l, int h) {
    int random_index = l + rand() % (h - l);
    swap(arr[l], arr[random_index]);
    return partition(l, h);
}

void randomized_quick_sort(int l, int h) {
    if (l < h) {
        int j = randomized_partition(l, h);
        randomized_quick_sort(l, j);
        randomized_quick_sort(j + 1, h);
    }
}


int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    arr[n] = INT_MAX;

    quick_sort(0, n);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}


// | Feature                  | Deterministic Quick Sort    | Randomized Quick Sort            |
// | ------------------------ | --------------------------- | -------------------------------- |
// | Pivot Choice             | Fixed (e.g., first element) | Random element                   |
// | Best Case                | O(n log n)                  | O(n log n)                       |
// | Average Case             | O(n log n)                  | O(n log n)                       |
// | Worst Case               | O(n²)                       | O(n²) (rare)                     |
// | Space                    | O(log n)                    | O(log n)                         |
// | Behavior on sorted input | Worst case                  | Safe (random pivot breaks order) |
// | Practical Performance    | May degrade for sorted data | Stable for any input             |


// #include <bits/stdc++.h>
// using namespace std;

// int arr[100];

// int partition(int l, int h) {
//     int pivot = arr[l];
//     int i = l, j = h;

//     while (i < j) {
//         do { i++; } while (i <= h && arr[i] <= pivot);
//         do { j--; } while (arr[j] > pivot);

//         if (i < j)
//             swap(arr[i], arr[j]);
//     }

//     swap(arr[l], arr[j]);
//     return j;
// }

// void quick_sort(int l, int h) {
//     if (l < h) {
//         int j = partition(l, h);
//         quick_sort(l, j);
//         quick_sort(j + 1, h);
//     }
// }

// int randomized_partition(int l, int h) {
//     int random_index = l + rand() % (h - l + 1);
//     swap(arr[l], arr[random_index]);
//     return partition(l, h);
// }

// void randomized_quick_sort(int l, int h) {
//     if (l < h) {
//         int j = randomized_partition(l, h);
//         randomized_quick_sort(l, j);
//         randomized_quick_sort(j + 1, h);
//     }
// }

// int main() {
//     int n;
//     cout << "Enter number of elements: ";
//     cin >> n;

//     cout << "Enter " << n << " elements: ";
//     for (int i = 0; i < n; i++) {
//         cin >> arr[i];
//     }

//     srand(time(0)); // for random pivot
//     randomized_quick_sort(0, n - 1);

//     cout << "Sorted array: ";
//     for (int i = 0; i < n; i++) {
//         cout << arr[i] << " ";
//     }
//     cout << endl;

//     return 0;
// }
