#include "cli/ConsoleMenu.h"
#include "model/Student.h"

// Заголовки из вашего проекта (lab4/include/...)
#include "sorting/InsertionSort.h"
#include "sorting/CountingSort.h"
#include "graph/Graph.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace cli {

namespace {

std::vector<model::Student> makeSampleStudents() {
    return {
        model::Student(104, "Ivan Petrov", "CS-21", 4.65, 19),
        model::Student(101, "Anna Smirnova", "CS-21", 4.90, 18),
        model::Student(107, "Dmitry Volkov", "CS-22", 3.80, 20),
        model::Student(102, "Elena Kuznetsova", "CS-22", 4.65, 19),
        model::Student(105, "Sergey Morozov", "CS-21", 4.10, 18),
    };
}

template <typename T>
void printVector(const std::string& title, const std::vector<T>& items) {
    std::cout << "\n--- " << title << " ---\n";
    for (std::size_t i = 0; i < items.size(); ++i) {
        std::cout << "  [" << i << "] " << items[i] << '\n';
    }
}

} // namespace

void ConsoleMenu::run() {
    bool running = true;
    while (running) {
        printMainMenu();
        const int choice = readInt("Выберите пункт: ", 0, 2);

        switch (choice) {
            case 1:
                handleSortingTask();
                break;
            case 2:
                handleGraphTask();
                break;
            case 0:
                running = false;
                std::cout << "Выход из программы.\n";
                break;
            default:
                break;
        }
    }
}

void ConsoleMenu::printMainMenu() {
    std::cout << "\n========================================\n"
              << "       ЛАБОРАТОРНАЯ РАБОТА №4 (CLI)       \n"
              << "========================================\n"
              << "1. Задание 1: Сортировка\n"
              << "2. Задание 2: Графы\n"
              << "0. Выход\n";
}

void ConsoleMenu::printSortingMenu() {
    std::cout << "\n--- Меню сортировки (Задание 1) ---\n"
              << "1. Демонстрация сортировки объектов Student (std::vector)\n"
              << "2. Демонстрация сортировки массива int (C-массив + std::vector)\n"
              << "3. Сравнение с std::sort / std::stable_sort\n"
              << "0. Назад\n";
}

void ConsoleMenu::printGraphMenu() {
    std::cout << "\n--- Меню графов (Задание 2) ---\n"
              << "1. Демонстрация Graph<Student> (добавление вершин/рёбер, вывод)\n"
              << "2. Демонстрация Graph<int> (копирование, сравнение, очистка)\n"
              << "0. Назад\n";
}

void ConsoleMenu::handleSortingTask() {
    bool back = false;
    while (!back) {
        printSortingMenu();
        const int choice = readInt("Выберите действие: ", 0, 3);

        switch (choice) {
            case 1: {
                auto students = makeSampleStudents();
                printVector("Исходный список студентов", students);

                // Используем обобщённую функцию InsertionSort из вашего проекта
                sorting::insertionSort(students);
                printVector("После сортировки (по умолчанию: GPA, ID, ФИО)", students);

                std::sort(students.begin(), students.end(), model::Student::compareByName);
                printVector("После сортировки по ФИО", students);
                break;
            }
            case 2: {
                int rawArr[] = {42, 7, 19, 3, 25, 19, 1, 14};
                constexpr std::size_t kSize = sizeof(rawArr) / sizeof(rawArr[0]);

                std::vector<int> vec(rawArr, rawArr + kSize);
                printVector("Исходный std::vector<int>", vec);

                sorting::insertionSort(vec);
                printVector("После insertionSort на std::vector<int>", vec);

                std::vector<int> nonNeg = {12, 4, 0, 9, 4, 2, 15, 7};
                printVector("Вектор неотрицательных чисел для CountingSort", nonNeg);
                sorting::countingSort(nonNeg);
                printVector("После countingSort", nonNeg);
                break;
            }
            case 3: {
                auto a = makeSampleStudents();
                auto b = makeSampleStudents();

                sorting::insertionSort(a);
                std::stable_sort(b.begin(), b.end());

                std::cout << "\nРезультаты совпадают: "
                          << (a == b ? "ДА" : "НЕТ") << '\n';
                printVector("Итоговый порядок студентов", a);
                break;
            }
            case 0:
                back = true;
                break;
            default:
                break;
        }
    }
}

void ConsoleMenu::handleGraphTask() {
    bool back = false;
    while (!back) {
        printGraphMenu();
        const int choice = readInt("Выберите действие: ", 0, 2);

        switch (choice) {
            case 1: {
                graph::Graph<model::Student> studentGraph;
                const auto students = makeSampleStudents();

                std::cout << "\nДобавление вершин типа Student в граф...\n";
                for (const auto& s : students) {
                    studentGraph.addVertex(s);
                }

                if (students.size() >= 4) {
                    studentGraph.addEdge(students[0], students[1]);
                    studentGraph.addEdge(students[1], students[2]);
                    studentGraph.addEdge(students[2], students[3]);
                }

                std::cout << "Количество вершин: " << studentGraph.vertexCount() << '\n';
                std::cout << "Количество рёбер:  " << studentGraph.edgeCount() << '\n';
                std::cout << "Содержимое графа:\n" << studentGraph << '\n';
                break;
            }
            case 2: {
                graph::Graph<int> g1;
                g1.addVertex(10);
                g1.addVertex(20);
                g1.addVertex(30);
                g1.addEdge(10, 20);
                g1.addEdge(20, 30);

                graph::Graph<int> g2(g1);
                std::cout << "\ng1 == g2 после копирования: "
                          << (g1 == g2 ? "ДА" : "НЕТ") << '\n';

                g2.addVertex(40);
                std::cout << "g1 != g2 после добавления вершины в g2: "
                          << (g1 != g2 ? "ДА" : "НЕТ") << '\n';

                g2.clear();
                std::cout << "g2 пуст после clear(): "
                          << (g2.empty() ? "ДА" : "НЕТ") << '\n';
                break;
            }
            case 0:
                back = true;
                break;
            default:
                break;
        }
    }
}

int ConsoleMenu::readInt(const char* prompt, int minVal, int maxVal) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputStream();
            return value;
        }
        std::cout << "Ошибка ввода. Введите целое число в диапазоне ["
                  << minVal << "; " << maxVal << "].\n";
        clearInputStream();
    }
}

double ConsoleMenu::readDouble(const char* prompt, double minVal, double maxVal) {
    double value = 0.0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputStream();
            return value;
        }
        std::cout << "Ошибка ввода. Введите число в диапазоне ["
                  << minVal << "; " << maxVal << "].\n";
        clearInputStream();
    }
}

void ConsoleMenu::clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace cli
