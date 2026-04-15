#include <vector>
#include <algorithm>

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
    return 0;
}