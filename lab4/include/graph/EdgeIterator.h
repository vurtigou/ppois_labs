#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>
#include <optional>

// Bidirectional iterator over all directed edges in the graph.
template <typename T>
class EdgeIterator {
    friend class Graph<T>;
    friend class ConstEdgeIterator<T>;

    Graph<T>* graph_ = nullptr;
    const void* source_ = nullptr;
    const void* edgeCell_ = nullptr;
    mutable std::optional<Edge<T>> cache_;

    EdgeIterator(Graph<T>* graph, const void* source, const void* edgeCell);

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = Edge<T>;

    EdgeIterator() noexcept = default;
    reference operator*() const;
    pointer operator->() const;
    EdgeIterator& operator++();
    EdgeIterator operator++(int);
    EdgeIterator& operator--();
    EdgeIterator operator--(int);
    bool operator==(const EdgeIterator& other) const noexcept;
    bool operator!=(const EdgeIterator& other) const noexcept;
    operator ConstEdgeIterator<T>() const;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "EdgeIterator.cpp"
