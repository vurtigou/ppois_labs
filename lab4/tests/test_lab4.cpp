#include "graph/Graph.h"
#include "model/Student.h"
#include "sorting/CountingSort.h"
#include "sorting/InsertionSort.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int assertions = 0;

void check(bool condition, const char* text, int line) {
    ++assertions;
    if (!condition) throw std::runtime_error("Line " + std::to_string(line) + ": " + text);
}

#define CHECK(...) check((__VA_ARGS__), #__VA_ARGS__, __LINE__)

template<class Error, class Function>
void throws(Function run) {
    try {
        run();
    } catch (const Error&) {
        ++assertions;
        return;
    }
    throw std::runtime_error("Missing expected exception");
}

struct Record {
    int key;
    std::string name;
    bool operator==(const Record& other) const { return key == other.key && name == other.name; }
};

template<class Sort>
void testDefaultSort() {
    const std::vector<int> input{4, -2, 4, 0, -7, 1};
    const std::vector<int> expected{-7, -2, 0, 1, 4, 4};
    std::vector<int> values = input;
    Sort::sort(values);
    CHECK(values == expected);
    Sort::sort_descending(values);
    CHECK(std::equal(values.begin(), values.end(), expected.rbegin()));

    int array[] = {4, -2, 4, 0, -7, 1};
    Sort::sort(array, std::size(array));
    CHECK(std::equal(std::begin(array), std::end(array), expected.begin()));
    Sort::sort_descending(array, std::size(array));
    CHECK(std::equal(std::begin(array), std::end(array), expected.rbegin()));

    Sort::sort(values.begin(), values.end());
    Sort::sort_descending(values.begin(), values.end());
    Sort::sort(static_cast<int*>(nullptr), std::size_t{0});
    Sort::sort_descending(static_cast<int*>(nullptr), std::size_t{0});
    throws<std::invalid_argument>([&] { Sort::sort(values.end(), values.begin()); });
    std::vector<int> empty;
    Sort::sort(empty);
    Sort::sort_descending(empty);
    CHECK(empty.empty());
    std::vector<int> single{8};
    Sort::sort(single);
    Sort::sort_descending(single);
    CHECK(single.front() == 8);
    std::vector<bool> bits{true, false, true, false};
    Sort::sort(bits);
    CHECK(bits == std::vector<bool>({false, false, true, true}));
    Sort::sort_descending(bits);
    CHECK(bits == std::vector<bool>({true, true, false, false}));
}

template<class Sort, class Order>
void testCustomSort(Order order) {
    const std::vector<Record> input{{2,"A"}, {3,"B"}, {2,"C"}, {1,"D"}};
    const std::vector<Record> ascending{{1,"D"}, {2,"A"}, {2,"C"}, {3,"B"}};
    const std::vector<Record> descending{{3,"B"}, {2,"A"}, {2,"C"}, {1,"D"}};
    auto values = input;
    Sort::sort(values, order);
    CHECK(values == ascending);
    Sort::sort_descending(values, order);
    CHECK(values == descending);
    Sort::sort(values.begin(), values.end(), order);
    CHECK(values == ascending);
    Sort::sort_descending(values.begin(), values.end(), order);
    CHECK(values == descending);

    Record array[] = {{2,"A"}, {3,"B"}, {2,"C"}, {1,"D"}};
    Sort::sort(array, std::size(array), order);
    CHECK(std::equal(std::begin(array), std::end(array), ascending.begin()));
    Sort::sort_descending(array, std::size(array), order);
    CHECK(std::equal(std::begin(array), std::end(array), descending.begin()));
    Sort::sort(static_cast<Record*>(nullptr), std::size_t{0}, order);
    Sort::sort_descending(static_cast<Record*>(nullptr), std::size_t{0}, order);
}

void testSorting() {
    testDefaultSort<InsertionSort>();
    testDefaultSort<CountingSort>();
    testCustomSort<InsertionSort>([](const Record& a, const Record& b){ return a.key < b.key; });
    testCustomSort<CountingSort>(&Record::key);

    using Limits = std::numeric_limits<std::int64_t>;
    std::vector<std::int64_t> extreme{Limits::max(), 0, Limits::min(), -1, Limits::max()};
    CountingSort::sort(extreme);
    CHECK((extreme == std::vector<std::int64_t>{Limits::min(), -1, 0, Limits::max(), Limits::max()}));
    const auto unsignedMax = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t large[] = {unsignedMax - 1, unsignedMax, unsignedMax - 2};
    CountingSort::sort_descending(large, std::size(large));
    CHECK(large[0] == unsignedMax && large[2] == unsignedMax - 2);
    std::vector<std::uint64_t> sparse{unsignedMax, 0, 4};
    CountingSort::sort(sparse);
    CHECK(sparse == std::vector<std::uint64_t>({0, 4, unsignedMax}));
    std::vector<double> precision{1.25, 1.2, -1.0, 1.25, 0.0};
    CountingSort::sort(precision);
    CHECK(precision == std::vector<double>({-1.0, 0.0, 1.2, 1.25, 1.25}));
    std::vector<double> nan{std::numeric_limits<double>::quiet_NaN()};
    throws<std::invalid_argument>([&] { CountingSort::sort(nan); });
    CHECK(std::isnan(nan.front()));
    std::vector<std::string> words{"beta", "alpha", "alphabet", "beta"};
    CountingSort::sort(words);
    CHECK(words == std::vector<std::string>({"alpha", "alphabet", "beta", "beta"}));

    std::vector<std::unique_ptr<int>> owned;
    owned.push_back(std::make_unique<int>(4));
    owned.push_back(std::make_unique<int>(2));
    CountingSort::sort(owned, [](const auto& p) { return *p; });
    CHECK(*owned[0] == 2 && *owned[1] == 4);
    InsertionSort::sort_descending(owned, [](const auto& a, const auto& b) { return *a < *b; });
    CHECK(*owned[0] == 4 && *owned[1] == 2);

    std::mt19937 random(112);
    std::uniform_int_distribution<int> keys(-15, 15);
    for (std::size_t size = 0; size < 65; ++size) {
        std::vector<int> samples(size);
        std::generate(samples.begin(), samples.end(), [&] { return keys(random); });
        auto expected = samples;
        std::stable_sort(expected.begin(), expected.end());
        auto inserted = samples;
        InsertionSort::sort(inserted);
        CountingSort::sort(samples);
        CHECK(samples == expected && inserted == expected);
    }
}

void testStudent() {
    using model::Student;
    Student value;
    CHECK(value.getId() == 0 && value.getAge() == 18);
    value.setId(12);
    value.setAge(19);
    value.setFullName("Amy");
    value.setGroup("X");
    value.setGpa(4.25);
    const Student equal(12, "Amy", "X", 4.25, 19);
    const Student next(13, "Ben", "Y", 4.2500000001, 20);
    CHECK(value == equal && value != next && value < next && next > value);
    CHECK(value <= equal && next >= equal && !(next <= equal));
    CHECK(Student::compareByGpa(value, next) && Student::compareByAge(value, next));
    CHECK(Student::compareById(value, next) && Student::compareByName(value, next) && Student::compareByGroup(value, next));
    CHECK(value.getFullName() == "Amy" && value.getGroup() == "X");
    throws<std::invalid_argument>([&] { value.setGpa(std::numeric_limits<double>::infinity()); });
    CHECK(value.getGpa() == 4.25);
    std::ostringstream stream;
    stream.precision(5);
    stream << value;
    CHECK(stream.str().find("gpa=4.25") != std::string::npos && stream.precision() == 5);
}

template <class Iterator, class Value>
void testIterator(Iterator begin, Iterator end, const std::vector<Value>& values) {
    using Traits = std::iterator_traits<Iterator>;
    static_assert(std::is_same_v<typename Traits::iterator_category, std::bidirectional_iterator_tag>);
    static_assert(std::is_lvalue_reference_v<typename Traits::reference>);
    static_assert(std::is_const_v<std::remove_reference_t<typename Traits::reference>>);
    CHECK(std::distance(begin, end) == static_cast<std::ptrdiff_t>(values.size()));
    CHECK(std::equal(begin, end, values.begin(), values.end()));

    auto it = begin;
    CHECK(*it == values.front());
    CHECK(it.operator->() == std::addressof(*it));
    const auto first = it++;
    CHECK(first == begin && first != it);
    CHECK(--it == begin);

    it = end;
    CHECK(it-- == end);
    CHECK(*it == values.back());
    ++it;
    CHECK(it == end);
    throws<GraphException>([&] { (void)*end; });
    throws<GraphException>([&] { ++end; });
    throws<GraphException>([&] { --begin; });
    Iterator unbound;
    CHECK(unbound == Iterator{} && unbound != begin);
    throws<GraphException>([&] { (void)*unbound; });
    throws<GraphException>([&] { ++unbound; });
    throws<GraphException>([&] { --unbound; });
}

template<class Iterator, class Value>
void testReverse(Iterator begin, Iterator end, const std::vector<Value>& normal) {
    CHECK(std::equal(begin, end, normal.rbegin(), normal.rend()));
    CHECK(begin.operator->() == std::addressof(*begin));
    CHECK(*begin.operator->() == normal.back());
    const auto before = begin++;
    CHECK(*before == normal.back());
    CHECK(--begin == before);
}

Graph<int> demoGraph() {
    Graph<int> graph;
    for (const int i : {0, 1, 2, 3, 4}) graph.addVertex(i);
    for (const auto& edge : {Edge<int>(1,2), Edge<int>(1,1), Edge<int>(2,1), Edge<int>(4,2)}) {
        graph.addEdge(edge.getFrom(), edge.getTo());
    }
    return graph;
}

void testIterators() {
    auto graph = demoGraph();
    const auto& ro = graph;
    const std::vector<int> vertices{0, 1, 2, 3, 4};
    const std::vector<int> adjacent{2, 1};
    const std::vector<Edge<int>> edges{{1,2}, {1,1}, {2,1}, {4,2}};
    const std::vector<Edge<int>> incident{{1,2}, {1,1}, {2,1}};

    testIterator(graph.beginVertices(), graph.endVertices(), vertices);
    testIterator(ro.beginVertices(), ro.endVertices(), vertices);
    testIterator(graph.beginEdges(), graph.endEdges(), edges);
    testIterator(ro.beginEdges(), ro.endEdges(), edges);
    testIterator(graph.beginIncidentEdges(1), graph.endIncidentEdges(1), incident);
    testIterator(ro.beginIncidentEdges(1), ro.endIncidentEdges(1), incident);
    testIterator(graph.beginAdjacentVertices(1), graph.endAdjacentVertices(1), adjacent);
    testIterator(ro.beginAdjacentVertices(1), ro.endAdjacentVertices(1), adjacent);
    testReverse(graph.rbeginVertices(), graph.rendVertices(), vertices);
    testReverse(ro.crbeginVertices(), ro.crendVertices(), vertices);
    testReverse(graph.rbeginEdges(), graph.rendEdges(), edges);
    testReverse(ro.crbeginEdges(), ro.crendEdges(), edges);
    testReverse(graph.rbeginIncidentEdges(1), graph.rendIncidentEdges(1), incident);
    testReverse(ro.crbeginIncidentEdges(1), ro.crendIncidentEdges(1), incident);
    testReverse(graph.rbeginAdjacentVertices(1), graph.rendAdjacentVertices(1), adjacent);
    testReverse(ro.crbeginAdjacentVertices(1), ro.crendAdjacentVertices(1), adjacent);

    CHECK(ConstVertexIterator<int>(graph.beginVertices()) == ro.cbeginVertices());
    CHECK(ConstEdgeIterator<int>(graph.beginEdges()) == ro.cbeginEdges());
    CHECK(ConstIncidentEdgeIterator<int>(graph.beginIncidentEdges(1)) == ro.cbeginIncidentEdges(1));
    CHECK(ConstAdjacentVertexIterator<int>(graph.beginAdjacentVertices(1)) == ro.cbeginAdjacentVertices(1));
    CHECK(ro.cendIncidentEdges(1) == ro.endIncidentEdges(1));
    CHECK(ro.cendAdjacentVertices(1) == ro.endAdjacentVertices(1));
    auto onlyIncoming = ro.cbeginIncidentEdges(2);
    CHECK(onlyIncoming->getFrom() == 2);
    ++onlyIncoming;
    CHECK(onlyIncoming->getFrom() == 1);
    ++onlyIncoming;
    CHECK(onlyIncoming->getFrom() == 4);
    auto edge = ro.cendEdges();
    --edge;
    CHECK(edge->getFrom() == 4);
    --edge;
    CHECK(edge->getFrom() == 2);
    --edge;
    CHECK(edge->getFrom() == 1);
    --edge;
    CHECK(edge == ro.cbeginEdges());
    CHECK(graph.beginIncidentEdges(3) == graph.endIncidentEdges(3));
    CHECK(graph.beginAdjacentVertices(3) == graph.endAdjacentVertices(3));
    CHECK(graph.rbeginAdjacentVertices(3) == graph.rendAdjacentVertices(3));
    CHECK(graph.rbeginIncidentEdges(3) == graph.rendIncidentEdges(3));
    throws<GraphException>([&] { graph.beginIncidentEdges(99); });
    throws<GraphException>([&] { graph.endIncidentEdges(99); });
    throws<GraphException>([&] { ro.cbeginIncidentEdges(99); });
    throws<GraphException>([&] { ro.cendIncidentEdges(99); });
    throws<GraphException>([&] { graph.beginAdjacentVertices(99); });
    throws<GraphException>([&] { graph.endAdjacentVertices(99); });
    throws<GraphException>([&] { ro.cbeginAdjacentVertices(99); });
    throws<GraphException>([&] { ro.cendAdjacentVertices(99); });

    Graph<int> empty;
    CHECK(empty.beginVertices() == empty.endVertices());
    CHECK(empty.crbeginVertices() == empty.crendVertices());
    CHECK(empty.beginEdges() == empty.endEdges());
    CHECK(empty.crbeginEdges() == empty.crendEdges());
    throws<GraphException>([&] { --empty.endVertices(); });
    throws<GraphException>([&] { --empty.endEdges(); });
}

struct EqualityOnly {
    int id;
    bool operator==(const EqualityOnly& other) const { return id == other.id; }
};

void testGraphOperations() {
    static_assert(std::is_same_v<DirectedAdjacencyListGraph<int>::value_type, int>);
    auto graph = demoGraph();
    const auto& read = graph;
    CHECK(!graph.empty() && graph.vertexCount() == 5 && graph.edgeCount() == 4);
    CHECK(graph.containsVertex(0) && !graph.containsVertex(90));
    CHECK(graph.containsEdge(4,2) && !graph.containsEdge(2,4) && !graph.containsEdge(0,99));
    CHECK(graph.inDegree(1) == 2 && graph.outDegree(1) == 2 && graph.degree(1) == 4);
    CHECK(graph.edgeDegree(1,2) == 3 && graph.edgeDegree(1,1) == 2);
    CHECK(graph.findVertex(0) == graph.beginVertices() && read.findVertex(90) == read.endVertices());
    int existing = 0;
    CHECK(!graph.addVertex(existing) && !graph.addVertex(2) && !graph.addEdge(1,2));
    throws<GraphException>([&] { graph.addEdge(99, 1); });
    throws<GraphException>([&] { graph.addEdge(1, 99); });
    throws<GraphException>([&] { graph.removeVertex(99); });
    throws<GraphException>([&] { graph.removeEdge(99,1); });
    throws<GraphException>([&] { graph.removeEdge(1,4); });
    throws<GraphException>([&] { graph.outDegree(99); });
    throws<GraphException>([&] { graph.inDegree(99); });
    throws<GraphException>([&] { graph.edgeDegree(99,1); });
    throws<GraphException>([&] { graph.edgeDegree(1,99); });
    throws<GraphException>([&] { graph.edgeDegree(1,4); });
    CHECK(graph.toString() == "Vertices: [0, 1, 2, 3, 4]\nEdges: [(1 -> 2), (1 -> 1), (2 -> 1), (4 -> 2)]");

    Graph<int> copy(graph);
    CHECK(copy == graph && !(copy != graph) && copy <= graph && copy >= graph);
    Graph<int> reordered;
    for (const int i : {4,3,2,1,0}) reordered.addVertex(i);
    for (const auto e : {Edge<int>(4,2), Edge<int>(2,1), Edge<int>(1,1), Edge<int>(1,2)}) {
        reordered.addEdge(e.getFrom(), e.getTo());
    }
    CHECK(reordered == graph && !(reordered < graph) && !(reordered > graph));
    reordered.removeEdge(1,2);
    reordered.addEdge(1,3);
    CHECK(reordered != graph && graph < reordered && reordered > graph && !(reordered <= graph));
    reordered.removeVertex(4);
    CHECK(reordered < graph);
    reordered.addVertex(5);
    CHECK(graph < reordered);
    auto& self = copy;
    copy = self;
    CHECK(copy == graph);
    copy = reordered;
    CHECK(copy == reordered);
    swap(copy, reordered);
    Graph<int> moved(std::move(copy));
    CHECK(moved == reordered && copy.empty() && copy.edgeCount() == 0);
    copy = std::move(moved);
    CHECK(copy == reordered && moved.empty());
    copy.clear();
    CHECK(copy.empty() && copy.edgeCount() == 0 && copy.toString() == "Vertices: []\nEdges: []");

    throws<GraphException>([&] { graph.eraseVertex(copy.cendVertices()); });
    throws<GraphException>([&] { graph.eraseVertex(graph.cendVertices()); });
    throws<GraphException>([&] { graph.eraseEdge(reordered.cbeginEdges()); });
    throws<GraphException>([&] { graph.eraseEdge(graph.cendEdges()); });
    const auto afterEdge = graph.eraseEdge(read.cbeginEdges());
    CHECK(afterEdge->getTo() == 1 && graph.edgeCount() == 3);
    const auto afterVertex = graph.eraseVertex(read.findVertex(1));
    CHECK(*afterVertex == 2 && graph.edgeCount() == 1 && !graph.containsVertex(1));
    graph.removeEdge(4,2);
    CHECK(graph.edgeCount() == 0);
    graph.removeVertex(4);
    CHECK(graph.vertexCount() == 3);
    auto tail = graph.endVertices();
    --tail;
    CHECK(graph.eraseVertex(tail) == graph.endVertices());

    Graph<EqualityOnly> one;
    one.addVertex({1}); one.addVertex({2}); one.addEdge({1},{2});
    Graph<EqualityOnly> two(one);
    CHECK(one == two);
    two.removeEdge({1},{2}); two.addEdge({2},{1});
    CHECK(one != two);
}

void testGraphAgainstMatrix() {
    constexpr int width = 6;
    std::mt19937 random(620);
    std::uniform_int_distribution<int> vertex(0, width - 1);
    std::array<std::array<bool, width>, width> matrix{};
    Graph<int> graph;
    for (int id = 0; id < width; ++id) graph.addVertex(id);
    for (int turn = 0; turn < 200; ++turn) {
        const int from = vertex(random), to = vertex(random);
        if (matrix[from][to]) graph.removeEdge(from, to);
        else CHECK(graph.addEdge(from, to));
        matrix[from][to] = !matrix[from][to];

        std::size_t all = 0;
        for (int source = 0; source < width; ++source) {
            std::size_t outgoing = 0, incoming = 0;
            for (int target = 0; target < width; ++target) {
                CHECK(graph.containsEdge(source, target) == matrix[source][target]);
                outgoing += matrix[source][target];
                incoming += matrix[target][source];
            }
            CHECK(graph.outDegree(source) == outgoing && graph.inDegree(source) == incoming);
            CHECK(static_cast<std::size_t>(std::distance(graph.cbeginIncidentEdges(source), graph.cendIncidentEdges(source))) ==
                  outgoing + incoming - matrix[source][source]);
            all += outgoing;
        }
        CHECK(graph.edgeCount() == all && static_cast<std::size_t>(std::distance(graph.cbeginEdges(), graph.cendEdges())) == all);
    }
}

} // namespace

int main() {
    try {
        testSorting();
        testStudent();
        testIterators();
        testGraphOperations();
        testGraphAgainstMatrix();
        std::cout << "Unit tests passed (" << assertions << " checks).\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
