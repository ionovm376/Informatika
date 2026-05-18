#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
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

void parallelQuickSort(vector<int>& arr, int low, int high, int num_threads) {
    if (low >= high)
        return;
    if (num_threads <= 1 || high - low < 10000) {
        quickSort(arr, low, high);
        return;
    }
    int pivotIndex = partition(arr, low, high);
    int left_threads = num_threads / 2;
    thread left_thread(
        parallelQuickSort,
        ref(arr),
        low,
        pivotIndex - 1,
        left_threads
    );
    parallelQuickSort(
        arr,
        pivotIndex + 1,
        high,
        num_threads - left_threads
    );
    left_thread.join();
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> sizes = {
        100000,
        200000,
        300000,
        400000,
        500000,
        600000,
        700000,
        800000,
        900000,
        1000000
    };
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 50000);
    for (int size : sizes) {
        vector<int> arr(size);
        for (int& x : arr) {
            x = distrib(gen);
        }
        vector<int> arr1 = arr;
        auto start1 =
            chrono::high_resolution_clock::now();
        quickSort(arr1, 0, arr1.size() - 1);
        auto end1 =
            chrono::high_resolution_clock::now();
        double time1 =
            chrono::duration_cast<chrono::microseconds>(
                end1 - start1
            ).count() / 1000000.0;
        vector<int> arr2 = arr;
        auto start2 =
            chrono::high_resolution_clock::now();
        parallelQuickSort(
            arr2,
            0,
            arr2.size() - 1,
            2
        );
        auto end2 =
            chrono::high_resolution_clock::now();
        double time2 =
            chrono::duration_cast<chrono::microseconds>(
                end2 - start2
            ).count() / 1000000.0;
        vector<int> arr4 = arr;
        auto start4 =
            chrono::high_resolution_clock::now();
        parallelQuickSort(
            arr4,
            0,
            arr4.size() - 1,
            4
        );
        auto end4 =
            chrono::high_resolution_clock::now();
        double time4 =
            chrono::duration_cast<chrono::microseconds>(
                end4 - start4
            ).count() / 1000000.0;
        vector<int> arr8 = arr;
        auto start8 =
            chrono::high_resolution_clock::now();
        parallelQuickSort(
            arr8,
            0,
            arr8.size() - 1,
            8
        );
        auto end8 =
            chrono::high_resolution_clock::now();
        double time8 =
            chrono::duration_cast<chrono::microseconds>(
                end8 - start8
            ).count() / 1000000.0;
        cout << "Размер массива: "
            << size << endl;
        cout << "Быстрая сортировка: "
            << time1 << " сек" << endl;
        cout << "2 потока: "
            << time2 << " сек" << endl;
        cout << "4 потока: "
            << time4 << " сек" << endl;
        cout << "8 потоков: "
            << time8 << " сек" << endl;
        cout << endl;
    }
    cin.get();
    return 0;
}