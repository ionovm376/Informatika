from ctypes import *
adder = CDLL('./StdDevDLL.dll')  
adder.num_even_vals_in_arr.restype = c_double 
arr = [i for i in range(1, 10001)]  
c_arr = (c_int * len(arr))(*arr)
res = adder.num_even_vals_in_arr(c_arr, len(arr)) / 100
print(res)