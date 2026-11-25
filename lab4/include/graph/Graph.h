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

/**
 * @brief Directed graph backed by a private adjacency list.
 * @tparam T Vertex label type. Equality identifies a vertex. Ordering and
 * stream insertion are required only by the corresponding optional operations.
 * @details Vertex labels and arc endpoints are read-only through iterators.
 * Self-loops are supported. Each ordered pair of vertices has at most one arc.
 * See the README for iterator order and invalidation rules.
 */
template <typename T>
class Graph {
private:
    struct VertexNode;
    struct Arc {
        VertexNode* destination;
        Edge<T> edge;

        Arc(VertexNode* target, const T& from, const T& to) : destination(target), edge(from, to) {}
    };

    struct VertexNode {
        T value;
        std::list<Arc> outgoing;

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
    const Edge<T>& edgeValue(const void* source, const void* edgeCell) const;
    void eraseEdgeHandle(const void* source, const void* edgeCell);

    std::size_t adjacentVertexCount(const void* center) const;
    const T& adjacentVertexValue(const void* center, std::size_t index) const;
    std::size_t incidentEdgeCount(const void* center) const;
    const Edge<T>& incidentEdgeValue(const void* center, std::size_t index) const;

    void eraseVertexHandle(const void* vertexHandle);
    std::vector<T> canonicalVertices() const;
    std::vector<Edge<T>> canonicalEdges() const;

public:
    using value_type = T;
    using edge_type = Edge<T>;
    using size_type = std::size_t;
    using VertexReverseIterator = std::reverse_iterator<VertexIterator<T>>;
    using ConstVertexReverseIterator = std::reverse_iterator<ConstVertexIterator<T>>;
    using EdgeReverseIterator = std::reverse_iterator<EdgeIterator<T>>;
    using ConstEdgeReverseIterator = std::reverse_iterator<ConstEdgeIterator<T>>;
    using IncidentEdgeReverseIterator = std::reverse_iterator<IncidentEdgeIterator<T>>;
    using ConstIncidentEdgeReverseIterator = std::reverse_iterator<ConstIncidentEdgeIterator<T>>;
    using AdjacentVertexReverseIterator = std::reverse_iterator<AdjacentVertexIterator<T>>;
    using ConstAdjacentVertexReverseIterator = std::reverse_iterator<ConstAdjacentVertexIterator<T>>;

    /// Constructs an empty graph.
    Graph() noexcept = default;
    /// Copies vertices and arcs into independent storage.
    Graph(const Graph& other);
    /// Transfers the graph storage and leaves the source empty.
    Graph(Graph&& other) noexcept;
    /// Releases the owned vertices and arcs.
    ~Graph() = default;

    /// Replaces the graph by copying or moving the source value.
    Graph& operator=(Graph other) noexcept;
    /// Exchanges the storage and edge counts of two graphs.
    void swap(Graph& other) noexcept;

    /// Checks whether the graph has no vertices.
    bool empty() const noexcept;
    /// Removes all vertices and edges.
    void clear() noexcept;

    // Adding an existing vertex/edge returns false. Edges require both endpoints
    // to exist; a missing endpoint results in GraphException.
    /// Adds a vertex and returns false when its value already exists.
    bool addVertex(const T& value);
    /// Adds a vertex and returns false when its value already exists.
    bool addVertex(T&& value);
    /// Adds a directed edge and returns false for a duplicate. Both endpoints must exist.
    bool addEdge(const T& from, const T& to);

    // Removing a missing vertex or edge results in GraphException.
    /// Removes a vertex and its incident edges. A missing vertex throws GraphException.
    void removeVertex(const T& value);
    /// Removes a directed edge. A missing edge throws GraphException.
    void removeEdge(const T& from, const T& to);
    /// Erases the vertex at the supplied position and returns its successor.
    VertexIterator<T> eraseVertex(const ConstVertexIterator<T>& position);
    /// Erases the edge at the supplied position and returns its successor.
    EdgeIterator<T> eraseEdge(const ConstEdgeIterator<T>& position);

    /// Checks whether a label identifies a vertex.
    bool containsVertex(const T& value) const;
    /// Checks whether the given directed edge exists.
    bool containsEdge(const T& from, const T& to) const;

    /// Returns the vertex count.
    std::size_t vertexCount() const noexcept;
    /// Returns the directed edge count.
    std::size_t edgeCount() const noexcept;
    /// Counts arcs leaving the given vertex.
    std::size_t outDegree(const T& vertex) const;
    /// Counts arcs entering the given vertex.
    std::size_t inDegree(const T& vertex) const;
    // Directed degree is defined here as indegree + outdegree (a loop counts twice).
    /// Returns the sum of the incoming and outgoing degrees.
    std::size_t degree(const T& vertex) const;
    // Edge degree is the number of other edges sharing at least one endpoint
    // with the specified edge; the edge itself is not counted.
    /// Counts other arcs that share at least one endpoint with this arc.
    std::size_t edgeDegree(const T& from, const T& to) const;

    /// Returns an iterator to the first vertices.
    VertexIterator<T> beginVertices();
    /// Returns an iterator to the past-the-end position for vertices.
    VertexIterator<T> endVertices();
    /// Returns an iterator to the first vertices.
    ConstVertexIterator<T> beginVertices() const;
    /// Returns an iterator to the past-the-end position for vertices.
    ConstVertexIterator<T> endVertices() const;
    /// Returns an iterator to the first vertices.
    ConstVertexIterator<T> cbeginVertices() const;
    /// Returns an iterator to the past-the-end position for vertices.
    ConstVertexIterator<T> cendVertices() const;
    /// Finds a vertex by value or returns the vertex end iterator.
    VertexIterator<T> findVertex(const T& value);
    /// Finds a vertex by value or returns the vertex end iterator.
    ConstVertexIterator<T> findVertex(const T& value) const;

    /// Returns an iterator to the first directed edges.
    EdgeIterator<T> beginEdges();
    /// Returns an iterator to the past-the-end position for directed edges.
    EdgeIterator<T> endEdges();
    /// Returns an iterator to the first directed edges.
    ConstEdgeIterator<T> beginEdges() const;
    /// Returns an iterator to the past-the-end position for directed edges.
    ConstEdgeIterator<T> endEdges() const;
    /// Returns an iterator to the first directed edges.
    ConstEdgeIterator<T> cbeginEdges() const;
    /// Returns an iterator to the past-the-end position for directed edges.
    ConstEdgeIterator<T> cendEdges() const;

    /// Returns an iterator to the first incident edges.
    IncidentEdgeIterator<T> beginIncidentEdges(const T& vertex);
    /// Returns an iterator to the past-the-end position for incident edges.
    IncidentEdgeIterator<T> endIncidentEdges(const T& vertex);
    /// Returns an iterator to the first incident edges.
    ConstIncidentEdgeIterator<T> beginIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for incident edges.
    ConstIncidentEdgeIterator<T> endIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the first incident edges.
    ConstIncidentEdgeIterator<T> cbeginIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for incident edges.
    ConstIncidentEdgeIterator<T> cendIncidentEdges(const T& vertex) const;

    /// Returns an iterator to the first outgoing neighbors.
    AdjacentVertexIterator<T> beginAdjacentVertices(const T& vertex);
    /// Returns an iterator to the past-the-end position for outgoing neighbors.
    AdjacentVertexIterator<T> endAdjacentVertices(const T& vertex);
    /// Returns an iterator to the first outgoing neighbors.
    ConstAdjacentVertexIterator<T> beginAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for outgoing neighbors.
    ConstAdjacentVertexIterator<T> endAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the first outgoing neighbors.
    ConstAdjacentVertexIterator<T> cbeginAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for outgoing neighbors.
    ConstAdjacentVertexIterator<T> cendAdjacentVertices(const T& vertex) const;

    /// Returns an iterator to the first vertices in reverse order.
    VertexReverseIterator rbeginVertices();
    /// Returns an iterator to the past-the-end position for vertices in reverse order.
    VertexReverseIterator rendVertices();
    /// Returns an iterator to the first vertices in reverse order.
    ConstVertexReverseIterator rbeginVertices() const;
    /// Returns an iterator to the past-the-end position for vertices in reverse order.
    ConstVertexReverseIterator rendVertices() const;
    /// Returns an iterator to the first vertices in reverse order.
    ConstVertexReverseIterator crbeginVertices() const;
    /// Returns an iterator to the past-the-end position for vertices in reverse order.
    ConstVertexReverseIterator crendVertices() const;

    /// Returns an iterator to the first directed edges in reverse order.
    EdgeReverseIterator rbeginEdges();
    /// Returns an iterator to the past-the-end position for directed edges in reverse order.
    EdgeReverseIterator rendEdges();
    /// Returns an iterator to the first directed edges in reverse order.
    ConstEdgeReverseIterator rbeginEdges() const;
    /// Returns an iterator to the past-the-end position for directed edges in reverse order.
    ConstEdgeReverseIterator rendEdges() const;
    /// Returns an iterator to the first directed edges in reverse order.
    ConstEdgeReverseIterator crbeginEdges() const;
    /// Returns an iterator to the past-the-end position for directed edges in reverse order.
    ConstEdgeReverseIterator crendEdges() const;

    /// Returns an iterator to the first incident edges in reverse order.
    IncidentEdgeReverseIterator rbeginIncidentEdges(const T& vertex);
    /// Returns an iterator to the past-the-end position for incident edges in reverse order.
    IncidentEdgeReverseIterator rendIncidentEdges(const T& vertex);
    /// Returns an iterator to the first incident edges in reverse order.
    ConstIncidentEdgeReverseIterator rbeginIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for incident edges in reverse order.
    ConstIncidentEdgeReverseIterator rendIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the first incident edges in reverse order.
    ConstIncidentEdgeReverseIterator crbeginIncidentEdges(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for incident edges in reverse order.
    ConstIncidentEdgeReverseIterator crendIncidentEdges(const T& vertex) const;

    /// Returns an iterator to the first outgoing neighbors in reverse order.
    AdjacentVertexReverseIterator rbeginAdjacentVertices(const T& vertex);
    /// Returns an iterator to the past-the-end position for outgoing neighbors in reverse order.
    AdjacentVertexReverseIterator rendAdjacentVertices(const T& vertex);
    /// Returns an iterator to the first outgoing neighbors in reverse order.
    ConstAdjacentVertexReverseIterator rbeginAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for outgoing neighbors in reverse order.
    ConstAdjacentVertexReverseIterator rendAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the first outgoing neighbors in reverse order.
    ConstAdjacentVertexReverseIterator crbeginAdjacentVertices(const T& vertex) const;
    /// Returns an iterator to the past-the-end position for outgoing neighbors in reverse order.
    ConstAdjacentVertexReverseIterator crendAdjacentVertices(const T& vertex) const;

    /// Formats vertices and directed edges through their public iterators.
    std::string toString() const;

    /// Tests structural equality independent of insertion order.
    bool operator==(const Graph& other) const;
    /// Tests whether the graphs differ structurally.
    bool operator!=(const Graph& other) const;
    /// Orders graphs lexicographically by sorted labels and ordered endpoints.
    bool operator<(const Graph& other) const;
    /// Checks whether this graph sorts after the other graph.
    bool operator>(const Graph& other) const;
    /// Checks whether this graph sorts at or before the other graph.
    bool operator<=(const Graph& other) const;
    /// Checks whether this graph sorts at or after the other graph.
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
#include "detail/Graph.tpp"
