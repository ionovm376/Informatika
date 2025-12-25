#pragma once

#ifdef STDDLL_EXPORTS  // или MYLIBRARY_EXPORTS
#define STDDLL_API __declspec(dllexport)
#else
#define STDDLL_API __declspec(dllimport)
#endif

extern "C" {
    STDDLL_API double num_even_vals_in_arr(const int arr[], int size);
}