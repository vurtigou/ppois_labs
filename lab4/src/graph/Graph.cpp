#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "GraphException.h"

#include <algorithm>
#include <memory>
#include <sstream>

// ---------- Internal lookup and opaque iterator cursors ----------

template <typename T>
typename Graph<T>::VertexNode* Graph<T>::findNode(const T& value) {
    for (auto& node : vertices_) {
        if (node.value == value) return std::addressof(node);
    }
    return nullptr;
}

template <typename T>
const typename Graph<T>::VertexNode* Graph<T>::findNode(const T& value) const {
    for (const auto& node : vertices_) {
        if (node.value == value) return std::addressof(node);
    }
    return nullptr;
}

template <typename T>
typename Graph<T>::VertexNode* Graph<T>::nodeFromHandle(const void* handle) {
    return const_cast<VertexNode*>(static_cast<const VertexNode*>(handle));
}

template <typename T>
const typename Graph<T>::VertexNode* Graph<T>::nodeFromHandle(const void* handle) const {
    return static_cast<const VertexNode*>(handle);
}

template <typename T>
const void* Graph<T>::firstVertexHandle() const noexcept {
    return vertices_.empty() ? nullptr : static_cast<const void*>(std::addressof(vertices_.front()));
}

template <typename T>
const void* Graph<T>::nextVertexHandle(const void* current) const {
    if (!current) throw GraphException("Graph: cannot increment the vertex end iterator");
    for (auto it = vertices_.cbegin(); it != vertices_.cend(); ++it) {
        if (static_cast<const void*>(std::addressof(*it)) == current) {
            ++it;
            return it == vertices_.cend() ? nullptr : static_cast<const void*>(std::addressof(*it));
        }
    }
    throw GraphException("Graph: vertex iterator does not refer to this graph or is invalid");
}

template <typename T>
const void* Graph<T>::previousVertexHandle(const void* current) const {
    if (vertices_.empty()) throw GraphException("Graph: cannot decrement an iterator in an empty graph");
    if (!current) return static_cast<const void*>(std::addressof(vertices_.back()));

    for (auto it = vertices_.cbegin(); it != vertices_.cend(); ++it) {
        if (static_cast<const void*>(std::addressof(*it)) == current) {
            if (it == vertices_.cbegin())
                throw GraphException("Graph: cannot decrement the begin vertex iterator");
            --it;
            return static_cast<const void*>(std::addressof(*it));
        }
    }
    throw GraphException("Graph: vertex iterator does not refer to this graph or is invalid");
}

template <typename T>
const T& Graph<T>::vertexValue(const void* handle) const {
    if (!handle) throw GraphException("Graph: invalid vertex iterator");
    return nodeFromHandle(handle)->value;
}

template <typename T>
void Graph<T>::firstEdgeHandle(const void*& source, const void*& edgeCell) const noexcept {
    source = nullptr;
    edgeCell = nullptr;
    for (const auto& node : vertices_) {
        if (!node.outgoing.empty()) {
            source = static_cast<const void*>(std::addressof(node));
            edgeCell = static_cast<const void*>(std::addressof(node.outgoing.front()));
            return;
        }
    }
}

template <typename T>
void Graph<T>::nextEdgeHandle(const void* source, const void* edgeCell,
                              const void*& nextSource, const void*& nextEdgeCell) const {
    nextSource = nullptr;
    nextEdgeCell = nullptr;
    if (!source || !edgeCell)
        throw GraphException("Graph: cannot increment the edge end iterator");

    auto sourceIt = vertices_.cbegin();
    for (; sourceIt != vertices_.cend(); ++sourceIt) {
        if (static_cast<const void*>(std::addressof(*sourceIt)) == source) break;
    }
    if (sourceIt == vertices_.cend())
        throw GraphException("Graph: edge iterator source is invalid");

    auto edgeIt = sourceIt->outgoing.cbegin();
    for (; edgeIt != sourceIt->outgoing.cend(); ++edgeIt) {
        if (static_cast<const void*>(std::addressof(*edgeIt)) == edgeCell) break;
    }
    if (edgeIt == sourceIt->outgoing.cend())
        throw GraphException("Graph: edge iterator is invalid");

    ++edgeIt;
    if (edgeIt != sourceIt->outgoing.cend()) {
        nextSource = source;
        nextEdgeCell = static_cast<const void*>(std::addressof(*edgeIt));
        return;
    }

    ++sourceIt;
    for (; sourceIt != vertices_.cend(); ++sourceIt) {
        if (!sourceIt->outgoing.empty()) {
            nextSource = static_cast<const void*>(std::addressof(*sourceIt));
            nextEdgeCell = static_cast<const void*>(std::addressof(sourceIt->outgoing.front()));
            return;
        }
    }
}

template <typename T>
void Graph<T>::previousEdgeHandle(const void* source, const void* edgeCell,
                                  const void*& previousSource, const void*& previousEdgeCell) const {
    previousSource = nullptr;
    previousEdgeCell = nullptr;

    if (!source && !edgeCell) {
        for (auto sourceIt = vertices_.crbegin(); sourceIt != vertices_.crend(); ++sourceIt) {
            if (!sourceIt->outgoing.empty()) {
                previousSource = static_cast<const void*>(std::addressof(*sourceIt));
                previousEdgeCell = static_cast<const void*>(std::addressof(sourceIt->outgoing.back()));
                return;
            }
        }
        throw GraphException("Graph: cannot decrement end edge iterator in an edgeless graph");
    }
    if (!source || !edgeCell)
        throw GraphException("Graph: invalid edge iterator state");

    auto sourceIt = vertices_.cbegin();
    for (; sourceIt != vertices_.cend(); ++sourceIt) {
        if (static_cast<const void*>(std::addressof(*sourceIt)) == source) break;
    }
    if (sourceIt == vertices_.cend())
        throw GraphException("Graph: edge iterator source is invalid");

    auto edgeIt = sourceIt->outgoing.cbegin();
    for (; edgeIt != sourceIt->outgoing.cend(); ++edgeIt) {
        if (static_cast<const void*>(std::addressof(*edgeIt)) == edgeCell) break;
    }
    if (edgeIt == sourceIt->outgoing.cend())
        throw GraphException("Graph: edge iterator is invalid");

    if (edgeIt != sourceIt->outgoing.cbegin()) {
        --edgeIt;
        previousSource = source;
        previousEdgeCell = static_cast<const void*>(std::addressof(*edgeIt));
        return;
    }

    while (sourceIt != vertices_.cbegin()) {
        --sourceIt;
        if (!sourceIt->outgoing.empty()) {
            previousSource = static_cast<const void*>(std::addressof(*sourceIt));
            previousEdgeCell = static_cast<const void*>(std::addressof(sourceIt->outgoing.back()));
            return;
        }
    }
    throw GraphException("Graph: cannot decrement the begin edge iterator");
}

template <typename T>
Edge<T> Graph<T>::edgeValue(const void* source, const void* edgeCell) const {
    if (!source || !edgeCell) throw GraphException("Graph: invalid edge iterator");
    const auto* fromNode = nodeFromHandle(source);
    auto* toNode = *static_cast<VertexNode* const*>(edgeCell);
    if (!toNode) throw GraphException("Graph: edge points to an invalid vertex");
    return Edge<T>(fromNode->value, toNode->value);
}

template <typename T>
void Graph<T>::eraseEdgeHandle(const void* source, const void* edgeCell) {
    if (!source || !edgeCell) throw GraphException("Graph: cannot erase an invalid edge iterator");
    VertexNode* sourceNode = nodeFromHandle(source);
    for (auto it = sourceNode->outgoing.begin(); it != sourceNode->outgoing.end(); ++it) {
        if (static_cast<const void*>(std::addressof(*it)) == edgeCell) {
            sourceNode->outgoing.erase(it);
            --edgeCount_;
            return;
        }
    }
    throw GraphException("Graph: edge iterator does not refer to an edge in this graph");
}

// ---------- Adjacent-vertex and incident-edge iterator helpers ----------

template <typename T>
std::size_t Graph<T>::adjacentVertexCount(const void* center) const {
    if (!center) throw GraphException("Graph: invalid center vertex for adjacent iterator");
    return nodeFromHandle(center)->outgoing.size();
}

template <typename T>
const T& Graph<T>::adjacentVertexValue(const void* center, std::size_t index) const {
    if (!center) throw GraphException("Graph: invalid center vertex for adjacent iterator");
    const auto* node = nodeFromHandle(center);
    if (index >= node->outgoing.size()) throw GraphException("Graph: adjacent-vertex iterator is at end");
    auto it = node->outgoing.cbegin();
    std::advance(it, static_cast<std::ptrdiff_t>(index));
    return (*it)->value;
}

template <typename T>
std::size_t Graph<T>::incidentEdgeCount(const void* center) const {
    if (!center) throw GraphException("Graph: invalid center vertex for incident-edge iterator");
    const auto* centerNode = nodeFromHandle(center);
    std::size_t count = centerNode->outgoing.size();
    for (const auto& source : vertices_) {
        if (std::addressof(source) == centerNode) continue; // a loop is counted only in outgoing part
        for (const auto* destination : source.outgoing) {
            if (destination == centerNode) ++count;
        }
    }
    return count;
}

template <typename T>
Edge<T> Graph<T>::incidentEdgeValue(const void* center, std::size_t index) const {
    if (!center) throw GraphException("Graph: invalid center vertex for incident-edge iterator");
    const auto* centerNode = nodeFromHandle(center);

    if (index < centerNode->outgoing.size()) {
        auto it = centerNode->outgoing.cbegin();
        std::advance(it, static_cast<std::ptrdiff_t>(index));
        return Edge<T>(centerNode->value, (*it)->value);
    }

    index -= centerNode->outgoing.size();
    for (const auto& source : vertices_) {
        if (std::addressof(source) == centerNode) continue;
        for (const auto* destination : source.outgoing) {
            if (destination == centerNode) {
                if (index == 0) return Edge<T>(source.value, centerNode->value);
                --index;
            }
        }
    }
    throw GraphException("Graph: incident-edge iterator is at end");
}

// ---------- Construction, assignment and basic state ----------

template <typename T>
Graph<T>::Graph(const Graph& other) {
    for (const auto& node : other.vertices_) addVertex(node.value);
    for (const auto& source : other.vertices_) {
        for (const auto* destination : source.outgoing) {
            addEdge(source.value, destination->value);
        }
    }
}

template <typename T>
Graph<T>::Graph(Graph&& other) noexcept
    : vertices_(std::move(other.vertices_)), edgeCount_(other.edgeCount_) {
    other.edgeCount_ = 0;
}

template <typename T>
Graph<T>& Graph<T>::operator=(Graph other) noexcept {
    swap(other);
    return *this;
}

template <typename T>
void Graph<T>::swap(Graph& other) noexcept {
    vertices_.swap(other.vertices_);
    std::swap(edgeCount_, other.edgeCount_);
}

template <typename T>
bool Graph<T>::empty() const noexcept {
    return vertices_.empty();
}

template <typename T>
void Graph<T>::clear() noexcept {
    vertices_.clear();
    edgeCount_ = 0;
}

// ---------- Modification and queries ----------

template <typename T>
bool Graph<T>::addVertex(const T& value) {
    if (findNode(value)) return false;
    vertices_.emplace_back(value);
    return true;
}

template <typename T>
bool Graph<T>::addVertex(T&& value) {
    if (findNode(value)) return false;
    vertices_.emplace_back(std::move(value));
    return true;
}

template <typename T>
bool Graph<T>::addEdge(const T& from, const T& to) {
    VertexNode* source = findNode(from);
    VertexNode* destination = findNode(to);
    if (!source) throw GraphException("Graph::addEdge: source vertex does not exist");
    if (!destination) throw GraphException("Graph::addEdge: destination vertex does not exist");

    for (const auto* existing : source->outgoing) {
        if (existing == destination) return false;
    }
    source->outgoing.push_back(destination);
    ++edgeCount_;
    return true;
}

template <typename T>
void Graph<T>::eraseVertexHandle(const void* vertexHandle) {
    if (!vertexHandle) throw GraphException("Graph: cannot erase an invalid vertex iterator");
    auto targetIt = vertices_.begin();
    for (; targetIt != vertices_.end(); ++targetIt) {
        if (static_cast<const void*>(std::addressof(*targetIt)) == vertexHandle) break;
    }
    if (targetIt == vertices_.end())
        throw GraphException("Graph: vertex iterator does not refer to this graph");

    VertexNode* target = std::addressof(*targetIt);
    const std::size_t outgoingCount = target->outgoing.size();
    std::size_t incomingFromOtherVertices = 0;

    for (auto& source : vertices_) {
        if (std::addressof(source) == target) continue;
        for (auto edgeIt = source.outgoing.begin(); edgeIt != source.outgoing.end();) {
            if (*edgeIt == target) {
                edgeIt = source.outgoing.erase(edgeIt);
                ++incomingFromOtherVertices;
            } else {
                ++edgeIt;
            }
        }
    }

    edgeCount_ -= outgoingCount + incomingFromOtherVertices;
    vertices_.erase(targetIt);
}

template <typename T>
void Graph<T>::removeVertex(const T& value) {
    VertexNode* node = findNode(value);
    if (!node) throw GraphException("Graph::removeVertex: vertex does not exist");
    eraseVertexHandle(node);
}

template <typename T>
void Graph<T>::removeEdge(const T& from, const T& to) {
    VertexNode* source = findNode(from);
    VertexNode* destination = findNode(to);
    if (!source || !destination)
        throw GraphException("Graph::removeEdge: one or both endpoint vertices do not exist");
    for (auto it = source->outgoing.begin(); it != source->outgoing.end(); ++it) {
        if (*it == destination) {
            source->outgoing.erase(it);
            --edgeCount_;
            return;
        }
    }
    throw GraphException("Graph::removeEdge: edge does not exist");
}

template <typename T>
VertexIterator<T> Graph<T>::eraseVertex(VertexIterator<T> position) {
    if (position.graph_ != this || !position.node_)
        throw GraphException("Graph::eraseVertex: iterator is foreign or at end");
    const void* next = nextVertexHandle(position.node_);
    eraseVertexHandle(position.node_);
    return VertexIterator<T>(this, next);
}

template <typename T>
EdgeIterator<T> Graph<T>::eraseEdge(EdgeIterator<T> position) {
    if (position.graph_ != this || !position.source_ || !position.edgeCell_)
        throw GraphException("Graph::eraseEdge: iterator is foreign or at end");
    const void* nextSource = nullptr;
    const void* nextCell = nullptr;
    nextEdgeHandle(position.source_, position.edgeCell_, nextSource, nextCell);
    eraseEdgeHandle(position.source_, position.edgeCell_);
    return EdgeIterator<T>(this, nextSource, nextCell);
}

template <typename T>
bool Graph<T>::containsVertex(const T& value) const {
    return findNode(value) != nullptr;
}

template <typename T>
bool Graph<T>::containsEdge(const T& from, const T& to) const {
    const VertexNode* source = findNode(from);
    const VertexNode* destination = findNode(to);
    if (!source || !destination) return false;
    for (const auto* existing : source->outgoing) {
        if (existing == destination) return true;
    }
    return false;
}

template <typename T>
std::size_t Graph<T>::vertexCount() const noexcept {
    return vertices_.size();
}

template <typename T>
std::size_t Graph<T>::edgeCount() const noexcept {
    return edgeCount_;
}

template <typename T>
std::size_t Graph<T>::outDegree(const T& vertex) const {
    const VertexNode* node = findNode(vertex);
    if (!node) throw GraphException("Graph::outDegree: vertex does not exist");
    return node->outgoing.size();
}

template <typename T>
std::size_t Graph<T>::inDegree(const T& vertex) const {
    const VertexNode* node = findNode(vertex);
    if (!node) throw GraphException("Graph::inDegree: vertex does not exist");
    std::size_t result = 0;
    for (const auto& source : vertices_) {
        for (const auto* destination : source.outgoing) {
            if (destination == node) ++result;
        }
    }
    return result;
}

template <typename T>
std::size_t Graph<T>::degree(const T& vertex) const {
    return inDegree(vertex) + outDegree(vertex);
}

template <typename T>
std::size_t Graph<T>::edgeDegree(const T& from, const T& to) const {
    const VertexNode* edgeFrom = findNode(from);
    const VertexNode* edgeTo = findNode(to);
    if (!edgeFrom || !edgeTo || !containsEdge(from, to))
        throw GraphException("Graph::edgeDegree: edge does not exist");

    std::size_t result = 0;
    for (const auto& source : vertices_) {
        for (const auto* destination : source.outgoing) {
            if (std::addressof(source) == edgeFrom && destination == edgeTo) continue;
            const bool sharesEndpoint =
                (std::addressof(source) == edgeFrom) ||
                (std::addressof(source) == edgeTo) ||
                (destination == edgeFrom) ||
                (destination == edgeTo);
            if (sharesEndpoint) ++result;
        }
    }
    return result;
}

// ---------- Vertex iterators ----------

template <typename T>
VertexIterator<T> Graph<T>::beginVertices() {
    return VertexIterator<T>(this, firstVertexHandle());
}

template <typename T>
VertexIterator<T> Graph<T>::endVertices() {
    return VertexIterator<T>(this, nullptr);
}

template <typename T>
ConstVertexIterator<T> Graph<T>::beginVertices() const {
    return ConstVertexIterator<T>(this, firstVertexHandle());
}

template <typename T>
ConstVertexIterator<T> Graph<T>::endVertices() const {
    return ConstVertexIterator<T>(this, nullptr);
}

template <typename T>
ConstVertexIterator<T> Graph<T>::cbeginVertices() const {
    return beginVertices();
}

template <typename T>
ConstVertexIterator<T> Graph<T>::cendVertices() const {
    return endVertices();
}

template <typename T>
VertexIterator<T> Graph<T>::findVertex(const T& value) {
    return VertexIterator<T>(this, findNode(value));
}

template <typename T>
ConstVertexIterator<T> Graph<T>::findVertex(const T& value) const {
    return ConstVertexIterator<T>(this, findNode(value));
}

// ---------- Global edge iterators ----------

template <typename T>
EdgeIterator<T> Graph<T>::beginEdges() {
    const void* source = nullptr;
    const void* cell = nullptr;
    firstEdgeHandle(source, cell);
    return EdgeIterator<T>(this, source, cell);
}

template <typename T>
EdgeIterator<T> Graph<T>::endEdges() {
    return EdgeIterator<T>(this, nullptr, nullptr);
}

template <typename T>
ConstEdgeIterator<T> Graph<T>::beginEdges() const {
    const void* source = nullptr;
    const void* cell = nullptr;
    firstEdgeHandle(source, cell);
    return ConstEdgeIterator<T>(this, source, cell);
}

template <typename T>
ConstEdgeIterator<T> Graph<T>::endEdges() const {
    return ConstEdgeIterator<T>(this, nullptr, nullptr);
}

template <typename T>
ConstEdgeIterator<T> Graph<T>::cbeginEdges() const { return beginEdges(); }

template <typename T>
ConstEdgeIterator<T> Graph<T>::cendEdges() const { return endEdges(); }

// ---------- Incident-edge iterators ----------

template <typename T>
IncidentEdgeIterator<T> Graph<T>::beginIncidentEdges(const T& vertex) {
    VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::beginIncidentEdges: vertex does not exist");
    return IncidentEdgeIterator<T>(this, center, 0);
}

template <typename T>
IncidentEdgeIterator<T> Graph<T>::endIncidentEdges(const T& vertex) {
    VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::endIncidentEdges: vertex does not exist");
    return IncidentEdgeIterator<T>(this, center, incidentEdgeCount(center));
}

template <typename T>
ConstIncidentEdgeIterator<T> Graph<T>::beginIncidentEdges(const T& vertex) const {
    const VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::beginIncidentEdges: vertex does not exist");
    return ConstIncidentEdgeIterator<T>(this, center, 0);
}

template <typename T>
ConstIncidentEdgeIterator<T> Graph<T>::endIncidentEdges(const T& vertex) const {
    const VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::endIncidentEdges: vertex does not exist");
    return ConstIncidentEdgeIterator<T>(this, center, incidentEdgeCount(center));
}

template <typename T>
ConstIncidentEdgeIterator<T> Graph<T>::cbeginIncidentEdges(const T& vertex) const {
    return beginIncidentEdges(vertex);
}

template <typename T>
ConstIncidentEdgeIterator<T> Graph<T>::cendIncidentEdges(const T& vertex) const {
    return endIncidentEdges(vertex);
}

// ---------- Adjacent-vertex iterators (outgoing neighbours) ----------

template <typename T>
AdjacentVertexIterator<T> Graph<T>::beginAdjacentVertices(const T& vertex) {
    VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::beginAdjacentVertices: vertex does not exist");
    return AdjacentVertexIterator<T>(this, center, 0);
}

template <typename T>
AdjacentVertexIterator<T> Graph<T>::endAdjacentVertices(const T& vertex) {
    VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::endAdjacentVertices: vertex does not exist");
    return AdjacentVertexIterator<T>(this, center, adjacentVertexCount(center));
}

template <typename T>
ConstAdjacentVertexIterator<T> Graph<T>::beginAdjacentVertices(const T& vertex) const {
    const VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::beginAdjacentVertices: vertex does not exist");
    return ConstAdjacentVertexIterator<T>(this, center, 0);
}

template <typename T>
ConstAdjacentVertexIterator<T> Graph<T>::endAdjacentVertices(const T& vertex) const {
    const VertexNode* center = findNode(vertex);
    if (!center) throw GraphException("Graph::endAdjacentVertices: vertex does not exist");
    return ConstAdjacentVertexIterator<T>(this, center, adjacentVertexCount(center));
}

template <typename T>
ConstAdjacentVertexIterator<T> Graph<T>::cbeginAdjacentVertices(const T& vertex) const {
    return beginAdjacentVertices(vertex);
}

template <typename T>
ConstAdjacentVertexIterator<T> Graph<T>::cendAdjacentVertices(const T& vertex) const {
    return endAdjacentVertices(vertex);
}

// ---------- Reverse iterators for every iterator family ----------

template <typename T>
typename Graph<T>::VertexReverseIterator Graph<T>::rbeginVertices() { return VertexReverseIterator(endVertices()); }
template <typename T>
typename Graph<T>::VertexReverseIterator Graph<T>::rendVertices() { return VertexReverseIterator(beginVertices()); }
template <typename T>
typename Graph<T>::ConstVertexReverseIterator Graph<T>::rbeginVertices() const { return ConstVertexReverseIterator(endVertices()); }
template <typename T>
typename Graph<T>::ConstVertexReverseIterator Graph<T>::rendVertices() const { return ConstVertexReverseIterator(beginVertices()); }
template <typename T>
typename Graph<T>::ConstVertexReverseIterator Graph<T>::crbeginVertices() const { return rbeginVertices(); }
template <typename T>
typename Graph<T>::ConstVertexReverseIterator Graph<T>::crendVertices() const { return rendVertices(); }

template <typename T>
typename Graph<T>::EdgeReverseIterator Graph<T>::rbeginEdges() { return EdgeReverseIterator(endEdges()); }
template <typename T>
typename Graph<T>::EdgeReverseIterator Graph<T>::rendEdges() { return EdgeReverseIterator(beginEdges()); }
template <typename T>
typename Graph<T>::ConstEdgeReverseIterator Graph<T>::rbeginEdges() const { return ConstEdgeReverseIterator(endEdges()); }
template <typename T>
typename Graph<T>::ConstEdgeReverseIterator Graph<T>::rendEdges() const { return ConstEdgeReverseIterator(beginEdges()); }
template <typename T>
typename Graph<T>::ConstEdgeReverseIterator Graph<T>::crbeginEdges() const { return rbeginEdges(); }
template <typename T>
typename Graph<T>::ConstEdgeReverseIterator Graph<T>::crendEdges() const { return rendEdges(); }

template <typename T>
typename Graph<T>::IncidentEdgeReverseIterator Graph<T>::rbeginIncidentEdges(const T& vertex) { return IncidentEdgeReverseIterator(endIncidentEdges(vertex)); }
template <typename T>
typename Graph<T>::IncidentEdgeReverseIterator Graph<T>::rendIncidentEdges(const T& vertex) { return IncidentEdgeReverseIterator(beginIncidentEdges(vertex)); }
template <typename T>
typename Graph<T>::ConstIncidentEdgeReverseIterator Graph<T>::rbeginIncidentEdges(const T& vertex) const { return ConstIncidentEdgeReverseIterator(endIncidentEdges(vertex)); }
template <typename T>
typename Graph<T>::ConstIncidentEdgeReverseIterator Graph<T>::rendIncidentEdges(const T& vertex) const { return ConstIncidentEdgeReverseIterator(beginIncidentEdges(vertex)); }
template <typename T>
typename Graph<T>::ConstIncidentEdgeReverseIterator Graph<T>::crbeginIncidentEdges(const T& vertex) const { return rbeginIncidentEdges(vertex); }
template <typename T>
typename Graph<T>::ConstIncidentEdgeReverseIterator Graph<T>::crendIncidentEdges(const T& vertex) const { return rendIncidentEdges(vertex); }

template <typename T>
typename Graph<T>::AdjacentVertexReverseIterator Graph<T>::rbeginAdjacentVertices(const T& vertex) { return AdjacentVertexReverseIterator(endAdjacentVertices(vertex)); }
template <typename T>
typename Graph<T>::AdjacentVertexReverseIterator Graph<T>::rendAdjacentVertices(const T& vertex) { return AdjacentVertexReverseIterator(beginAdjacentVertices(vertex)); }
template <typename T>
typename Graph<T>::ConstAdjacentVertexReverseIterator Graph<T>::rbeginAdjacentVertices(const T& vertex) const { return ConstAdjacentVertexReverseIterator(endAdjacentVertices(vertex)); }
template <typename T>
typename Graph<T>::ConstAdjacentVertexReverseIterator Graph<T>::rendAdjacentVertices(const T& vertex) const { return ConstAdjacentVertexReverseIterator(beginAdjacentVertices(vertex)); }
template <typename T>
typename Graph<T>::ConstAdjacentVertexReverseIterator Graph<T>::crbeginAdjacentVertices(const T& vertex) const { return rbeginAdjacentVertices(vertex); }
template <typename T>
typename Graph<T>::ConstAdjacentVertexReverseIterator Graph<T>::crendAdjacentVertices(const T& vertex) const { return rendAdjacentVertices(vertex); }

// ---------- Text representation and graph comparisons ----------

template <typename T>
std::string Graph<T>::toString() const {
    std::ostringstream out;
    out << *this; // operator<< traverses the public iterators with std::for_each
    return out.str();
}

template <typename T>
std::vector<T> Graph<T>::canonicalVertices() const {
    std::vector<T> result;
    result.reserve(vertices_.size());
    for (const auto& node : vertices_) result.push_back(node.value);
    std::sort(result.begin(), result.end());
    return result;
}

template <typename T>
std::vector<Edge<T>> Graph<T>::canonicalEdges() const {
    std::vector<Edge<T>> result;
    result.reserve(edgeCount_);
    for (const auto& source : vertices_) {
        for (const auto* destination : source.outgoing) {
            result.emplace_back(source.value, destination->value);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

template <typename T>
bool Graph<T>::operator==(const Graph& other) const {
    if (vertexCount() != other.vertexCount() || edgeCount() != other.edgeCount()) return false;
    return canonicalVertices() == other.canonicalVertices() && canonicalEdges() == other.canonicalEdges();
}

template <typename T>
bool Graph<T>::operator!=(const Graph& other) const { return !(*this == other); }

template <typename T>
bool Graph<T>::operator<(const Graph& other) const {
    const auto leftVertices = canonicalVertices();
    const auto rightVertices = other.canonicalVertices();
    if (std::lexicographical_compare(leftVertices.begin(), leftVertices.end(),
                                     rightVertices.begin(), rightVertices.end())) return true;
    if (std::lexicographical_compare(rightVertices.begin(), rightVertices.end(),
                                     leftVertices.begin(), leftVertices.end())) return false;
    const auto leftEdges = canonicalEdges();
    const auto rightEdges = other.canonicalEdges();
    return std::lexicographical_compare(leftEdges.begin(), leftEdges.end(),
                                        rightEdges.begin(), rightEdges.end());
}

template <typename T>
bool Graph<T>::operator>(const Graph& other) const { return other < *this; }

template <typename T>
bool Graph<T>::operator<=(const Graph& other) const { return !(other < *this); }

template <typename T>
bool Graph<T>::operator>=(const Graph& other) const { return !(*this < other); }

#endif // GRAPH_CPP
