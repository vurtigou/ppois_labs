#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iterator>
#include <cstdint>
#include <algorithm>
#include <utility>

/**
 * @brief Stable counting sort for random-access ranges, vectors, and C arrays.
 * @details Dense integral keys use direct counting. Sparse or non-integral keys use ordered rank compression.
 */
class CountingSort {
public:
    /// Maximum number of direct counting buckets before rank compression.
    static constexpr std::int64_t MAX_ALLOWED_RANGE = 1000000LL;

    /// Sorts the supplied range in ascending order using the projected key.
    template <typename RandomIt, typename KeyExtractor>
    static void sort(RandomIt first, RandomIt last, KeyExtractor key_extractor);

    /// Sorts the supplied range in ascending order.
    template <typename RandomIt>
    static void sort(RandomIt first, RandomIt last);

    /// Sorts the supplied range in descending order using the projected key.
    template <typename RandomIt, typename KeyExtractor>
    static void sort_descending(RandomIt first, RandomIt last, KeyExtractor key_extractor);

    /// Sorts the supplied range in descending order.
    template <typename RandomIt>
    static void sort_descending(RandomIt first, RandomIt last);

    /// Sorts the supplied vector in ascending order using the projected key.
    template <typename T, typename KeyExtractor>
    static void sort(std::vector<T>& vec, KeyExtractor key_extractor);

    /// Sorts the supplied vector in ascending order.
    template <typename T>
    static void sort(std::vector<T>& vec);

    /// Sorts the supplied vector in descending order using the projected key.
    template <typename T, typename KeyExtractor>
    static void sort_descending(std::vector<T>& vec, KeyExtractor key_extractor);

    /// Sorts the supplied vector in descending order.
    template <typename T>
    static void sort_descending(std::vector<T>& vec);

    /// Sorts the first size elements of the supplied C array in ascending order using the projected key.
    template <typename T, typename KeyExtractor>
    static void sort(T* arr, std::size_t size, KeyExtractor key_extractor);

    /// Sorts the first size elements of the supplied C array in ascending order.
    template <typename T>
    static void sort(T* arr, std::size_t size);

    /// Sorts the first size elements of the supplied C array in descending order using the projected key.
    template <typename T, typename KeyExtractor>
    static void sort_descending(T* arr, std::size_t size, KeyExtractor key_extractor);

    /// Sorts the first size elements of the supplied C array in descending order.
    template <typename T>
    static void sort_descending(T* arr, std::size_t size);
};

#include "detail/CountingSort.tpp"
