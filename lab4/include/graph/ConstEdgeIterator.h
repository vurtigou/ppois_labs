#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>
#include <optional>

// Read-only bidirectional iterator over all directed edges in the graph.
template <typename T>
class ConstEdgeIterator {
    friend class Graph<T>;
    friend class EdgeIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* source_ = nullptr;
    const void* edgeCell_ = nullptr;
    mutable std::optional<Edge<T>> cache_;

    ConstEdgeIterator(const Graph<T>* graph, const void* source, const void* edgeCell);

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = Edge<T>;

    ConstEdgeIterator() noexcept = default;
    ConstEdgeIterator(const EdgeIterator<T>& other);
    reference operator*() const;
    pointer operator->() const;
    ConstEdgeIterator& operator++();
    ConstEdgeIterator operator++(int);
    ConstEdgeIterator& operator--();
    ConstEdgeIterator operator--(int);
    bool operator==(const ConstEdgeIterator& other) const noexcept;
    bool operator!=(const ConstEdgeIterator& other) const noexcept;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "ConstEdgeIterator.cpp"
