#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <algorithm>

/**
 * @brief Stable insertion sort for random-access ranges, vectors, and C arrays.
 * @details The supplied comparator must impose a strict weak ordering.
 */
class InsertionSort {
public:

    /// Sorts the supplied range in ascending order using the supplied comparator.
    template <typename RandomIt, typename Compare>
    static void sort(RandomIt first, RandomIt last, Compare comp);

    /// Sorts the supplied range in ascending order.
    template <typename RandomIt>
    static void sort(RandomIt first, RandomIt last);

    /// Sorts the supplied range in descending order using the supplied comparator.
    template <typename RandomIt, typename Compare>
    static void sort_descending(RandomIt first, RandomIt last, Compare comp);

    /// Sorts the supplied range in descending order.
    template <typename RandomIt>
    static void sort_descending(RandomIt first, RandomIt last);

    /// Sorts the supplied vector in ascending order using the supplied comparator.
    template <typename T, typename Compare>
    static void sort(std::vector<T>& vec, Compare comp);

    /// Sorts the supplied vector in ascending order.
    template <typename T>
    static void sort(std::vector<T>& vec);

    /// Sorts the supplied vector in descending order using the supplied comparator.
    template <typename T, typename Compare>
    static void sort_descending(std::vector<T>& vec, Compare comp);

    /// Sorts the supplied vector in descending order.
    template <typename T>
    static void sort_descending(std::vector<T>& vec);

    /// Sorts the first size elements of the supplied C array in ascending order using the supplied comparator.
    template <typename T, typename Compare>
    static void sort(T* arr, std::size_t size, Compare comp);

    /// Sorts the first size elements of the supplied C array in ascending order.
    template <typename T>
    static void sort(T* arr, std::size_t size);

    /// Sorts the first size elements of the supplied C array in descending order using the supplied comparator.
    template <typename T, typename Compare>
    static void sort_descending(T* arr, std::size_t size, Compare comp);

    /// Sorts the first size elements of the supplied C array in descending order.
    template <typename T>
    static void sort_descending(T* arr, std::size_t size);
};

// Шаблонные реализации подключаются в конце заголовочного файла
#include "detail/InsertionSort.tpp"
