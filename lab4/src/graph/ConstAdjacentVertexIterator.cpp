#ifndef CONSTADJACENTVERTEXITERATOR_CPP
#define CONSTADJACENTVERTEXITERATOR_CPP

#include "ConstAdjacentVertexIterator.h"
#include "AdjacentVertexIterator.h"
#include "GraphException.h"
#include "Graph.h"

template <typename T>
ConstAdjacentVertexIterator<T>::ConstAdjacentVertexIterator(const AdjacentVertexIterator<T>& other) noexcept
    : graph_(other.graph_), center_(other.center_), index_(other.index_) {}

template <typename T>
typename ConstAdjacentVertexIterator<T>::reference ConstAdjacentVertexIterator<T>::operator*() const {
    if (!graph_ || !center_ || index_ >= graph_->adjacentVertexCount(center_))
        throw GraphException("ConstAdjacentVertexIterator: dereferencing an end or invalid iterator");
    return graph_->adjacentVertexValue(center_, index_);
}

template <typename T>
typename ConstAdjacentVertexIterator<T>::pointer ConstAdjacentVertexIterator<T>::operator->() const {
    return std::addressof(operator*());
}

template <typename T>
ConstAdjacentVertexIterator<T>& ConstAdjacentVertexIterator<T>::operator++() {
    if (!graph_ || !center_ || index_ >= graph_->adjacentVertexCount(center_))
        throw GraphException("ConstAdjacentVertexIterator: incrementing an end or invalid iterator");
    ++index_;
    return *this;
}

template <typename T>
ConstAdjacentVertexIterator<T> ConstAdjacentVertexIterator<T>::operator++(int) {
    ConstAdjacentVertexIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
ConstAdjacentVertexIterator<T>& ConstAdjacentVertexIterator<T>::operator--() {
    if (!graph_ || !center_ || index_ == 0)
        throw GraphException("ConstAdjacentVertexIterator: decrementing before begin or an invalid iterator");
    --index_;
    return *this;
}

template <typename T>
ConstAdjacentVertexIterator<T> ConstAdjacentVertexIterator<T>::operator--(int) {
    ConstAdjacentVertexIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool ConstAdjacentVertexIterator<T>::operator==(const ConstAdjacentVertexIterator& other) const noexcept {
    return graph_ == other.graph_ && center_ == other.center_ && index_ == other.index_;
}

template <typename T>
bool ConstAdjacentVertexIterator<T>::operator!=(const ConstAdjacentVertexIterator& other) const noexcept {
    return !(*this == other);
}

#endif // CONSTADJACENTVERTEXITERATOR_CPP
