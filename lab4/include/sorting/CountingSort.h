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
 * @brief Обобщённый класс сортировки подсчётом (Counting sort).
 * Эффективен при небольшом диапазоне ключей. Поддерживает как целочисленные типы,
 * так и произвольные пользовательские объекты через функцию извлечения числового ключа.
 * Предоставляет полную симметрию: сортировка по возрастанию и по убыванию.
 */
class CountingSort {
public:
    /// Максимально допустимый диапазон значений ключей во избежание чрезмерного расхода памяти
    static constexpr std::int64_t MAX_ALLOWED_RANGE = 50000000LL;

    /**
     * @brief Сортировка диапазона [first, last) по возрастанию с заданной функцией извлечения ключа.
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @tparam KeyExtractor Тип функции, извлекающей целочисленный ключ из элемента.
     * @param first Начало диапазона.
     * @param last Конец диапазона.
     * @param key_extractor Функция извлечения целочисленного ключа.
     */
    template <typename RandomIt, typename KeyExtractor>
    static void sort(RandomIt first, RandomIt last, KeyExtractor key_extractor);

    /**
     * @brief Сортировка диапазона [first, last) по возрастанию (для целочисленных типов).
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @param first Начало диапазона.
     * @param last Конец диапазона.
     */
    template <typename RandomIt>
    static void sort(RandomIt first, RandomIt last);

    /**
     * @brief Сортировка диапазона [first, last) по убыванию с заданной функцией извлечения ключа.
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @tparam KeyExtractor Тип функции, извлекающей целочисленный ключ из элемента.
     * @param first Начало диапазона.
     * @param last Конец диапазона.
     * @param key_extractor Функция извлечения ключа.
     */
    template <typename RandomIt, typename KeyExtractor>
    static void sort_descending(RandomIt first, RandomIt last, KeyExtractor key_extractor);

    /**
     * @brief Сортировка диапазона [first, last) по убыванию (для целочисленных типов).
     * @tparam RandomIt Тип итератора произвольного доступа.
     * @param first Начало диапазона.
     * @param last Конец диапазона.
     */
    template <typename RandomIt>
    static void sort_descending(RandomIt first, RandomIt last);

    /**
     * @brief Сортировка вектора std::vector по возрастанию с функцией извлечения ключа.
     * @tparam T Тип элементов вектора.
     * @tparam KeyExtractor Тип функции извлечения ключа.
     * @param vec Ссылка на сортируемый вектор.
     * @param key_extractor Функция извлечения ключа.
     */
    template <typename T, typename KeyExtractor>
    static void sort(std::vector<T>& vec, KeyExtractor key_extractor);

    /**
     * @brief Сортировка вектора std::vector по возрастанию.
     * @tparam T Тип элементов вектора.
     * @param vec Ссылка на сортируемый вектор.
     */
    template <typename T>
    static void sort(std::vector<T>& vec);

    /**
     * @brief Сортировка вектора std::vector по убыванию с функцией извлечения ключа.
     * @tparam T Тип элементов вектора.
     * @tparam KeyExtractor Тип функции извлечения ключа.
     * @param vec Ссылка на сортируемый вектор.
     * @param key_extractor Функция извлечения ключа.
     */
    template <typename T, typename KeyExtractor>
    static void sort_descending(std::vector<T>& vec, KeyExtractor key_extractor);

    /**
     * @brief Сортировка вектора std::vector по убыванию.
     * @tparam T Тип элементов вектора.
     * @param vec Ссылка на сортируемый вектор.
     */
    template <typename T>
    static void sort_descending(std::vector<T>& vec);

    /**
     * @brief Сортировка массива по возрастанию с функцией извлечения ключа.
     * @tparam T Тип элементов массива.
     * @tparam KeyExtractor Тип функции извлечения ключа.
     * @param arr Указатель на начало массива.
     * @param size Размер массива.
     * @param key_extractor Функция извлечения ключа.
     */
    template <typename T, typename KeyExtractor>
    static void sort(T* arr, std::size_t size, KeyExtractor key_extractor);

    /**
     * @brief Сортировка массива по возрастанию.
     * @tparam T Тип элементов массива.
     * @param arr Указатель на начало массива.
     * @param size Размер массива.
     */
    template <typename T>
    static void sort(T* arr, std::size_t size);

    /**
     * @brief Сортировка массива по убыванию с функцией извлечения ключа.
     * @tparam T Тип элементов массива.
     * @tparam KeyExtractor Тип функции извлечения ключа.
     * @param arr Указатель на начало массива.
     * @param size Размер массива.
     * @param key_extractor Функция извлечения ключа.
     */
    template <typename T, typename KeyExtractor>
    static void sort_descending(T* arr, std::size_t size, KeyExtractor key_extractor);

    /**
     * @brief Сортировка массива по убыванию.
     * @tparam T Тип элементов массива.
     * @param arr Указатель на начало массива.
     * @param size Размер массива.
     */
    template <typename T>
    static void sort_descending(T* arr, std::size_t size);
};

#include "CountingSort.cpp"
