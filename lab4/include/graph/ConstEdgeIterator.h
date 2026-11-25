#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses all directed edges of a const graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
template <typename T>
class ConstEdgeIterator {
    friend class Graph<T>;
    friend class EdgeIterator<T>;

    const Graph<T>* graph_ = nullptr;
    const void* source_ = nullptr;
    const void* edgeCell_ = nullptr;

    ConstEdgeIterator(const Graph<T>* graph, const void* source, const void* edgeCell);

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = const Edge<T>&;

    ConstEdgeIterator() noexcept = default;
    ConstEdgeIterator(const EdgeIterator<T>& other);
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    ConstEdgeIterator& operator++();
    /// Advances by one position.
    ConstEdgeIterator operator++(int);
    /// Moves back by one position.
    ConstEdgeIterator& operator--();
    /// Moves back by one position.
    ConstEdgeIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const ConstEdgeIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const ConstEdgeIterator& other) const noexcept;
};

#include "detail/ConstEdgeIterator.tpp"
