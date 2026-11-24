#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>
#include <optional>

// Read-only iterator over the incoming and outgoing edges of one vertex.
template <typename T>
class ConstIncidentEdgeIterator {
    friend class Graph<T>;
    friend class IncidentEdgeIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* center_ = nullptr;
    std::size_t index_ = 0;
    mutable std::optional<Edge<T>> cache_;

    ConstIncidentEdgeIterator(const Graph<T>* graph, const void* center, std::size_t index) noexcept
        : graph_(graph), center_(center), index_(index) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = Edge<T>;

    ConstIncidentEdgeIterator() noexcept = default;
    ConstIncidentEdgeIterator(const IncidentEdgeIterator<T>& other) noexcept;
    reference operator*() const;
    pointer operator->() const;
    ConstIncidentEdgeIterator& operator++();
    ConstIncidentEdgeIterator operator++(int);
    ConstIncidentEdgeIterator& operator--();
    ConstIncidentEdgeIterator operator--(int);
    bool operator==(const ConstIncidentEdgeIterator& other) const noexcept;
    bool operator!=(const ConstIncidentEdgeIterator& other) const noexcept;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "ConstIncidentEdgeIterator.cpp"
