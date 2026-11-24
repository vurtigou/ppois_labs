#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

// Bidirectional iterator over outgoing neighbours (successors) of one vertex.
template <typename T>
class AdjacentVertexIterator {
    friend class Graph<T>;
    friend class ConstAdjacentVertexIterator<T>;

    Graph<T>* graph_ = nullptr;
    const void* center_ = nullptr;
    std::size_t index_ = 0;

    AdjacentVertexIterator(Graph<T>* graph, const void* center, std::size_t index) noexcept
        : graph_(graph), center_(center), index_(index) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    AdjacentVertexIterator() noexcept = default;
    reference operator*() const;
    pointer operator->() const;
    AdjacentVertexIterator& operator++();
    AdjacentVertexIterator operator++(int);
    AdjacentVertexIterator& operator--();
    AdjacentVertexIterator operator--(int);
    bool operator==(const AdjacentVertexIterator& other) const noexcept;
    bool operator!=(const AdjacentVertexIterator& other) const noexcept;
    operator ConstAdjacentVertexIterator<T>() const;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "AdjacentVertexIterator.cpp"
