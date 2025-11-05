def count_elements_greater_than_t(arr, t):
    return sum(1 for x in arr if x > t)

def lowest_array(arrA, arrB):
    t = int(input("Enter a value of t: "))
    countA = count_elements_greater_than_t(arrA, t)
    countB = count_elements_greater_than_t(arrB, t)

    if countA < countB:
        print(f"Array A has fewer elements greater than t: {countA}")
        print("Elements of array A:", *arrA)
    else:
        print(f"Array B has fewer elements greater than t: {countB}")
        print("Elements of array B:", *arrB)

def main():
    N = 5
    A = list(map(int, input(f"Введите {N} элементов для массива A: ").split()))
    B = list(map(int, input(f"Введите {N} элементов для массива B: ").split()))
    lowest_array(A, B)

if __name__ == "__main__":
    main()
