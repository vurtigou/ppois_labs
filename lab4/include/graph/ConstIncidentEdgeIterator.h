#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses all arcs incident to a vertex of a const graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
template <typename T>
class ConstIncidentEdgeIterator {
    friend class Graph<T>;
    friend class IncidentEdgeIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* center_ = nullptr;
    std::size_t index_ = 0;

    ConstIncidentEdgeIterator(const Graph<T>* graph, const void* center, std::size_t index) noexcept
        : graph_(graph), center_(center), index_(index) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = const Edge<T>&;

    ConstIncidentEdgeIterator() noexcept = default;
    ConstIncidentEdgeIterator(const IncidentEdgeIterator<T>& other) noexcept;
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    ConstIncidentEdgeIterator& operator++();
    /// Advances by one position.
    ConstIncidentEdgeIterator operator++(int);
    /// Moves back by one position.
    ConstIncidentEdgeIterator& operator--();
    /// Moves back by one position.
    ConstIncidentEdgeIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const ConstIncidentEdgeIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const ConstIncidentEdgeIterator& other) const noexcept;
};

#include "detail/ConstIncidentEdgeIterator.tpp"
