#ifndef EDGE_CPP
#define EDGE_CPP

#include "graph/Edge.h"

// Конструкторы

template <typename T>
Edge<T>::Edge(const T& source, const T& destination)
    : from(source), to(destination) {}

template <typename T>
Edge<T>::Edge(T&& source, T&& destination)
    : from(std::move(source)), to(std::move(destination)) {}

// Операторы сравнения

template <typename T>
bool Edge<T>::operator==(const Edge& other) const {
    return from == other.from && to == other.to;
}

template <typename T>
bool Edge<T>::operator!=(const Edge& other) const {
    return !(*this == other);
}

template <typename T>
bool Edge<T>::operator<(const Edge& other) const {
    if (from < other.from) return true;
    if (other.from < from) return false;
    return to < other.to;
}

//Свободный оператор вывода 

template <typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& edge) {
    return out << '(' << edge.getFrom() << " -> " << edge.getTo() << ')';
}

#endif // EDGE_CPP
