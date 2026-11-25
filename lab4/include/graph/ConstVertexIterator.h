#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses all vertex labels of a const graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
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
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    ConstVertexIterator& operator++();
    /// Advances by one position.
    ConstVertexIterator operator++(int);
    /// Moves back by one position.
    ConstVertexIterator& operator--();
    /// Moves back by one position.
    ConstVertexIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const ConstVertexIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const ConstVertexIterator& other) const noexcept;
};

#include "detail/ConstVertexIterator.tpp"
