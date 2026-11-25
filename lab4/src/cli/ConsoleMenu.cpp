#include "cli/ConsoleMenu.h"
#include "graph/Graph.h"
#include "model/Student.h"
#include "sorting/CountingSort.h"
#include "sorting/InsertionSort.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace cli {
namespace {

using model::Student;

std::optional<int> readInt(const char* prompt) {
    for (std::string line; std::cout << prompt && std::getline(std::cin, line);) {
        std::istringstream input(line);
        int number;
        char extra;
        if (input >> number && !(input >> extra)) return number;
        std::cout << "Enter one integer.\n";
    }
    return std::nullopt;
}

template <class Range>
void print(const char* label, const Range& range) {
    std::cout << label << '\n';
    for (const auto& item : range) std::cout << "  " << item << '\n';
}

void demonstrateStudents() {
    Student students[] = {
        {104, "Ivan Petrov", "CS-21", 4.65, 19},
        {101, "Anna Smirnova", "CS-21", 4.90, 18},
        {107, "Dmitry Volkov", "CS-22", 3.80, 20},
        {102, "Elena Kuznetsova", "CS-22", 4.65, 19},
    };
    std::vector<Student> vector(std::begin(students), std::end(students));
    InsertionSort::sort(students, std::size(students), Student::compareByName);
    CountingSort::sort(vector, &Student::getAge);
    print("Insertion sort, student array by name", students);
    print("Counting sort, student vector by age", vector);

    CountingSort::sort(students, std::size(students), &Student::getId);
    InsertionSort::sort(vector, Student::compareByGpa);
    print("Counting sort, student array by ID", students);
    print("Insertion sort, student vector by GPA", vector);
}

void demonstrateIntegers() {
    int numbers[] = {42, 7, -19, 3, 25, 19, 1, -14};
    std::vector<int> vector(std::begin(numbers), std::end(numbers));
    auto expected = vector;
    std::sort(expected.begin(), expected.end());
    InsertionSort::sort(numbers, std::size(numbers));
    CountingSort::sort(vector);
    print("Sorted integer array", numbers);
    std::cout << "Both algorithms agree with std::sort: " << std::boolalpha
              << (vector == expected && std::equal(std::begin(numbers), std::end(numbers), expected.begin())) << '\n';
    CountingSort::sort_descending(numbers, std::size(numbers));
    InsertionSort::sort_descending(vector);
    print("Counting sort, descending integer array", numbers);
    print("Insertion sort, descending integer vector", vector);
}

void sortFromInput() {
    std::cout << "Enter integers on one line: ";
    std::string line;
    if (!std::getline(std::cin, line)) return;
    std::istringstream tokens(line);
    std::vector<int> collected;
    for (std::string word; tokens >> word;) {
        std::istringstream token(word);
        int number;
        char extra;
        if (!(token >> number) || token >> extra) {
            std::cout << "Invalid integer: " << word << '\n';
            return;
        }
        collected.push_back(number);
    }
    auto inserted = collected;
    InsertionSort::sort(inserted);
    CountingSort::sort(collected);
    print("Insertion sort result", inserted);
    print("Counting sort result", collected);
}

void sortingMenu() {
    while (true) {
        std::cout << "\nSorting\n1. Sort students\n2. Sort sample integers and compare with std::sort\n"
                     "3. Sort your own integers\n0. Back\n";
        const auto choice = readInt("Sorting option: ");
        if (!choice || *choice == 0) return;
        switch (*choice) {
            case 1: demonstrateStudents(); break;
            case 2: demonstrateIntegers(); break;
            case 3: sortFromInput(); break;
            default: std::cout << "Unknown sorting option.\n";
        }
    }
}

void showVertex(const Graph<int>& graph, int vertex) {
    std::cout << "Vertex " << vertex << " in-degree: " << graph.inDegree(vertex)
              << ", out-degree: " << graph.outDegree(vertex) << ", total: " << graph.degree(vertex) << '\n';
    std::cout << "Outgoing neighbors:";
    for (auto it = graph.cbeginAdjacentVertices(vertex); it != graph.cendAdjacentVertices(vertex); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << "\nIncident edges in reverse:";
    for (auto it = graph.crbeginIncidentEdges(vertex); it != graph.crendIncidentEdges(vertex); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << '\n';
}

void handleVertex(Graph<int>& graph, int choice, int vertex) {
    switch (choice) {
        case 1: std::cout << (graph.addVertex(vertex) ? "Vertex added.\n" : "Vertex already exists.\n"); break;
        case 3: graph.removeVertex(vertex); std::cout << "Vertex removed.\n"; break;
        case 5: showVertex(graph, vertex); break;
    }
}

void handleEdge(Graph<int>& graph, int choice, int source, int target) {
    switch (choice) {
        case 2: std::cout << (graph.addEdge(source, target) ? "Edge added.\n" : "Edge already exists.\n"); break;
        case 4: graph.removeEdge(source, target); std::cout << "Edge removed.\n"; break;
        case 6: {
            const auto degree = graph.edgeDegree(source, target);
            std::cout << "Edge degree: " << degree << '\n';
            break;
        }
    }
}

void graphMenu() {
    Graph<int> graph;
    while (true) {
        std::cout << "\nDirected adjacency-list graph\n1. Add vertex\n2. Add edge\n3. Remove vertex\n"
                     "4. Remove edge\n5. Inspect vertex\n6. Inspect edge degree\n7. Print graph\n"
                     "8. Copy and compare\n9. Clear graph\n0. Back\n";
        const auto choice = readInt("Graph option: ");
        if (!choice || *choice == 0) return;
        try {
            if (*choice >= 1 && *choice <= 6) {
                auto from = readInt(*choice % 2 != 0 ? "Vertex: " : "Source: ");
                if (!from) return;
                if (*choice % 2 != 0) {
                    handleVertex(graph, *choice, *from);
                } else {
                    auto to = readInt("Destination: ");
                    if (!to) return;
                    handleEdge(graph, *choice, *from, *to);
                }
            } else if (*choice == 7) {
                std::cout << graph.vertexCount() << " vertices, " << graph.edgeCount() << " edges\n" << graph << '\n';
            } else if (*choice == 8) {
                const Graph<int> copy(graph);
                std::cout << "Copy matches graph: " << std::boolalpha << (copy == graph) << '\n';
            } else if (*choice == 9) {
                graph.clear();
                std::cout << "Graph cleared.\n";
            } else {
                std::cout << "Unknown graph option.\n";
            }
        } catch (const GraphException& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }
}

} // namespace

void ConsoleMenu::run() {
    while (true) {
        std::cout << "\nLab 4, variant 6\n1. Sorting\n2. Graph\n0. Exit\n";
        const auto choice = readInt("Main option: ");
        if (!choice || *choice == 0) return;
        switch (*choice) {
            case 1: sortingMenu(); break;
            case 2: graphMenu(); break;
            default: std::cout << "Unknown main option.\n";
        }
        if (!std::cin) return;
    }
}

} // namespace cli
