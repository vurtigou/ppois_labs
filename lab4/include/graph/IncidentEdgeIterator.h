#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>
#include <optional>

// Iterates over every edge touching a selected vertex: outgoing edges first,
// then incoming edges. A loop at the selected vertex is yielded only once.
template <typename T>
class IncidentEdgeIterator {
    friend class Graph<T>;
    friend class ConstIncidentEdgeIterator<T>;

    Graph<T>* graph_ = nullptr;
    const void* center_ = nullptr;
    std::size_t index_ = 0;
    mutable std::optional<Edge<T>> cache_;

    IncidentEdgeIterator(Graph<T>* graph, const void* center, std::size_t index) noexcept
        : graph_(graph), center_(center), index_(index) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = Edge<T>;

    IncidentEdgeIterator() noexcept = default;
    reference operator*() const;
    pointer operator->() const;
    IncidentEdgeIterator& operator++();
    IncidentEdgeIterator operator++(int);
    IncidentEdgeIterator& operator--();
    IncidentEdgeIterator operator--(int);
    bool operator==(const IncidentEdgeIterator& other) const noexcept;
    bool operator!=(const IncidentEdgeIterator& other) const noexcept;
    operator ConstIncidentEdgeIterator<T>() const;
};

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "IncidentEdgeIterator.cpp"
