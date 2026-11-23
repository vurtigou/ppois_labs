#ifndef VERTEXITERATOR_CPP
#define VERTEXITERATOR_CPP

#include "VertexIterator.h"
#include "GraphException.h"
#include "Graph.h"

template <typename T>
typename VertexIterator<T>::reference VertexIterator<T>::operator*() const {
    if (!graph_ || !node_) throw GraphException("VertexIterator: dereferencing an end or invalid iterator");
    return graph_->vertexValue(node_);
}

template <typename T>
typename VertexIterator<T>::pointer VertexIterator<T>::operator->() const {
    return std::addressof(operator*());
}

template <typename T>
VertexIterator<T>& VertexIterator<T>::operator++() {
    if (!graph_ || !node_) throw GraphException("VertexIterator: incrementing an end or invalid iterator");
    node_ = graph_->nextVertexHandle(node_);
    return *this;
}

template <typename T>
VertexIterator<T> VertexIterator<T>::operator++(int) {
    VertexIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
VertexIterator<T>& VertexIterator<T>::operator--() {
    if (!graph_) throw GraphException("VertexIterator: decrementing an unbound iterator");
    node_ = graph_->previousVertexHandle(node_);
    return *this;
}

template <typename T>
VertexIterator<T> VertexIterator<T>::operator--(int) {
    VertexIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool VertexIterator<T>::operator==(const VertexIterator& other) const noexcept {
    return graph_ == other.graph_ && node_ == other.node_;
}

template <typename T>
bool VertexIterator<T>::operator!=(const VertexIterator& other) const noexcept {
    return !(*this == other);
}

template <typename T>
VertexIterator<T>::operator ConstVertexIterator<T>() const {
    return ConstVertexIterator<T>(graph_, node_);
}

#endif // VERTEXITERATOR_CPP
