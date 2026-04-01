#include <iostream>
#include <cmath>

double simpson_rule(double (*f)(double), double a, double b, int n) {
    if (n % 2 == 1) n++; 
    double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; i += 2)
        sum += 4 * f(a + i * h);

    for (int i = 2; i < n - 1; i += 2)
        sum += 2 * f(a + i * h);

    return (h / 3) * sum;
}

double function1(double x) {
    return 3 * pow(x, 2) - 2;
}

double function2(double x) {
    return -pow(x, 2) + 6;
}

double diff(double x) {
    return function1(x) - function2(x);
}

int main() {
    double a = 2.0;
    double b = 7.0;

    std::cout << "N = 2: " << simpson_rule(diff, a, b, 2) << std::endl;
    std::cout << "N = 4: " << simpson_rule(diff, a, b, 4) << std::endl;
    std::cout << "N = 10: " << simpson_rule(diff, a, b, 10) << std::endl;
    std::cout << "N = 18: " << simpson_rule(diff, a, b, 18) << std::endl;
    std::cout << "N = 100: " << simpson_rule(diff, a, b, 100) << std::endl;
    std::cout << "N = 1000: " << simpson_rule(diff, a, b, 1000) << std::endl;

    return 0;
}