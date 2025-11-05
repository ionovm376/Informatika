#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

std::set<int> findCommonGreaterThanT(const std::set<int>& A, const std::set<int>& B, int t) {
    std::set<int> result;

    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), std::inserter(result, result.begin()));

    // Удаляем элементы, которые не больше t
    for (auto it = result.begin(); it != result.end(); ) {
        if (*it <= t) {
            it = result.erase(it);
        }
        else {
            ++it;
        }
    }

    return result;
}

int main() {
    std::setlocale(LC_ALL, "");
    std::set<int> A;
    std::set<int> B;
    int t;
    int x;

    std::cout << "Введите множество A (5 элементов): ";
    for (int i = 0; i < 5; i++) {
        std::cin >> x;
        A.insert(x);
    }

    std::cout << "Введите множество B (5 элементов): ";
    for (int i = 0; i < 5; i++) {
        std::cin >> x;
        B.insert(x);
    }

    std::cout << "Введите t: ";
    std::cin >> t;

    auto result = findCommonGreaterThanT(A, B, t);

    // Выводим результат
    std::cout << "Результат: ";
    for (int elem : result) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}