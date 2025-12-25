#include "pch.h"
#include "Header.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifndef M_PI
#endif

double num_even_vals_in_arr(const int* arr, int size) {
    clock_t start_time = clock();

    double sum = 0.0;
    for (int i = 0; i < size; ++i) sum += arr[i];
    double mean = sum / size;

    double variance = 0.0;
    for (int i = 0; i < size; ++i) {
        double diff = arr[i] - mean;
        variance += diff * diff;
    }
    double std_dev = sqrt(variance / size);

    clock_t end_time = clock();
    double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;
    return time_taken;
}