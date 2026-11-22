#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <algorithm>

/**
 * @brief Обобщённый класс сортировки вставками (Insertion sort).
 * Эффективен для небольших массивов или частично отсортированных данных.
 * Поддерживает как встроенные, так и пользовательские типы через компараторы.
 * Предоставляет полную симметрию: сортировка по возрастанию и по убыванию.
 */
class InsertionSort {
public:
    /**
     * @brief Сортировка диапазона [first, last) по возрастанию с заданным компаратором.
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @tparam Compare Тип функции сравнения (например, std::less<> или лямбда).
     * @param first Начало диапазона.
     * @param last Конец диапазона.
     * @param comp Функция сравнения (возвращает true, если первый аргумент должен идти раньше второго).
     */
    template <typename RandomIt, typename Compare>
    static void sort(RandomIt first, RandomIt last, Compare comp);

    /**
     * @brief Сортировка диапазона [first, last) по возрастанию (по умолчанию std::less).
     * @tparam RandomIt Тип итератора произвольного доступа.
     */
    template <typename RandomIt>
    static void sort(RandomIt first, RandomIt last);

    /**
     * @brief Сортировка диапазона [first, last) по убыванию с заданным компаратором.
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @tparam Compare Тип функции сравнения.
     */
    template <typename RandomIt, typename Compare>
    static void sort_descending(RandomIt first, RandomIt last, Compare comp);

    /**
     * @brief Сортировка диапазона [first, last) по убыванию (по умолчанию std::greater).
     * @tparam RandomIt Тип итератора произвольного доступа.
     */
    template <typename RandomIt>
    static void sort_descending(RandomIt first, RandomIt last);

    /**
     * @brief Сортировка вектора std::vector по возрастанию с компаратором.
     */
    template <typename T, typename Compare>
    static void sort(std::vector<T>& vec, Compare comp);

    /**
     * @brief Сортировка вектора std::vector по возрастанию.
     */
    template <typename T>
    static void sort(std::vector<T>& vec);

    /**
     * @brief Сортировка вектора std::vector по убыванию с компаратором.
     */
    template <typename T, typename Compare>
    static void sort_descending(std::vector<T>& vec, Compare comp);

    /**
     * @brief Сортировка вектора std::vector по убыванию.
     */
    template <typename T>
    static void sort_descending(std::vector<T>& vec);

    /**
     * @brief Сортировка C-массива по возрастанию с компаратором.
     */
    template <typename T, typename Compare>
    static void sort(T* arr, std::size_t size, Compare comp);

    /**
     * @brief Сортировка C-массива по возрастанию.
     */
    template <typename T>
    static void sort(T* arr, std::size_t size);

    /**
     * @brief Сортировка C-массива по убыванию с компаратором.
     */
    template <typename T, typename Compare>
    static void sort_descending(T* arr, std::size_t size, Compare comp);

    /**
     * @brief Сортировка C-массива по убыванию.
     */
    template <typename T>
    static void sort_descending(T* arr, std::size_t size);
};

// Шаблонные реализации подключаются в конце заголовочного файла
#include "InsertionSort.cpp"
