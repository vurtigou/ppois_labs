#pragma once

#include "../ConstVertexIterator.h"
#include "../GraphException.h"
#include "../Graph.h"
#include "../VertexIterator.h"

template <typename T>
ConstVertexIterator<T>::ConstVertexIterator(const VertexIterator<T>& other) noexcept
    : graph_(other.graph_), node_(other.node_) {}

template <typename T>
typename ConstVertexIterator<T>::reference ConstVertexIterator<T>::operator*() const {
    if (!graph_ || !node_) throw GraphException("ConstVertexIterator: dereferencing an end or invalid iterator");
    return graph_->vertexValue(node_);
}

template <typename T>
typename ConstVertexIterator<T>::pointer ConstVertexIterator<T>::operator->() const {
    return std::addressof(operator*());
}

template <typename T>
ConstVertexIterator<T>& ConstVertexIterator<T>::operator++() {
    if (!graph_ || !node_) throw GraphException("ConstVertexIterator: incrementing an end or invalid iterator");
    node_ = graph_->nextVertexHandle(node_);
    return *this;
}

template <typename T>
ConstVertexIterator<T> ConstVertexIterator<T>::operator++(int) {
    ConstVertexIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
ConstVertexIterator<T>& ConstVertexIterator<T>::operator--() {
    if (!graph_) throw GraphException("ConstVertexIterator: decrementing an unbound iterator");
    node_ = graph_->previousVertexHandle(node_);
    return *this;
}

template <typename T>
ConstVertexIterator<T> ConstVertexIterator<T>::operator--(int) {
    ConstVertexIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool ConstVertexIterator<T>::operator==(const ConstVertexIterator& other) const noexcept {
    return graph_ == other.graph_ && node_ == other.node_;
}

template <typename T>
bool ConstVertexIterator<T>::operator!=(const ConstVertexIterator& other) const noexcept {
    return !(*this == other);
}

