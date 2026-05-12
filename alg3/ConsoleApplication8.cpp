#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

void combSort(vector<int>& arr) {
    int n = arr.size();
    int step = n;
    bool swapped = true;

    while (step > 1 || swapped) {
        step = (int)(step / 1.3);
        if (step < 1) {
            step = 1;
        }

        swapped = false;

        for (int i = 0; i + step < n; i++) {
            if (arr[i] > arr[i + step]) {
                swap(arr[i], arr[i + step]);
                swapped = true;
            }
        }
    }
}

void shellSort(vector<int>& arr) {
    int n = arr.size();

    for (int step = n / 2; step > 0; step = step / 2) {
        for (int i = step; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= step && arr[j - step] > temp) {
                arr[j] = arr[j - step];
                j = j - step;
            }

            arr[j] = temp;
        }
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

int main() {
    vector<int> sizes = { 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };

    for (int size : sizes) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 100000;
        }

        vector<int> arr1 = arr;
        vector<int> arr2 = arr;
        vector<int> arr3 = arr;

        auto start = chrono::high_resolution_clock::now();
        combSort(arr1);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration1 = end - start;

        start = chrono::high_resolution_clock::now();
        shellSort(arr2);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration2 = end - start;

        start = chrono::high_resolution_clock::now();
        quickSort(arr3, 0, size - 1);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration3 = end - start;

        cout << "Size: " << size << endl;
        cout << "Comb Sort: " << duration1.count() << " sec" << endl;
        cout << "Shell Sort: " << duration2.count() << " sec" << endl;
        cout << "Quick Sort: " << duration3.count() << " sec" << endl;
        cout << endl;
    }

    return 0;
}