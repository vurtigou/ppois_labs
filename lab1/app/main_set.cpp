#include <iostream>
#include <string>
#include <limits>
#include "Set.h" // Подключаем класс

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Set setA, setB;
    int choice;

    while (true) {
        std::cout << "\n=== МЕНЮ ТЕСТИРОВАНИЯ МНОЖЕСТВ ===\n";
        std::cout << "1. Ввести множество A\n";
        std::cout << "2. Ввести множество B\n";
        std::cout << "3. Вывести A и B\n";
        std::cout << "4. Объединение (A + B)\n";
        std::cout << "5. Пересечение (A * B)\n";
        std::cout << "6. Разность (A - B)\n";
        std::cout << "7. Построить булеан (для A)\n";
        std::cout << "8. Проверить A на пустоту\n";
        std::cout << "9. Мощность множества A\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        
        if (!(std::cin >> choice)) {
            std::cout << "Ошибка: введите число.\n";
            clearInput();
            continue;
        }
        clearInput(); // Очищаем буфер после ввода числа

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Введите A (например, {a, b, {c, d}}): ";
                    std::cin >> setA;
                    break;
                }
                case 2: {
                    std::cout << "Введите B (например, {a, c}): ";
                    std::cin >> setB;
                    break;
                }
                case 3:
                    std::cout << "A = " << setA << "\n";
                    std::cout << "B = " << setB << "\n";
                    break;
                case 4:
                    std::cout << "A + B = " << (setA + setB) << "\n";
                    break;
                case 5:
                    std::cout << "A * B = " << (setA * setB) << "\n";
                    break;
                case 6:
                    std::cout << "A - B = " << (setA - setB) << "\n";
                    break;
                case 7:
                    std::cout << "Булеан A = " << setA.getPowerSet() << "\n";
                    break;
                case 8:
                    std::cout << "Множество A " << (setA.isEmpty() ? "пустое" : "не пустое") << "\n";
                    break;
                case 9:
                    std::cout << "Мощность A = " << setA.getCardinality() << "\n";
                    break;
                case 0:
                    std::cout << "Выход...\n";
                    return 0;
                default:
                    std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "ОШИБКА: " << e.what() << "\n";
        }
    }
    return 0;
}
