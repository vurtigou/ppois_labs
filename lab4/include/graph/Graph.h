#pragma once

#include "GraphForward.h"
#include "GraphException.h"
#include "Edge.h"
#include "VertexIterator.h"
#include "ConstVertexIterator.h"
#include "EdgeIterator.h"
#include "ConstEdgeIterator.h"
#include "IncidentEdgeIterator.h"
#include "ConstIncidentEdgeIterator.h"
#include "AdjacentVertexIterator.h"
#include "ConstAdjacentVertexIterator.h"

#include <algorithm>
#include <cstddef>
#include <iosfwd>
#include <ostream>
#include <iterator>
#include <list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Directed graph implemented with an adjacency list.
// The implementation (the list of vertex nodes and their outgoing lists) is
// private; callers work only with values and public iterators.
template <typename T>
class Graph {
private:
    struct VertexNode {
        T value;
        std::list<VertexNode*> outgoing;

        explicit VertexNode(const T& v) : value(v) {}
        explicit VertexNode(T&& v) : value(std::move(v)) {}
    };

    std::list<VertexNode> vertices_;
    std::size_t edgeCount_ = 0;

    template <typename U> friend class VertexIterator;
    template <typename U> friend class ConstVertexIterator;
    template <typename U> friend class EdgeIterator;
    template <typename U> friend class ConstEdgeIterator;
    template <typename U> friend class IncidentEdgeIterator;
    template <typename U> friend class ConstIncidentEdgeIterator;
    template <typename U> friend class AdjacentVertexIterator;
    template <typename U> friend class ConstAdjacentVertexIterator;

    VertexNode* findNode(const T& value);
    const VertexNode* findNode(const T& value) const;
    VertexNode* nodeFromHandle(const void* handle);
    const VertexNode* nodeFromHandle(const void* handle) const;

    // Opaque cursor helpers used by iterator classes. void pointers keep the
    // adjacency-list representation out of the public iterator interface.
    const void* firstVertexHandle() const noexcept;
    const void* nextVertexHandle(const void* current) const;
    const void* previousVertexHandle(const void* current) const;
    const T& vertexValue(const void* handle) const;

    void firstEdgeHandle(const void*& source, const void*& edgeCell) const noexcept;
    void nextEdgeHandle(const void* source, const void* edgeCell,
                        const void*& nextSource, const void*& nextEdgeCell) const;
    void previousEdgeHandle(const void* source, const void* edgeCell,
                            const void*& previousSource, const void*& previousEdgeCell) const;
    Edge<T> edgeValue(const void* source, const void* edgeCell) const;
    void eraseEdgeHandle(const void* source, const void* edgeCell);

    std::size_t adjacentVertexCount(const void* center) const;
    const T& adjacentVertexValue(const void* center, std::size_t index) const;
    std::size_t incidentEdgeCount(const void* center) const;
    Edge<T> incidentEdgeValue(const void* center, std::size_t index) const;

    void eraseVertexHandle(const void* vertexHandle);
    std::vector<T> canonicalVertices() const;
    std::vector<Edge<T>> canonicalEdges() const;

public:
    using VertexReverseIterator = std::reverse_iterator<VertexIterator<T>>;
    using ConstVertexReverseIterator = std::reverse_iterator<ConstVertexIterator<T>>;
    using EdgeReverseIterator = std::reverse_iterator<EdgeIterator<T>>;
    using ConstEdgeReverseIterator = std::reverse_iterator<ConstEdgeIterator<T>>;
    using IncidentEdgeReverseIterator = std::reverse_iterator<IncidentEdgeIterator<T>>;
    using ConstIncidentEdgeReverseIterator = std::reverse_iterator<ConstIncidentEdgeIterator<T>>;
    using AdjacentVertexReverseIterator = std::reverse_iterator<AdjacentVertexIterator<T>>;
    using ConstAdjacentVertexReverseIterator = std::reverse_iterator<ConstAdjacentVertexIterator<T>>;

    Graph() noexcept = default;
    Graph(const Graph& other);
    Graph(Graph&& other) noexcept;
    ~Graph() = default;

    Graph& operator=(Graph other) noexcept;
    void swap(Graph& other) noexcept;

    bool empty() const noexcept;
    void clear() noexcept;

    // Adding an existing vertex/edge returns false. Edges require both endpoints
    // to exist; a missing endpoint results in GraphException.
    bool addVertex(const T& value);
    bool addVertex(T&& value);
    bool addEdge(const T& from, const T& to);

    // Removing a missing vertex or edge results in GraphException.
    void removeVertex(const T& value);
    void removeEdge(const T& from, const T& to);
    VertexIterator<T> eraseVertex(VertexIterator<T> position);
    EdgeIterator<T> eraseEdge(EdgeIterator<T> position);

    bool containsVertex(const T& value) const;
    bool containsEdge(const T& from, const T& to) const;

    std::size_t vertexCount() const noexcept;
    std::size_t edgeCount() const noexcept;
    std::size_t outDegree(const T& vertex) const;
    std::size_t inDegree(const T& vertex) const;
    // Directed degree is defined here as indegree + outdegree (a loop counts twice).
    std::size_t degree(const T& vertex) const;
    // Edge degree is the number of other edges sharing at least one endpoint
    // with the specified edge; the edge itself is not counted.
    std::size_t edgeDegree(const T& from, const T& to) const;

    VertexIterator<T> beginVertices();
    VertexIterator<T> endVertices();
    ConstVertexIterator<T> beginVertices() const;
    ConstVertexIterator<T> endVertices() const;
    ConstVertexIterator<T> cbeginVertices() const;
    ConstVertexIterator<T> cendVertices() const;
    VertexIterator<T> findVertex(const T& value);
    ConstVertexIterator<T> findVertex(const T& value) const;

    EdgeIterator<T> beginEdges();
    EdgeIterator<T> endEdges();
    ConstEdgeIterator<T> beginEdges() const;
    ConstEdgeIterator<T> endEdges() const;
    ConstEdgeIterator<T> cbeginEdges() const;
    ConstEdgeIterator<T> cendEdges() const;

    IncidentEdgeIterator<T> beginIncidentEdges(const T& vertex);
    IncidentEdgeIterator<T> endIncidentEdges(const T& vertex);
    ConstIncidentEdgeIterator<T> beginIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeIterator<T> endIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeIterator<T> cbeginIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeIterator<T> cendIncidentEdges(const T& vertex) const;

    AdjacentVertexIterator<T> beginAdjacentVertices(const T& vertex);
    AdjacentVertexIterator<T> endAdjacentVertices(const T& vertex);
    ConstAdjacentVertexIterator<T> beginAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexIterator<T> endAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexIterator<T> cbeginAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexIterator<T> cendAdjacentVertices(const T& vertex) const;

    VertexReverseIterator rbeginVertices();
    VertexReverseIterator rendVertices();
    ConstVertexReverseIterator rbeginVertices() const;
    ConstVertexReverseIterator rendVertices() const;
    ConstVertexReverseIterator crbeginVertices() const;
    ConstVertexReverseIterator crendVertices() const;

    EdgeReverseIterator rbeginEdges();
    EdgeReverseIterator rendEdges();
    ConstEdgeReverseIterator rbeginEdges() const;
    ConstEdgeReverseIterator rendEdges() const;
    ConstEdgeReverseIterator crbeginEdges() const;
    ConstEdgeReverseIterator crendEdges() const;

    IncidentEdgeReverseIterator rbeginIncidentEdges(const T& vertex);
    IncidentEdgeReverseIterator rendIncidentEdges(const T& vertex);
    ConstIncidentEdgeReverseIterator rbeginIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeReverseIterator rendIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeReverseIterator crbeginIncidentEdges(const T& vertex) const;
    ConstIncidentEdgeReverseIterator crendIncidentEdges(const T& vertex) const;

    AdjacentVertexReverseIterator rbeginAdjacentVertices(const T& vertex);
    AdjacentVertexReverseIterator rendAdjacentVertices(const T& vertex);
    ConstAdjacentVertexReverseIterator rbeginAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexReverseIterator rendAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexReverseIterator crbeginAdjacentVertices(const T& vertex) const;
    ConstAdjacentVertexReverseIterator crendAdjacentVertices(const T& vertex) const;

    std::string toString() const;

    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;
    bool operator<(const Graph& other) const;
    bool operator>(const Graph& other) const;
    bool operator<=(const Graph& other) const;
    bool operator>=(const Graph& other) const;
};

// Explicit name of this assignment variant; Graph<T> remains as a short alias.
template <typename T>
using DirectedAdjacencyListGraph = Graph<T>;

template <typename T>
void swap(Graph<T>& left, Graph<T>& right) noexcept {
    left.swap(right);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& graph) {
    out << "Vertices: [";
    bool first = true;
    std::for_each(graph.cbeginVertices(), graph.cendVertices(),
                  [&out, &first](const T& vertex) {
                      if (!first) out << ", ";
                      out << vertex;
                      first = false;
                  });

    out << "]\nEdges: [";
    first = true;
    std::for_each(graph.cbeginEdges(), graph.cendEdges(),
                  [&out, &first](const Edge<T>& edge) {
                      if (!first) out << ", ";
                      out << edge;
                      first = false;
                  });
    return out << ']';
}

// Подключаем реализации (Edge-style) — иначе шаблон не инстанцируется в других TU
#include "Graph.cpp"
