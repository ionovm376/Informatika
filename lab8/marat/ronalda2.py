import time
import math

def num_even_vals_in_arr(arr):
    start_time = time.time()
    
    mean = sum(arr) / len(arr)
    variance = sum((x - mean) ** 2 for x in arr) / len(arr)
    math.sqrt(variance)
    
    end_time = time.time()
    return end_time - start_time

massiv = [i for i in range(1, 10000001)]
print(round(num_even_vals_in_arr(massiv), 2))