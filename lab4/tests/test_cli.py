"""Integration tests for the actual lab4 executable."""

from pathlib import Path
import subprocess
import sys


def run(commands, *expected):
    result = subprocess.run([Path(sys.argv[1]).resolve()], input=commands, text=True,
                            capture_output=True, timeout=5, check=True)
    for message in expected:
        if message not in result.stdout:
            raise AssertionError(f"Missing {message!r} in:\n{result.stdout}")
    if result.stderr:
        raise AssertionError(result.stderr)


run("junk\n2 trailing\n9\n1\n9\n1\n2\n3\n7 -3 7 0\n3\n2 bad\n0\n0\n",
    "Enter one integer.", "Unknown main option.", "Unknown sorting option.",
    "Insertion sort, student array by name", "Counting sort, student vector by age",
    "Counting sort, student array by ID", "Insertion sort, student vector by GPA",
    "Both algorithms agree with std::sort: true",
    "Insertion sort result\n  -3\n  0\n  7\n  7",
    "Counting sort result\n  -3\n  0\n  7\n  7", "Invalid integer: bad")

run("2\n10\n1\n1\n1\n2\n1\n1\n2\n1\n2\n2\n1\n2\n2\n2\n1\n"
    "2\n1\n3\n5\n1\n5\n99\n6\n1\n2\n7\n8\n4\n2\n1\n4\n2\n1\n3\n1\n9\n7\n0\n0\n",
    "Unknown graph option.", "Vertex added.", "Vertex already exists.",
    "Edge added.", "Edge already exists.", "destination vertex does not exist",
    "Vertex 1 in-degree: 1, out-degree: 1, total: 2",
    "Outgoing neighbors: 2", "Incident edges in reverse:",
    "Error: Graph::inDegree: vertex does not exist",
    "Edge degree: 1", "2 vertices, 2 edges", "Copy matches graph: true",
    "Edge removed.", "edge does not exist", "Vertex removed.", "Graph cleared.", "0 vertices, 0 edges")

# EOF at each level exits instead of spinning, including incomplete operations.
for partial in ("", "1\n", "1\n3\n", "2\n", "2\n1\n", "2\n2\n1\n"):
    run(partial)
print("CLI integration tests passed.")
