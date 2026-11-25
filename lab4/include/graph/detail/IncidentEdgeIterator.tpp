#pragma once

#include <memory>

#include "../IncidentEdgeIterator.h"
#include "../ConstIncidentEdgeIterator.h"
#include "../GraphException.h"
#include "../Graph.h"

template <typename T>
typename IncidentEdgeIterator<T>::reference IncidentEdgeIterator<T>::operator*() const {
    if (!graph_ || !center_ || index_ >= graph_->incidentEdgeCount(center_))
        throw GraphException("IncidentEdgeIterator: dereferencing an end or invalid iterator");
    return graph_->incidentEdgeValue(center_, index_);
}

template <typename T>
typename IncidentEdgeIterator<T>::pointer IncidentEdgeIterator<T>::operator->() const {
    return std::addressof(operator*());
}

template <typename T>
IncidentEdgeIterator<T>& IncidentEdgeIterator<T>::operator++() {
    if (!graph_ || !center_ || index_ >= graph_->incidentEdgeCount(center_))
        throw GraphException("IncidentEdgeIterator: incrementing an end or invalid iterator");
    ++index_;
    return *this;
}

template <typename T>
IncidentEdgeIterator<T> IncidentEdgeIterator<T>::operator++(int) {
    IncidentEdgeIterator copy(*this);
    ++(*this);
    return copy;
}

template <typename T>
IncidentEdgeIterator<T>& IncidentEdgeIterator<T>::operator--() {
    if (!graph_ || !center_ || index_ == 0)
        throw GraphException("IncidentEdgeIterator: decrementing before begin or an invalid iterator");
    --index_;
    return *this;
}

template <typename T>
IncidentEdgeIterator<T> IncidentEdgeIterator<T>::operator--(int) {
    IncidentEdgeIterator copy(*this);
    --(*this);
    return copy;
}

template <typename T>
bool IncidentEdgeIterator<T>::operator==(const IncidentEdgeIterator& other) const noexcept {
    return graph_ == other.graph_ && center_ == other.center_ && index_ == other.index_;
}

template <typename T>
bool IncidentEdgeIterator<T>::operator!=(const IncidentEdgeIterator& other) const noexcept {
    return !(*this == other);
}


