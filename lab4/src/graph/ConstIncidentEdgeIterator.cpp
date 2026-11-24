#ifndef CONSTINCIDENTEDGEITERATOR_CPP
#define CONSTINCIDENTEDGEITERATOR_CPP

#include "ConstIncidentEdgeIterator.h"
#include "IncidentEdgeIterator.h"
#include "GraphException.h"
#include "Graph.h"

template <typename T>
ConstIncidentEdgeIterator<T>::ConstIncidentEdgeIterator(const IncidentEdgeIterator<T>& other) noexcept
    : graph_(other.graph_), center_(other.center_), index_(other.index_) {}

template <typename T>
typename ConstIncidentEdgeIterator<T>::reference ConstIncidentEdgeIterator<T>::operator*() const {
    if (!graph_ || !center_ || index_ >= graph_->incidentEdgeCount(center_))
        throw GraphException("ConstIncidentEdgeIterator: dereferencing an end or invalid iterator");
    return graph_->incidentEdgeValue(center_, index_);
}

template <typename T>
typename ConstIncidentEdgeIterator<T>::pointer ConstIncidentEdgeIterator<T>::operator->() const {
    cache_.emplace(operator*());
    return std::addressof(*cache_);
}

template <typename T>
ConstIncidentEdgeIterator<T>& ConstIncidentEdgeIterator<T>::operator++() {
    if (!graph_ || !center_ || index_ >= graph_->incidentEdgeCount(center_))
        throw GraphException("ConstIncidentEdgeIterator: incrementing an end or invalid iterator");
    ++index_;
    cache_.reset();
    return *this;
}

template <typename T>
ConstIncidentEdgeIterator<T> ConstIncidentEdgeIterator<T>::operator++(int) {
    ConstIncidentEdgeIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
ConstIncidentEdgeIterator<T>& ConstIncidentEdgeIterator<T>::operator--() {
    if (!graph_ || !center_ || index_ == 0)
        throw GraphException("ConstIncidentEdgeIterator: decrementing before begin or an invalid iterator");
    --index_;
    cache_.reset();
    return *this;
}

template <typename T>
ConstIncidentEdgeIterator<T> ConstIncidentEdgeIterator<T>::operator--(int) {
    ConstIncidentEdgeIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool ConstIncidentEdgeIterator<T>::operator==(const ConstIncidentEdgeIterator& other) const noexcept {
    return graph_ == other.graph_ && center_ == other.center_ && index_ == other.index_;
}

template <typename T>
bool ConstIncidentEdgeIterator<T>::operator!=(const ConstIncidentEdgeIterator& other) const noexcept {
    return !(*this == other);
}

#endif // CONSTINCIDENTEDGEITERATOR_CPP
