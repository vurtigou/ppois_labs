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

std::optional<int> readInt(std::istream& input, std::ostream& output, const char* prompt) {
    for (std::string line; output << prompt && std::getline(input, line);) {
        std::istringstream token(line);
        int number;
        char extra;
        if (token >> number && !(token >> extra)) return number;
        output << "Enter one integer.\n";
    }
    return std::nullopt;
}

template <class Range>
void print(std::ostream& output, const char* label, const Range& range) {
    output << label << '\n';
    for (const auto& item : range) output << "  " << item << '\n';
}

void demonstrateStudents(std::ostream& output) {
    Student students[] = {
        {104, "Ivan Petrov", "CS-21", 4.65, 19},
        {101, "Anna Smirnova", "CS-21", 4.90, 18},
        {107, "Dmitry Volkov", "CS-22", 3.80, 20},
        {102, "Elena Kuznetsova", "CS-22", 4.65, 19},
    };
    std::vector<Student> vector(std::begin(students), std::end(students));
    InsertionSort::sort(students, std::size(students), Student::compareByName);
    CountingSort::sort(vector, &Student::getAge);
    print(output, "Insertion sort, student array by name", students);
    print(output, "Counting sort, student vector by age", vector);

    CountingSort::sort(students, std::size(students), &Student::getId);
    InsertionSort::sort(vector, Student::compareByGpa);
    print(output, "Counting sort, student array by ID", students);
    print(output, "Insertion sort, student vector by GPA", vector);
}

void demonstrateIntegers(std::ostream& output) {
    int numbers[] = {42, 7, -19, 3, 25, 19, 1, -14};
    std::vector<int> vector(std::begin(numbers), std::end(numbers));
    auto expected = vector;
    std::sort(expected.begin(), expected.end());
    InsertionSort::sort(numbers, std::size(numbers));
    CountingSort::sort(vector);
    print(output, "Sorted integer array", numbers);
    output << "Both algorithms agree with std::sort: " << std::boolalpha
              << (vector == expected && std::equal(std::begin(numbers), std::end(numbers), expected.begin())) << '\n';
    CountingSort::sort_descending(numbers, std::size(numbers));
    InsertionSort::sort_descending(vector);
    print(output, "Counting sort, descending integer array", numbers);
    print(output, "Insertion sort, descending integer vector", vector);
}

void sortFromInput(std::istream& input, std::ostream& output) {
    output << "Enter integers on one line: ";
    std::string line;
    if (!std::getline(input, line)) return;
    std::istringstream tokens(line);
    std::vector<int> collected;
    for (std::string word; tokens >> word;) {
        std::istringstream token(word);
        int number;
        char extra;
        if (!(token >> number) || token >> extra) {
            output << "Invalid integer: " << word << '\n';
            return;
        }
        collected.push_back(number);
    }
    auto inserted = collected;
    InsertionSort::sort(inserted);
    CountingSort::sort(collected);
    print(output, "Insertion sort result", inserted);
    print(output, "Counting sort result", collected);
}

void sortingMenu(std::istream& input, std::ostream& output) {
    while (true) {
        output << "\nSorting\n1. Sort students\n2. Sort sample integers and compare with std::sort\n"
                     "3. Sort your own integers\n0. Back\n";
        const auto choice = readInt(input, output, "Sorting option: ");
        if (!choice || *choice == 0) return;
        switch (*choice) {
            case 1: demonstrateStudents(output); break;
            case 2: demonstrateIntegers(output); break;
            case 3: sortFromInput(input, output); break;
            default: output << "Unknown sorting option.\n";
        }
    }
}

void showVertex(std::ostream& output, const Graph<int>& graph, int vertex) {
    output << "Vertex " << vertex << " in-degree: " << graph.inDegree(vertex)
              << ", out-degree: " << graph.outDegree(vertex) << ", total: " << graph.degree(vertex) << '\n';
    output << "Outgoing neighbors:";
    for (auto it = graph.cbeginAdjacentVertices(vertex); it != graph.cendAdjacentVertices(vertex); ++it) {
        output << ' ' << *it;
    }
    output << "\nIncident edges in reverse:";
    for (auto it = graph.crbeginIncidentEdges(vertex); it != graph.crendIncidentEdges(vertex); ++it) {
        output << ' ' << *it;
    }
    output << '\n';
}

void handleVertex(std::ostream& output, Graph<int>& graph, int choice, int vertex) {
    switch (choice) {
        case 1: output << (graph.addVertex(vertex) ? "Vertex added.\n" : "Vertex already exists.\n"); break;
        case 3: graph.removeVertex(vertex); output << "Vertex removed.\n"; break;
        case 5: showVertex(output, graph, vertex); break;
    }
}

void handleEdge(std::ostream& output, Graph<int>& graph, int choice, int source, int target) {
    switch (choice) {
        case 2: output << (graph.addEdge(source, target) ? "Edge added.\n" : "Edge already exists.\n"); break;
        case 4: graph.removeEdge(source, target); output << "Edge removed.\n"; break;
        case 6: {
            const auto degree = graph.edgeDegree(source, target);
            output << "Edge degree: " << degree << '\n';
            break;
        }
    }
}

void graphMenu(std::istream& input, std::ostream& output) {
    Graph<int> graph;
    while (true) {
        output << "\nDirected adjacency-list graph\n1. Add vertex\n2. Add edge\n3. Remove vertex\n"
                     "4. Remove edge\n5. Inspect vertex\n6. Inspect edge degree\n7. Print graph\n"
                     "8. Copy and compare\n9. Clear graph\n0. Back\n";
        const auto choice = readInt(input, output, "Graph option: ");
        if (!choice || *choice == 0) return;
        try {
            if (*choice >= 1 && *choice <= 6) {
                auto from = readInt(input, output, *choice % 2 != 0 ? "Vertex: " : "Source: ");
                if (!from) return;
                if (*choice % 2 != 0) {
                    handleVertex(output, graph, *choice, *from);
                } else {
                    auto to = readInt(input, output, "Destination: ");
                    if (!to) return;
                    handleEdge(output, graph, *choice, *from, *to);
                }
            } else if (*choice == 7) {
                output << graph.vertexCount() << " vertices, " << graph.edgeCount() << " edges\n" << graph << '\n';
            } else if (*choice == 8) {
                const Graph<int> copy(graph);
                output << "Copy matches graph: " << std::boolalpha << (copy == graph) << '\n';
            } else if (*choice == 9) {
                graph.clear();
                output << "Graph cleared.\n";
            } else {
                output << "Unknown graph option.\n";
            }
        } catch (const GraphException& error) {
            output << "Error: " << error.what() << '\n';
        }
    }
}

} // namespace

void ConsoleMenu::run() {
    run(std::cin, std::cout);
}

void ConsoleMenu::run(std::istream& input, std::ostream& output) {
    while (true) {
        output << "\nLab 4, variant 6\n1. Sorting\n2. Graph\n0. Exit\n";
        const auto choice = readInt(input, output, "Main option: ");
        if (!choice || *choice == 0) return;
        switch (*choice) {
            case 1: sortingMenu(input, output); break;
            case 2: graphMenu(input, output); break;
            default: output << "Unknown main option.\n";
        }
        if (!input) return;
    }
}

} // namespace cli
