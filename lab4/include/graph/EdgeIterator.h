#pragma once

#include "GraphForward.h"
#include "Edge.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses all directed edges of a mutable graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
template <typename T>
class EdgeIterator {
    friend class Graph<T>;
    friend class ConstEdgeIterator<T>;

    Graph<T>* graph_ = nullptr;
    const void* source_ = nullptr;
    const void* edgeCell_ = nullptr;

    EdgeIterator(Graph<T>* graph, const void* source, const void* edgeCell);

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Edge<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const Edge<T>*;
    using reference = const Edge<T>&;

    EdgeIterator() noexcept = default;
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    EdgeIterator& operator++();
    /// Advances by one position.
    EdgeIterator operator++(int);
    /// Moves back by one position.
    EdgeIterator& operator--();
    /// Moves back by one position.
    EdgeIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const EdgeIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const EdgeIterator& other) const noexcept;
};

#include "detail/EdgeIterator.tpp"
