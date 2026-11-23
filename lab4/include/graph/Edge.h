#pragma once

#include <ostream>
#include <utility>

//класс представляет собой хранилище данных без поведения, описывающее направленное ребро графа
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
    const T& getFrom() const { return from; }
    const T& getTo()   const { return to;   }

    // Операторы 
    bool operator==(const Edge& other) const;
    bool operator!=(const Edge& other) const;
    bool operator<(const Edge& other) const;
};

// Объявление свободного шаблона operator<<
template <typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& edge);

#include "graph/Edge.cpp"
