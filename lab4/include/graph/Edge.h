#pragma once

#include <ostream>
#include <utility>

/// Directed edge value storing a source label and a destination label.
template <typename T>
class Edge {
private:
    T from;
    T to;

public:
    // Конструкторы копирования и перемещения
    Edge(const T& source, const T& destination);
    Edge(T&& source, T&& destination);

    // Геттеры 
    /// Returns the source vertex label.
    const T& getFrom() const { return from; }
    /// Returns the destination vertex label.
    const T& getTo()   const { return to;   }

    // Операторы 
    bool operator==(const Edge& other) const;
    bool operator!=(const Edge& other) const;
    bool operator<(const Edge& other) const;
};

// Объявление свободного шаблона operator<<
template <typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& edge);

#include "detail/Edge.tpp"
