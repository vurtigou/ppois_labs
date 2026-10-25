#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include "Set.h"

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

// Хелпер для создания строкового элемента
std::shared_ptr<SetElement> makeStr(const std::string& s) {
    return std::make_shared<StringElement>(s);
}

// ==========================================
// ТЕСТЫ
// ==========================================

void test_creation_and_basic_ops() {
    std::cout << "\n--- Тестирование базовых операций ---\n";
    
    Set emptySet;
    check(emptySet.isEmpty() == true, "Пустое множество создано");
    check(emptySet.getCardinality() == 0, "Мощность пустого множества = 0");

    Set setA;
    setA.addElement(makeStr("a"));
    setA.addElement(makeStr("b"));
    setA.addElement(makeStr("a")); // Дубликат
    
    check(setA.getCardinality() == 2, "Добавление без дубликатов");
    check(setA[makeStr("a")] == true, "Проверка принадлежности: 'a' есть");
    check(setA[makeStr("c")] == false, "Проверка принадлежности: 'c' нет");

    setA.removeElement(makeStr("a"));
    check(setA.getCardinality() == 1, "Удаление элемента");
    check(setA[makeStr("a")] == false, "Удаленный элемент больше не принадлежит множеству");
}

void test_math_operations() {
    std::cout << "\n--- Тестирование математических операций ---\n";

    Set A = Set::fromString("{a, b, c}");
    Set B = Set::fromString("{b, c, d}");

    // Объединение
    Set unionSet = A + B;
    check(unionSet.getCardinality() == 4, "Объединение: мощность 4");
    check(unionSet[makeStr("a")] && unionSet[makeStr("d")], "Объединение содержит a и d");

    // Пересечение
    Set intersectSet = A * B;
    check(intersectSet.getCardinality() == 2, "Пересечение: мощность 2");
    check(intersectSet[makeStr("b")] && intersectSet[makeStr("c")], "Пересечение содержит b и c");

    // Разность
    Set diffSet = A - B;
    check(diffSet.getCardinality() == 1, "Разность: мощность 1");
    check(diffSet[makeStr("a")], "Разность содержит 'a'");

    // Операторы с присваиванием
    Set C = A;
    C += B;
    check(C == unionSet, "Оператор += работает корректно");
}

void test_comparisons_and_clone() {
    std::cout << "\n--- Тестирование сравнений и клонирования ---\n";

    Set A = Set::fromString("{x, y}");
    Set B = Set::fromString("{y, x}");
    Set C = Set::fromString("{x, z}");

    check(A == B, "Сравнение: {x, y} == {y, x} (порядок не важен)");
    check(A != C, "Сравнение: {x, y} != {x, z}");

    // Тест глубокого копирования (конструктор копирования)
    Set copyA = A;
    copyA.addElement(makeStr("new_elem"));
    check(A.getCardinality() == 2, "Глубокое копирование: изменение копии не влияет на оригинал");

    // Тест оператора присваивания
    Set assignA;
    assignA = A;
    assignA.removeElement(makeStr("x"));
    check(A[makeStr("x")] == true, "Оператор = : изменение присвоенного объекта не влияет на оригинал");
}

void test_parsing_and_output() {
    std::cout << "\n--- Тестирование парсинга и вывода ---\n";

    // Парсинг вложенных множеств
    Set nested = Set::fromString("{a, {b, c}, d}");
    check(nested.getCardinality() == 3, "Парсинг: мощность вложенного множества 3");

    // Тест вывода
    std::stringstream ss;
    ss << nested;
    check(ss.str() == "{a, {b, c}, d}", "Вывод в поток работает корректно");

    // Тест исключения
    bool exception_caught = false;
    try {
        Set invalid = Set::fromString("a, b, c"); // Нет скобок
    } catch (const std::invalid_argument& e) {
        exception_caught = true;
    }
    check(exception_caught, "Выброс исключения при неверном формате");
}

void test_power_set() {
    std::cout << "\n--- Тестирование булеана ---\n";

    Set A = Set::fromString("{1, 2}");
    Set powerA = A.getPowerSet();
    
    // Для множества из 2 элементов булеан содержит 2^2 = 4 подмножества
    check(powerA.getCardinality() == 4, "Мощность булеана от 2 элементов = 4");

    Set emptyPower = Set().getPowerSet();
    check(emptyPower.getCardinality() == 1, "Булеан пустого множества содержит 1 элемент (пустое подмножество)");
}

void test_missing_operations() {
    std::cout << "\n--- Тестирование оставшихся операций ---\n";

    Set A = Set::fromString("{a, b, c}");
    Set B = Set::fromString("{b, c, d}");

    // 1. Тестируем *= и -=
    Set C = A;
    C *= B;
    check(C.getCardinality() == 2, "Оператор *= работает");

    C = A;
    C -= B;
    check(C.getCardinality() == 1 && C[makeStr("a")], "Оператор -= работает");

    // 2. Тестируем !=
    check(A != B, "Оператор != работает");

    // 3. Тестируем удаление элемента, которого нет
    Set emptySet;
    emptySet.removeElement(makeStr("x")); // Не должно упасть
    check(emptySet.isEmpty(), "Удаление несуществующего элемента безопасно");

    // 4. Тестируем сравнение разных типов (Строка != Множество)
    auto strElem = makeStr("hello");
    auto setElem = std::make_shared<SetElementWrapper>(std::make_shared<Set>());
    check(!(*strElem == *setElem), "Сравнение строки и множества дает false");

    // 5. Тестируем чтение из потока (operator>>)
    std::stringstream input(" {x, y} "); // С пробелами по краям
    Set readSet;
    input >> readSet;
    check(readSet.getCardinality() == 2, "Чтение из потока (>>) работает");

    // 6. Тестируем пустой вывод
    Set zeroSet;
    std::stringstream ss;
    ss << zeroSet;
    check(ss.str() == "{}", "Вывод пустого множества");
}
void test_missing_coverage_boost() {
    std::cout << "\n--- Тестирование для повышения покрытия ---\n";
    
    // 1. Тест для getValue() (поднимает покрытие Set.h до 100%)
    auto strElem = std::make_shared<StringElement>("test_str");
    check(strElem->getValue() == "test_str", "Метод getValue() работает");

    // 2. Тест для getSet() и toString() обертки (поднимает Set.cpp)
    auto innerSet = std::make_shared<Set>();
    innerSet->addElement(strElem);
    auto wrapper = std::make_shared<SetElementWrapper>(innerSet);
    
    check(wrapper->getSet() == innerSet, "Метод getSet() возвращает внутреннее множество");
    check(wrapper->toString() == "{test_str}", "Метод toString() у обертки работает");

    // 3. Тест несовпадения типов (когда dynamic_cast возвращает nullptr)
    // Это покроет те самые красные строчки "return false" в operator==
    check(!(*strElem == *wrapper), "Сравнение Строки и Множества = false");
    check(!(*wrapper == *strElem), "Сравнение Множества и Строки = false");

    // 4. Тест глубокого клонирования обертки
    auto clonedWrapper = wrapper->clone();
    check(*wrapper == *clonedWrapper, "Клонирование обертки работает");
}

// ==========================================
// MAIN
// ==========================================
int main() {
    std::cout << "=== ЗАПУСК ЮНИТ-ТЕСТОВ ДЛЯ КЛАССА SET ===\n";
    
    test_creation_and_basic_ops();
    test_math_operations();
    test_comparisons_and_clone();
    test_parsing_and_output();
    test_power_set();
    test_power_set();
    test_missing_operations();
    test_power_set();
    
    test_missing_coverage_boost();
    
    std::cout << "\n=========================================\n";
    std::cout << "РЕЗУЛЬТАТ: Пройдено " << passed_tests << " из " << total_tests << " тестов.\n";
    
    if (passed_tests == total_tests) {
        std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! \n";
        return 0; // Успех для CI/CD
    } else {
        std::cout << "ЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ! \n";
        return 1; // Ошибка для CI/CD
    }
}
