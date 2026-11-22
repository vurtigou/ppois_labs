#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <utility>

// Пользовательский класс
class Student {
private:
    std::string name;
    int id;
public:
    Student(std::string name, int id);
    std::string const& get_name() const { return name; }
    int get_id() const { return id; }
    bool operator<(const Student& other) const;
    bool operator==(const Student& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Student& s);
};

// Обобщённые алгоритмы 

// 1. Сортировка вставками
template <typename Iterator, typename Compare = std::less<>>
void insertionSort(Iterator begin, Iterator end, Compare comp = Compare()) {
    if (begin > end) throw std::invalid_argument("Invalid iterator range");
    if (std::distance(begin, end) <= 1) return;

    for (auto it = std::next(begin); it != end; ++it) {
        auto key = *it;
        auto j = it;
        while (j > begin && comp(key, *(j - 1))) {
            *j = *(j - 1);
            --j;
        }
        *j = key;
    }
}

// Вспомогательная функция поиска минимума и максимума
template <typename Iterator, typename KeyExtractor>
std::pair<int, int> findKeyRange(Iterator begin, Iterator end, KeyExtractor getKey) {
    int minKey = getKey(*begin);
    int maxKey = getKey(*begin);
    for (auto it = begin; it != end; ++it) {
        int currentKey = getKey(*it);
        if (currentKey < minKey) minKey = currentKey;
        if (currentKey > maxKey) maxKey = currentKey;
    }
    return {minKey, maxKey};
}

// 2. Сортировка подсчётом
template <typename Iterator, typename KeyExtractor>
void countingSort(Iterator begin, Iterator end, KeyExtractor getKey) {
    if (begin > end) throw std::invalid_argument("Invalid iterator range");
    if (std::distance(begin, end) <= 1) return;

    auto [minKey, maxKey] = findKeyRange(begin, end, getKey);
    size_t range = static_cast<size_t>(maxKey - minKey + 1);
    
    std::vector<std::vector<typename std::iterator_traits<Iterator>::value_type>> buckets(range);
    
    for (auto it = begin; it != end; ++it) {
        buckets[getKey(*it) - minKey].push_back(*it);
    }

    auto it = begin;
    for (const auto& bucket : buckets) {
        for (const auto& item : bucket) {
            *it = item;
            ++it;
        }
    }
}

// Утилита для вывода вектора
template <typename T>
void printContainer(const std::vector<T>& container) {
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// Константы меню и объявления функций
constexpr int MENU_EXIT = 0;
constexpr int MENU_INSERTION_INT = 1;
constexpr int MENU_COUNTING_INT = 2;
constexpr int MENU_INSERTION_OBJ = 3;
constexpr int MENU_COUNTING_OBJ = 4;
constexpr int MENU_RUN_TESTS = 5;

void runTests();
void demoInsertionInt();
void demoCountingInt();
void demoInsertionObj();
void demoCountingObj();

#endif // SORTING_H
