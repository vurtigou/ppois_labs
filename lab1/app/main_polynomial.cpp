#include <iostream>
#include <vector>
#include "Polynomial.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

int main() {
    Polynomial p1, p2;
    int choice;

    while (true) {
        std::cout << "\n=== МЕНЮ МНОГОЧЛЕНОВ ===\n";
        std::cout << "1. Ввести многочлен A\n";
        std::cout << "2. Ввести многочлен B\n";
        std::cout << "3. Вывести A и B\n";
        std::cout << "4. Сложение (A + B)\n";
        std::cout << "5. Вычитание (A - B)\n";
        std::cout << "6. Умножение (A * B)\n";
        std::cout << "7. Деление (A / B)\n";
        std::cout << "8. Вычислить A(x)\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cout << "Ошибка ввода.\n";
            clearInput();
            continue;
        }
        clearInput();

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Введите степень A: ";
                    int deg; std::cin >> deg;
                    std::cout << "Введите " << deg + 1 << " коэффициентов (от младшего к старшему): ";
                    std::vector<double> c(deg + 1);
                    for(int i = 0; i <= deg; ++i) std::cin >> c[i];
                    p1 = Polynomial(c);
                    break;
                }
                case 2: {
                    std::cout << "Введите степень B: ";
                    int deg; std::cin >> deg;
                    std::cout << "Введите " << deg + 1 << " коэффициентов: ";
                    std::vector<double> c(deg + 1);
                    for(int i = 0; i <= deg; ++i) std::cin >> c[i];
                    p2 = Polynomial(c);
                    break;
                }
                case 3:
                    std::cout << "A = " << p1 << "\nB = " << p2 << "\n";
                    break;
                case 4:
                    std::cout << "A + B = " << (p1 + p2) << "\n";
                    break;
                case 5:
                    std::cout << "A - B = " << (p1 - p2) << "\n";
                    break;
                case 6:
                    std::cout << "A * B = " << (p1 * p2) << "\n";
                    break;
                case 7:
                    std::cout << "A / B = " << (p1 / p2) << "\n";
                    break;
                case 8: {
                    std::cout << "Введите x: ";
                    double x; std::cin >> x;
                    std::cout << "A(" << x << ") = " << p1(x) << "\n";
                    break;
                }
                case 0:
                    return 0;
            }
        } catch (const std::exception& e) {
            std::cout << "ОШИБКА: " << e.what() << "\n";
        }
    }
}
