#ifndef EDGEITERATOR_CPP
#define EDGEITERATOR_CPP

#include "EdgeIterator.h"
#include "ConstEdgeIterator.h"
#include "GraphException.h"
#include "Graph.h"

template <typename T>
EdgeIterator<T>::EdgeIterator(Graph<T>* graph, const void* source, const void* edgeCell)
    : graph_(graph), source_(source), edgeCell_(edgeCell) {}

template <typename T>
typename EdgeIterator<T>::reference EdgeIterator<T>::operator*() const {
    if (!graph_ || !source_ || !edgeCell_) throw GraphException("EdgeIterator: dereferencing an end or invalid iterator");
    return graph_->edgeValue(source_, edgeCell_);
}

template <typename T>
typename EdgeIterator<T>::pointer EdgeIterator<T>::operator->() const {
    cache_.emplace(operator*());
    return std::addressof(*cache_);
}

template <typename T>
EdgeIterator<T>& EdgeIterator<T>::operator++() {
    if (!graph_ || !source_ || !edgeCell_) throw GraphException("EdgeIterator: incrementing an end or invalid iterator");
    const void* nextSource = nullptr;
    const void* nextCell = nullptr;
    graph_->nextEdgeHandle(source_, edgeCell_, nextSource, nextCell);
    source_ = nextSource;
    edgeCell_ = nextCell;
    cache_.reset();
    return *this;
}

template <typename T>
EdgeIterator<T> EdgeIterator<T>::operator++(int) {
    EdgeIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
EdgeIterator<T>& EdgeIterator<T>::operator--() {
    if (!graph_) throw GraphException("EdgeIterator: decrementing an unbound iterator");
    const void* previousSource = nullptr;
    const void* previousCell = nullptr;
    graph_->previousEdgeHandle(source_, edgeCell_, previousSource, previousCell);
    source_ = previousSource;
    edgeCell_ = previousCell;
    cache_.reset();
    return *this;
}

template <typename T>
EdgeIterator<T> EdgeIterator<T>::operator--(int) {
    EdgeIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool EdgeIterator<T>::operator==(const EdgeIterator& other) const noexcept {
    return graph_ == other.graph_ && source_ == other.source_ && edgeCell_ == other.edgeCell_;
}

template <typename T>
bool EdgeIterator<T>::operator!=(const EdgeIterator& other) const noexcept {
    return !(*this == other);
}

template <typename T>
EdgeIterator<T>::operator ConstEdgeIterator<T>() const {
    return ConstEdgeIterator<T>(graph_, source_, edgeCell_);
}

#endif // EDGEITERATOR_CPP
