#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

// Read-only bidirectional iterator over vertices.
template <typename T>
class ConstVertexIterator {
    friend class Graph<T>;
    friend class VertexIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* node_ = nullptr;

    ConstVertexIterator(const Graph<T>* graph, const void* node) noexcept
        : graph_(graph), node_(node) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstVertexIterator() noexcept = default;
    ConstVertexIterator(const VertexIterator<T>& other) noexcept;
    reference operator*() const;
    pointer operator->() const;
    ConstVertexIterator& operator++();
    ConstVertexIterator operator++(int);
    ConstVertexIterator& operator--();
    ConstVertexIterator operator--(int);
    bool operator==(const ConstVertexIterator& other) const noexcept;
    bool operator!=(const ConstVertexIterator& other) const noexcept;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "ConstVertexIterator.cpp"
