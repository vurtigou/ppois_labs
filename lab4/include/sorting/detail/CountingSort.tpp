#pragma once

#include "../CountingSort.h"

#include <cmath>
#include <type_traits>

namespace counting_sort_detail {

/// Selects the original value as the default key.
struct Identity {
    template <class T>
    const T& operator()(const T& value) const noexcept { return value; }
};

template <class Key>
std::size_t buildRanks(const std::vector<Key>& keys, std::vector<std::size_t>& ranks) {
    if constexpr (std::is_integral_v<Key>) {
        const auto [minimum, maximum] = std::minmax_element(keys.begin(), keys.end());
        const auto base = static_cast<std::uintmax_t>(*minimum);
        const auto span = static_cast<std::uintmax_t>(*maximum) - base;
        if (span < static_cast<std::uintmax_t>(CountingSort::MAX_ALLOWED_RANGE)) {
            for (const auto& key : keys) ranks.push_back(static_cast<std::uintmax_t>(key) - base);
            return static_cast<std::size_t>(span) + 1;
        }
    }

    // Compress ordered keys into contiguous ranks before the counting pass.
    auto unique = keys;
    const std::less<> less;
    std::sort(unique.begin(), unique.end(), less);
    unique.erase(std::unique(unique.begin(), unique.end(), [&less](const Key& a, const Key& b) {
        return !less(a, b) && !less(b, a);
    }), unique.end());

    for (const auto& key : keys) {
        const auto found = std::lower_bound(unique.begin(), unique.end(), key, less);
        ranks.push_back(static_cast<std::size_t>(found - unique.begin()));
    }
    return unique.size();
}

template <class Iterator, class KeyExtractor>
void sortByKey(Iterator first, Iterator last, KeyExtractor& extract, bool descending) {
    if (first == last) return;
    if (last < first) throw std::invalid_argument("CountingSort: reversed iterator range");

    using Value = typename std::iterator_traits<Iterator>::value_type;
    using Key = std::decay_t<decltype(std::invoke(extract, *first))>;
    // vector<bool> iterators expose a proxy. Materialize a real bool key for identity sorting.
    using StoredKey = std::conditional_t<std::is_same_v<KeyExtractor, Identity>, Value, Key>;
    const auto count = static_cast<std::size_t>(last - first);
    std::vector<StoredKey> keys;
    keys.reserve(count);
    for (auto current = first; current != last; ++current) {
        StoredKey key = std::invoke(extract, *current);
        if constexpr (std::is_floating_point_v<StoredKey>) {
            if (std::isnan(key)) throw std::invalid_argument("CountingSort: NaN key");
        }
        keys.push_back(std::move(key));
    }
    if (count == 1) return;

    std::vector<std::size_t> ranks;
    ranks.reserve(count);
    const auto keyCount = buildRanks(keys, ranks);
    if (descending) {
        for (auto& rank : ranks) rank = keyCount - 1 - rank;
    }
    std::vector<std::size_t> offsets(keyCount, 0);
    for (auto rank : ranks) ++offsets[rank];
    std::size_t offset = 0;
    for (auto& frequency : offsets) {
        const auto next = offset + frequency;
        frequency = offset;
        offset = next;
    }

    std::vector<Iterator> ordered(count, first);
    auto current = first;
    for (auto rank : ranks) ordered[offsets[rank]++] = current++;
    std::vector<Value> output;
    output.reserve(count);
    for (auto item : ordered) output.emplace_back(std::move(*item));
    for (auto&& value : output) *first++ = std::move(value);
}

} // namespace counting_sort_detail

template <typename RandomIt, typename KeyExtractor>
void CountingSort::sort(RandomIt first, RandomIt last, KeyExtractor key_extractor) {
    counting_sort_detail::sortByKey(first, last, key_extractor, false);
}

template <typename RandomIt>
void CountingSort::sort(RandomIt first, RandomIt last) {
    sort(first, last, counting_sort_detail::Identity{});
}

template <typename RandomIt, typename KeyExtractor>
void CountingSort::sort_descending(RandomIt first, RandomIt last, KeyExtractor key_extractor) {
    counting_sort_detail::sortByKey(first, last, key_extractor, true);
}

template <typename RandomIt>
void CountingSort::sort_descending(RandomIt first, RandomIt last) {
    sort_descending(first, last, counting_sort_detail::Identity{});
}

// Сортировка вектора с функцией извлечения ключа по возрастанию
template <typename T, typename KeyExtractor>
void CountingSort::sort(std::vector<T>& vec, KeyExtractor key_extractor) {
    sort(vec.begin(), vec.end(), key_extractor);
}

// Сортировка вектора по возрастанию
template <typename T>
void CountingSort::sort(std::vector<T>& vec) {
    sort(vec.begin(), vec.end());
}

// Сортировка вектора с функцией извлечения ключа по убыванию
template <typename T, typename KeyExtractor>
void CountingSort::sort_descending(std::vector<T>& vec, KeyExtractor key_extractor) {
    sort_descending(vec.begin(), vec.end(), key_extractor);
}

// Сортировка вектора по убыванию
template <typename T>
void CountingSort::sort_descending(std::vector<T>& vec) {
    sort_descending(vec.begin(), vec.end());
}

// Сортировка массива с функцией извлечения ключа по возрастанию
template <typename T, typename KeyExtractor>
void CountingSort::sort(T* arr, std::size_t size, KeyExtractor key_extractor) {
    if (arr != nullptr && size > 0) {
        sort(arr, arr + size, key_extractor);
    }
}

// Сортировка массива по возрастанию
template <typename T>
void CountingSort::sort(T* arr, std::size_t size) {
    if (arr != nullptr && size > 0) {
        sort(arr, arr + size);
    }
}

// Сортировка массива с функцией извлечения ключа по убыванию
template <typename T, typename KeyExtractor>
void CountingSort::sort_descending(T* arr, std::size_t size, KeyExtractor key_extractor) {
    if (arr != nullptr && size > 0) {
        sort_descending(arr, arr + size, key_extractor);
    }
}

// Сортировка массива по убыванию
template <typename T>
void CountingSort::sort_descending(T* arr, std::size_t size) {
    if (arr != nullptr && size > 0) {
        sort_descending(arr, arr + size);
    }
}

