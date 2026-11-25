# Lab 4 — Generic programming

This C++17 project implements stable insertion sort, stable counting sort, and a directed graph stored as a private adjacency list. It includes an interactive console program and a test suite that runs from the terminal.

## Build and run

Run these commands from the directory that contains `lab4/`:

```sh
make -C lab4
./lab4/build/lab4
make -C lab4 test
make -C lab4 coverage CXX=g++ GCOV=gcov
```

The build requires GNU Make and a C++17 compiler. The tests require Python 3.9 or newer. The coverage target requires GNU GCC and its matching gcov executable. On macOS the regular build works with Apple Clang via `CXX=c++`. For coverage use the matching versioned GNU tools installed on your machine, for example `CXX=g++-15 GCOV=gcov-15`.

## Demonstration

Select **Sorting** to run both algorithms on raw arrays and vectors of `Student` records or built-in integers. The student demonstration orders records by name, age, ID, and GPA. The integer demonstration checks the output against `std::sort`. You can also enter an integer sequence of your own.

Select **Graph** to add or remove vertices and directed edges, inspect a vertex and its degrees, print the graph, copy it, or clear it. For example, add vertices 10 and 20, add edge 10 → 20, and inspect vertex 10. It has out-degree 1 and in-degree 0. Add 20 → 10 to see the separate reverse arc. Input ends cleanly on EOF.

## Sorting

| Algorithm | API | Time and space |
|---|---|---|
| Insertion sort | `InsertionSort::sort` and `sort_descending` accept a random-access range, vector, or pointer and size. A custom comparator is optional. | O(n) best-case time, O(n²) worst-case time, O(1) auxiliary space. |
| Counting sort | `CountingSort::sort` and `sort_descending` accept the same containers and accept a key projection for records, including a pointer to a member. | Dense integral keys use O(n + k) time and space for n elements and k possible key values. Sparse keys or ordered non-integral keys are compressed to ranks in O(n log n) time and O(n) auxiliary space before the stable counting pass. |

Equal keys keep their input order in both directions. Insertion sort expects a strict weak ordering. Counting sort can order integers directly and can use keys such as strings or floating-point numbers through rank compression. Floating-point NaN keys throw `std::invalid_argument`. Key values must be copyable and orderable. For move-only records, pass a projection that produces a copyable key.

```cpp
#include "sorting/InsertionSort.h"
#include "sorting/CountingSort.h"
#include "model/Student.h"

#include <iterator>
#include <vector>

int main() {
    int scores[] = {9, -2, 9, 4};
    InsertionSort::sort(scores, std::size(scores));
    std::vector<model::Student> students{
        {2, "Sam Lee", "CS-22", 4.2, 20},
        {1, "Jean Ray", "CS-21", 4.5, 19},
    };
    CountingSort::sort(students, &model::Student::getAge);
}
```

## Directed graph

`Graph<T>`, also available as `DirectedAdjacencyListGraph<T>`, has one type parameter for the vertex label. Its public aliases include `value_type`, `edge_type`, and `size_type`. The default constructor makes an empty graph. A copy owns independent vertices and arcs. The destructor releases its storage automatically.

| Requirement | API or rule |
|---|---|
| Access and membership | `findVertex`, `containsVertex`, `containsEdge`, `vertexCount`, and `edgeCount`. The edge iterators expose source and destination labels. |
| Vertex and edge degrees | `inDegree` and `outDegree` distinguish incoming and outgoing arcs. `degree` adds the two, so a self-loop contributes twice. `edgeDegree` counts each other arc sharing either endpoint once. |
| Add and remove | `addVertex`, `addEdge`, `removeVertex`, `removeEdge`, `eraseVertex`, and `eraseEdge`. Removing a vertex removes all its incident edges. Erasing by iterator returns the next position. |
| Copy, comparison, and reset | Copy construction and assignment, `empty`, `clear`, and all six comparison operators. Equality uses labels and directed arcs regardless of insertion order. Ordering compares sorted vertex labels first and sorted ordered endpoints second. |
| Iteration and output | Bidirectional iterators traverse vertices, all edges, incident edges, and outgoing neighbors. All four support const and reverse traversal. `operator<<` formats the graph using its public iterators and `std::for_each`. |
| Error handling | Duplicate additions return false. Adding an edge requires both endpoints. Missing elements in removal or degree queries and invalid iterator boundaries throw `GraphException`. |

Vertices and each outgoing adjacency list preserve insertion order. All-edge traversal groups arcs by source vertex. Incident traversal visits outgoing arcs first, then incoming arcs from other vertices. A self-loop appears once in incident traversal. Adjacent traversal follows outgoing arcs. Edge and vertex references are read-only to preserve graph identity.

Iterators belong to their graph. Treat assignment, move, swap, clear, and any mutation affecting a range as invalidating that range's cursors. In particular, reacquire incident and adjacent iterators after modifying their edges. Use the successor returned by an iterator erase operation to continue traversal. Basic graph operations need label equality, edge storage and graph copying need copyable labels, and ordering operations need a strict weak label order consistent with equality.

## Testing, coverage, and API reference

`make test` runs the C++ unit suite and separate CLI sessions. The suite checks both algorithms against the standard library, covers stable ordering and extreme keys, exercises iterator variants and error paths, and compares a sequence of graph changes against an independent adjacency-matrix model.

Run `make coverage` to regenerate `build/coverage/coverage.txt`. The script merges instrumented executable lines across translation units by file and line, includes production templates and the CLI, and excludes test code and system headers. It fails the build at **90% or lower**. The verified GCC 14.2.0 result is **98.70%** (1,061 of 1,075 executable lines).

```sh
make -C lab4 lint
make -C lab4 sanitize
make -C lab4 docs
```

The lint target requires cppcheck. The sanitizer target requires a compiler with AddressSanitizer and UndefinedBehaviorSanitizer. The docs target requires Doxygen and produces `lab4/build/doxygen/html/index.html`.

## GitHub Actions

Place `.github/workflows/lab4.yml` in the shared course repository root alongside `lab4/`. The workflow tests the project, enforces coverage, runs static analysis, generates documentation, and tests the Linux executable after extraction. It uploads the binary archive as a workflow artifact. A pushed tag such as `lab4-v1.0.0` also publishes the tested archive in GitHub Releases.

Course specifications are available in [Lab 4](https://github.com/alix-zzh/ppois-course/blob/master/ru/lab4.md) and [Common requirements](https://github.com/alix-zzh/ppois-course/blob/master/ru/common_requirements.md).
