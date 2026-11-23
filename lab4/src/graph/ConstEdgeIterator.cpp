#ifndef CONSTEDGEITERATOR_CPP
#define CONSTEDGEITERATOR_CPP

#include "ConstEdgeIterator.h"
#include "EdgeIterator.h"
#include "GraphException.h"
#include "Graph.h"

template <typename T>
ConstEdgeIterator<T>::ConstEdgeIterator(const Graph<T>* graph, const void* source, const void* edgeCell)
    : graph_(graph), source_(source), edgeCell_(edgeCell) {}

template <typename T>
ConstEdgeIterator<T>::ConstEdgeIterator(const EdgeIterator<T>& other)
    : graph_(other.graph_), source_(other.source_), edgeCell_(other.edgeCell_) {}

template <typename T>
typename ConstEdgeIterator<T>::reference ConstEdgeIterator<T>::operator*() const {
    if (!graph_ || !source_ || !edgeCell_) throw GraphException("ConstEdgeIterator: dereferencing an end or invalid iterator");
    return graph_->edgeValue(source_, edgeCell_);
}

template <typename T>
typename ConstEdgeIterator<T>::pointer ConstEdgeIterator<T>::operator->() const {
    cache_.emplace(operator*());
    return std::addressof(*cache_);
}

template <typename T>
ConstEdgeIterator<T>& ConstEdgeIterator<T>::operator++() {
    if (!graph_ || !source_ || !edgeCell_) throw GraphException("ConstEdgeIterator: incrementing an end or invalid iterator");
    const void* nextSource = nullptr;
    const void* nextCell = nullptr;
    graph_->nextEdgeHandle(source_, edgeCell_, nextSource, nextCell);
    source_ = nextSource;
    edgeCell_ = nextCell;
    cache_.reset();
    return *this;
}

template <typename T>
ConstEdgeIterator<T> ConstEdgeIterator<T>::operator++(int) {
    ConstEdgeIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
ConstEdgeIterator<T>& ConstEdgeIterator<T>::operator--() {
    if (!graph_) throw GraphException("ConstEdgeIterator: decrementing an unbound iterator");
    const void* previousSource = nullptr;
    const void* previousCell = nullptr;
    graph_->previousEdgeHandle(source_, edgeCell_, previousSource, previousCell);
    source_ = previousSource;
    edgeCell_ = previousCell;
    cache_.reset();
    return *this;
}

template <typename T>
ConstEdgeIterator<T> ConstEdgeIterator<T>::operator--(int) {
    ConstEdgeIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool ConstEdgeIterator<T>::operator==(const ConstEdgeIterator& other) const noexcept {
    return graph_ == other.graph_ && source_ == other.source_ && edgeCell_ == other.edgeCell_;
}

template <typename T>
bool ConstEdgeIterator<T>::operator!=(const ConstEdgeIterator& other) const noexcept {
    return !(*this == other);
}

#endif // CONSTEDGEITERATOR_CPP
