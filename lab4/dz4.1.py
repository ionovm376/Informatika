def find_common_greater_than_t(A, B, t):

    result = A & B

    result = {x for x in result if x > t}
    return result

def main():
    print("Введите множество A (5 элементов): ")
    A = set()
    for i in range(5):
        x = int(input())
        A.add(x)

    print("Введите множество B (5 элементов): ")
    B = set()
    for i in range(5):
        x = int(input())
        B.add(x)

    t = int(input("Введите t: "))

    result = find_common_greater_than_t(A, B, t)

    print("Результат:", *result)

if __name__ == "__main__":
    main()
