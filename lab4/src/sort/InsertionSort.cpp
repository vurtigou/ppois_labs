#ifndef INSERTION_SORT_CPP
#define INSERTION_SORT_CPP

#include "InsertionSort.h"

// Сортировка диапазона с компаратором по возрастанию
template <typename RandomIt, typename Compare>
void InsertionSort::sort(RandomIt first, RandomIt last, Compare comp) {
    if (first > last) {
        throw std::invalid_argument("Некорректный диапазон итераторов: first > last");
    }
    
    if (first == last || std::next(first) == last) {
        return;
    }

    for (RandomIt it = std::next(first); it != last; ++it) {
        auto key = std::move(*it);
        RandomIt j = it;
        
        while (j != first && comp(key, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        *j = std::move(key);
    }
}

// Сортировка диапазона по возрастанию (по умолчанию)
template <typename RandomIt>
void InsertionSort::sort(RandomIt first, RandomIt last) {
    sort(first, last, std::less<>());
}

// Сортировка диапазона с компаратором по убыванию
template <typename RandomIt, typename Compare>
void InsertionSort::sort_descending(RandomIt first, RandomIt last, Compare comp) {
    // Инвертируем компаратор для достижения обратного порядка
    auto inverted_comp = [&comp](const auto& a, const auto& b) {
        return comp(b, a);
    };
    sort(first, last, inverted_comp);
}

// Сортировка диапазона по убыванию (по умолчанию)
template <typename RandomIt>
void InsertionSort::sort_descending(RandomIt first, RandomIt last) {
    sort_descending(first, last, std::less<>());
}

// Сортировка вектора с компаратором по возрастанию
template <typename T, typename Compare>
void InsertionSort::sort(std::vector<T>& vec, Compare comp) {
    sort(vec.begin(), vec.end(), comp);
}

// Сортировка вектора по возрастанию
template <typename T>
void InsertionSort::sort(std::vector<T>& vec) {
    sort(vec.begin(), vec.end());
}

// Сортировка вектора с компаратором по убыванию
template <typename T, typename Compare>
void InsertionSort::sort_descending(std::vector<T>& vec, Compare comp) {
    sort_descending(vec.begin(), vec.end(), comp);
}

// Сортировка вектора по убыванию
template <typename T>
void InsertionSort::sort_descending(std::vector<T>& vec) {
    sort_descending(vec.begin(), vec.end());
}

// Сортировка массива с компаратором по возрастанию
template <typename T, typename Compare>
void InsertionSort::sort(T* arr, std::size_t size, Compare comp) {
    if (arr != nullptr && size > 0) {
        sort(arr, arr + size, comp);
    }
}

// Сортировка массива по возрастанию
template <typename T>
void InsertionSort::sort(T* arr, std::size_t size) {
    if (arr != nullptr && size > 0) {
        sort(arr, arr + size);
    }
}

// Сортировка массива с компаратором по убыванию
template <typename T, typename Compare>
void InsertionSort::sort_descending(T* arr, std::size_t size, Compare comp) {
    if (arr != nullptr && size > 0) {
        sort_descending(arr, arr + size, comp);
    }
}

// Сортировка массива по убыванию
template <typename T>
void InsertionSort::sort_descending(T* arr, std::size_t size) {
    if (arr != nullptr && size > 0) {
        sort_descending(arr, arr + size);
    }
}

#endif // INSERTION_SORT_CPP
