#include <iostream>
#include <cmath>
#include <vector>

// Функция для подсчета элементов больше t
void countElementsGreaterThanT(const int* arr, int size, int t, int& count) {
    count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > t) {
            count++;
        }
    }
}


void lowestarray(const int* arrA, const int* arrB, int size) {
    int countA = 0, countB = 0;
    int t;
    std::cout << "Enter a value of t: ";
    std::cin >> t;
    countElementsGreaterThanT(arrA, size, t, countA);
    countElementsGreaterThanT(arrB, size, t, countB);
    if (countA < countB) {
        std::cout << "Array A has fewer elements greater than t: " << countA << std::endl;
        std::cout << "Elements of array A: ";
        for (int i = 0; i < size; i++) {
            std::cout << arrA[i] << " ";
        }
    }
    else {
        std::cout << "Array B has fewer elements greater than t: " << countB << std::endl;
        std::cout << "Elements of array B: ";
        for (int i = 0; i < size; i++) {
            std::cout << arrB[i] << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::setlocale(LC_ALL, "");
    const int N = 5;
    int* A = new int[N];
    int* B = new int[N];
    int t;

    // Ввод массивов
    std::cout << "Введите " << N << " элементов для массива A:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }

    std::cout << "Введите " << N << " элементов для массива B:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> B[i];
    }



    lowestarray(A, B, N);



    return 0;
}
