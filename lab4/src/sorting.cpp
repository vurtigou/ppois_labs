#include "sorting.h"
#include <cassert>

// Реализация методов класса Student
Student::Student(std::string name, int id) : name(std::move(name)), id(id) {}

bool Student::operator<(const Student& other) const {
    return name < other.name;
}

bool Student::operator==(const Student& other) const {
    return name == other.name && id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << "{" << s.name << ", id=" << s.id << "}";
    return os;
}

// Модульные тесты
void runTests() {
    // Тест 1: Insertion Sort для вектора int
    std::vector<int> v1 = {5, 2, 9, 1, 5, 6};
    insertionSort(v1.begin(), v1.end());
    assert((v1 == std::vector<int>{1, 2, 5, 5, 6, 9}));

    // Тест 2: Insertion Sort для пустого вектора
    std::vector<int> v_empty;
    insertionSort(v_empty.begin(), v_empty.end());
    assert(v_empty.empty());

    // Тест 3: Insertion Sort для обычного массива C-style
    int arr[] = {3, 1, 4, 1, 5};
    insertionSort(arr, arr + 5);
    assert((arr[0] == 1 && arr[1] == 1 && arr[2] == 3 && arr[3] == 4 && arr[4] == 5));

    // Тест 4: Counting Sort для вектора int (включая отрицательные числа)
    std::vector<int> v2 = {4, -2, 4, 0, 1, -2};
    countingSort(v2.begin(), v2.end(), [](int x) { return x; });
    assert((v2 == std::vector<int>{-2, -2, 0, 1, 4, 4}));

    // Тест 5: Insertion Sort для пользовательского класса
    std::vector<Student> students = {{"Ivan", 3}, {"Anna", 1}, {"Boris", 2}};
    insertionSort(students.begin(), students.end());
    assert(students[0].name == "Anna" && students[1].name == "Boris" && students[2].name == "Ivan");

    // Тест 6: Counting Sort для пользовательского класса (по ID)
    countingSort(students.begin(), students.end(), [](const Student& s) { return s.id; });
    assert(students[0].id == 1 && students[1].id == 2 && students[2].id == 3);

    std::cout << "[OK] Все модульные тесты пройдены успешно!\n\n";
}
 
// Демонстрационные функции (CLI)
void demoInsertionInt() {
    std::vector<int> data = {8, 3, 1, 7, 0, 10, 2};
    std::cout << "\nДо сортировки: ";
    printContainer(data);
    insertionSort(data.begin(), data.end());
    std::cout << "После сортировки вставками: ";
    printContainer(data);
}

void demoCountingInt() {
    std::vector<int> data = {4, -1, 3, 4, 1, -1, 0};
    std::cout << "\nДо сортировки: ";
    printContainer(data);
    countingSort(data.begin(), data.end(), [](int x) { return x; });
    std::cout << "После сортировки подсчётом: ";
    printContainer(data);
}

void demoInsertionObj() {
    std::vector<Student> data = {{"Oleg", 3}, {"Anna", 1}, {"Maria", 2}};
    std::cout << "\nДо сортировки:\n";
    printContainer(data);
    insertionSort(data.begin(), data.end());
    std::cout << "После сортировки вставками (по имени):\n";
    printContainer(data);
}

void demoCountingObj() {
    std::vector<Student> data = {{"Oleg", 30}, {"Anna", 10}, {"Maria", 20}};
    std::cout << "\nДо сортировки:\n";
    printContainer(data);
    countingSort(data.begin(), data.end(), [](const Student& s) { return s.id; });
    std::cout << "После сортировки подсчётом (по ID):\n";
    printContainer(data);
}
