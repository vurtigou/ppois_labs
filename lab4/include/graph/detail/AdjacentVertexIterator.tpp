#pragma once

#include "../AdjacentVertexIterator.h"
#include "../ConstAdjacentVertexIterator.h"
#include "../GraphException.h"
#include "../Graph.h"

template <typename T>
typename AdjacentVertexIterator<T>::reference AdjacentVertexIterator<T>::operator*() const {
    if (!graph_ || !center_ || index_ >= graph_->adjacentVertexCount(center_))
        throw GraphException("AdjacentVertexIterator: dereferencing an end or invalid iterator");
    return graph_->adjacentVertexValue(center_, index_);
}

template <typename T>
typename AdjacentVertexIterator<T>::pointer AdjacentVertexIterator<T>::operator->() const {
    return std::addressof(operator*());
}

template <typename T>
AdjacentVertexIterator<T>& AdjacentVertexIterator<T>::operator++() {
    if (!graph_ || !center_ || index_ >= graph_->adjacentVertexCount(center_))
        throw GraphException("AdjacentVertexIterator: incrementing an end or invalid iterator");
    ++index_;
    return *this;
}

template <typename T>
AdjacentVertexIterator<T> AdjacentVertexIterator<T>::operator++(int) {
    AdjacentVertexIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
AdjacentVertexIterator<T>& AdjacentVertexIterator<T>::operator--() {
    if (!graph_ || !center_ || index_ == 0)
        throw GraphException("AdjacentVertexIterator: decrementing before begin or an invalid iterator");
    --index_;
    return *this;
}

template <typename T>
AdjacentVertexIterator<T> AdjacentVertexIterator<T>::operator--(int) {
    AdjacentVertexIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool AdjacentVertexIterator<T>::operator==(const AdjacentVertexIterator& other) const noexcept {
    return graph_ == other.graph_ && center_ == other.center_ && index_ == other.index_;
}

template <typename T>
bool AdjacentVertexIterator<T>::operator!=(const AdjacentVertexIterator& other) const noexcept {
    return !(*this == other);
}


