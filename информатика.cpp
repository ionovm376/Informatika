#include <iostream>
#include <cmath>

using namespace std;

int main() {
    float x, y, R, S, C;
    const float PI = 3.14159265358979323846;

    // Ввод данных
    cout << "Enter x: ";
    cin >> x;
    cout << "Enter y: ";
    cin >> y;

    // Вычисление R = ctg(π/4 * x)
    float argument = (PI / 4) * x;
    R = cos(argument) / sin(argument);

    // Вычисление S = ⁴√y * (1/2^y)
    S = pow(y, 0.25) * (1.0 / pow(2.0, y));

    // max(a,b) = (a + b + |a - b|) / 2
    C = (R + S + abs(R - S)) / 2.0;

    // Вывод результатов
    cout << "R = " << R << endl;
    cout << "S = " << S << endl;
    cout << "C (max) = " << C << endl;

    return 0;
}