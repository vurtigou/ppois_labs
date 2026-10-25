#include <iostream>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include "Polynomial.h"

// --- Мини-фреймворк для тестирования ---
int total_tests = 0;
int passed_tests = 0;

void check(bool condition, const std::string& test_name) {
    total_tests++;
    if (condition) {
        passed_tests++;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

// ==========================================
// ТЕСТЫ
// ==========================================

void test_creation_and_access() {
    std::cout << "\n--- Тестирование создания и доступа ---\n";

    // По умолчанию нулевой многочлен
    Polynomial zero;
    check(zero.isZero() == true, "Конструктор по умолчанию создает 0");
    check(zero.degree() == 0, "Степень нулевого многочлена = 0");

    // Конструктор из вектора (например, 3x^2 + 2x + 1)
    Polynomial p({1.0, 2.0, 3.0});
    check(p.degree() == 2, "Степень многочлена 3x^2 + 2x + 1 равна 2");
    check(p[0] == 1.0 && p[1] == 2.0 && p[2] == 3.0, "Получение коэффициентов через []");
    check(p[3] == 0.0, "Получение коэффициента за пределами степени возвращает 0.0");

    // Удаление ведущих нулей (например, 0x^2 + 2x + 1)
    Polynomial p_zeros({1.0, 2.0, 0.0, 0.0});
    check(p_zeros.degree() == 1, "Ведущие нули удалены (степень стала 1)");

    // Исключение при отрицательной степени
    bool exception_caught = false;
    try {
        p[-1];
    } catch (const std::out_of_range& e) {
        exception_caught = true;
    }
    check(exception_caught, "Выброс исключения при запросе отрицательной степени");
}

void test_evaluation() {
    std::cout << "\n--- Тестирование вычисления значения P(x) ---\n";
    
    // P(x) = 2x^2 + 3x + 1
    Polynomial p({1.0, 3.0, 2.0});
    
    check(p(2.0) == 15.0, "P(2) = 2*4 + 3*2 + 1 = 15");
    check(p(0.0) == 1.0, "P(0) = 1");
    check(p(-1.0) == 0.0, "P(-1) = 0");
    
    Polynomial zero;
    check(zero(100.0) == 0.0, "Значение нулевого многочлена всегда 0");
}

void test_math_operations() {
    std::cout << "\n--- Тестирование математических операций ---\n";

    Polynomial A({1.0, 2.0, 1.0}); // x^2 + 2x + 1
    Polynomial B({1.0, 1.0});      // x + 1

    // Сложение
    Polynomial sum = A + B;
    check(sum == Polynomial({2.0, 3.0, 1.0}), "Сложение: (x^2+2x+1) + (x+1) = x^2+3x+2");

    // Вычитание
    Polynomial diff = A - B;
    check(diff == Polynomial({0.0, 1.0, 1.0}), "Вычитание: (x^2+2x+1) - (x+1) = x^2+x");

    // Умножение
    Polynomial prod = A * B;
    check(prod == Polynomial({1.0, 3.0, 3.0, 1.0}), "Умножение: (x^2+2x+1) * (x+1) = x^3+3x^2+3x+1");
    
    // Умножение на ноль
    Polynomial zero;
    check((A * zero).isZero(), "Умножение на нулевой многочлен дает 0");

    // Деление
    Polynomial div = A / B; // (x^2+2x+1) / (x+1) = x+1
    check(div == B, "Деление: (x^2+2x+1) / (x+1) = x+1");

    // Деление меньшего на большее (результат 0)
    check((B / A).isZero(), "Деление меньшего на большее дает 0");

    // Деление на ноль (исключение)
    bool exception_caught = false;
    try {
        A / zero;
    } catch (const std::invalid_argument& e) {
        exception_caught = true;
    }
    check(exception_caught, "Выброс исключения при делении на нулевой многочлен");

    // Операторы с присваиванием
    Polynomial C = A;
    C += B;
    check(C == sum, "Оператор += работает корректно");
    
    C = A;
    C *= B;
    check(C == prod, "Оператор *= работает корректно");
}

void test_comparisons() {
    std::cout << "\n--- Тестирование сравнений ---\n";

    Polynomial A({1.0, 2.0, 3.0});
    Polynomial B({1.0, 2.0, 3.0});
    Polynomial C({1.0, 2.0, 4.0});
    Polynomial D({1.0, 2.0}); // Другая степень

    check(A == B, "Равенство: одинаковые многочлены");
    check(A != C, "Неравенство: разные коэффициенты");
    check(A != D, "Неравенство: разные степени");

    // Проверка с учетом EPSILON (погрешности)
    Polynomial E({1.0000000001, 2.0});
    Polynomial F({1.0, 2.0});
    check(E == F, "Сравнение с учетом EPSILON для double");
}

void test_io() {
    std::cout << "\n--- Тестирование ввода/вывода ---\n";

    // Вывод
    Polynomial p({1.0, -2.0, 3.0}); // 3x^2 - 2x + 1
    std::stringstream ss;
    ss << p;
    check(ss.str() == "3x^2 - 2x + 1", "Вывод в поток форматируется верно: " + ss.str());

    Polynomial zero;
    ss.str("");
    ss << zero;
    check(ss.str() == "0", "Вывод нулевого многочлена");
    
    // Ввод
    std::stringstream input("2 1 2 3"); // Степень 2, коэффициенты 1, 2, 3
    Polynomial read_p;
    input >> read_p;
    check(read_p == Polynomial({1.0, 2.0, 3.0}), "Чтение из потока (>>) работает");

    // Ввод с отрицательной степенью (исключение)
    std::stringstream bad_input("-1 5");
    bool exception_caught = false;
    try {
        bad_input >> read_p;
    } catch (const std::invalid_argument& e) {
        exception_caught = true;
    }
    check(exception_caught, "Выброс исключения при вводе отрицательной степени");
}

// ==========================================
// MAIN
// ==========================================
int main() {
    std::cout << "=== ЗАПУСК ЮНИТ-ТЕСТОВ ДЛЯ КЛАССА POLYNOMIAL ===\n";
    
    test_creation_and_access();
    test_evaluation();
    test_math_operations();
    test_comparisons();
    test_io();
    
    std::cout << "\n=========================================\n";
    std::cout << "РЕЗУЛЬТАТ: Пройдено " << passed_tests << " из " << total_tests << " тестов.\n";
    
    if (passed_tests == total_tests) {
        std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! ✅\n";
        return 0;
    } else {
        std::cout << "ЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ! ❌\n";
        return 1;
    }
}
