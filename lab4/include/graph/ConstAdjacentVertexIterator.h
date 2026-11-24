#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

// Read-only bidirectional iterator over outgoing neighbours of one vertex.
template <typename T>
class ConstAdjacentVertexIterator {
    friend class Graph<T>;
    friend class AdjacentVertexIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* center_ = nullptr;
    std::size_t index_ = 0;

    ConstAdjacentVertexIterator(const Graph<T>* graph, const void* center, std::size_t index) noexcept
        : graph_(graph), center_(center), index_(index) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstAdjacentVertexIterator() noexcept = default;
    ConstAdjacentVertexIterator(const AdjacentVertexIterator<T>& other) noexcept;
    reference operator*() const;
    pointer operator->() const;
    ConstAdjacentVertexIterator& operator++();
    ConstAdjacentVertexIterator operator++(int);
    ConstAdjacentVertexIterator& operator--();
    ConstAdjacentVertexIterator operator--(int);
    bool operator==(const ConstAdjacentVertexIterator& other) const noexcept;
    bool operator!=(const ConstAdjacentVertexIterator& other) const noexcept;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "ConstAdjacentVertexIterator.cpp"
