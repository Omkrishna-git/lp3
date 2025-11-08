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
