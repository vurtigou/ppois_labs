#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses outgoing neighbors of a vertex of a const graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
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
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    ConstAdjacentVertexIterator& operator++();
    /// Advances by one position.
    ConstAdjacentVertexIterator operator++(int);
    /// Moves back by one position.
    ConstAdjacentVertexIterator& operator--();
    /// Moves back by one position.
    ConstAdjacentVertexIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const ConstAdjacentVertexIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const ConstAdjacentVertexIterator& other) const noexcept;
};

#include "detail/ConstAdjacentVertexIterator.tpp"
