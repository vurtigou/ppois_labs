#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

/**
 * @brief Traverses all vertex labels of a mutable graph.
 * @tparam T Type of the vertex label.
 * @details Bidirectional, multi-pass traversal with read-only references.
 * Dereferencing an unbound or end iterator throws GraphException.
 */
template <typename T>
class VertexIterator {
    friend class Graph<T>;
    friend class ConstVertexIterator<T>;

    Graph<T>* graph_ = nullptr; //указатель на граф, к которому относится итератор
    const void* node_ = nullptr; //сырой указатель на внутренний узел графа

    VertexIterator(Graph<T>* graph, const void* node) noexcept
        : graph_(graph), node_(node) {}

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    VertexIterator() noexcept = default;
    /// Returns a reference to the current element.
    reference operator*() const;
    /// Returns a pointer to the current element.
    pointer operator->() const;
    /// Advances by one position.
    VertexIterator& operator++();
    /// Advances by one position.
    VertexIterator operator++(int);
    /// Moves back by one position.
    VertexIterator& operator--();
    /// Moves back by one position.
    VertexIterator operator--(int);
    /// Checks whether the two cursors identify the same position.
    bool operator==(const VertexIterator& other) const noexcept;
    /// Checks whether the two cursors identify different positions.
    bool operator!=(const VertexIterator& other) const noexcept;
};

#include "detail/VertexIterator.tpp"
