#pragma once

#include "GraphForward.h"
#include <cstddef>
#include <iterator>

//Двунаправленный итератор по вершинам неконстантного графа. методы возвращают const T&, чтобы пользователь не мог через итератор менять метку вершины (иначе нарушилась бы уникальность вершин).
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
    reference operator*() const;
    pointer operator->() const;
    VertexIterator& operator++();
    VertexIterator operator++(int);
    VertexIterator& operator--();
    VertexIterator operator--(int);
    bool operator==(const VertexIterator& other) const noexcept;
    bool operator!=(const VertexIterator& other) const noexcept;
    operator ConstVertexIterator<T>() const;
};

// Подключаем реализации
#include "VertexIterator.cpp"
