"""Measure executable project lines and enforce the lab's coverage threshold."""

import gzip
import json
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    gcov, build = sys.argv[1:]
    root = Path.cwd()
    notes = sorted(Path(build).resolve().rglob("*.gcno"))
    sources = {Path("main.cpp"), *Path("src").rglob("*.cpp"),
               *Path("include").rglob("*.h"), *Path("include").rglob("*.tpp")}
    report_dir = Path(build)
    hits = {}
    if not notes:
        raise SystemExit("No GCC coverage notes found. Run make coverage.")

    # Separate gcov runs preserve reports even when object basenames coincide.
    with tempfile.TemporaryDirectory() as temp:
        for note in notes:
            subprocess.run([gcov, "--json-format", str(note)], cwd=temp, check=True,
                           stdout=subprocess.DEVNULL)
            for report in Path(temp).glob("*.gcov.json.gz"):
                with gzip.open(report, "rt") as stream:
                    data = json.load(stream)
                report.unlink()
                for file in data["files"]:
                    source = (Path(data["current_working_directory"]) / file["file"]).resolve()
                    if not source.is_relative_to(root):
                        continue
                    key = source.relative_to(root)
                    if key not in sources:
                        continue
                    for line in file["lines"]:
                        rows = hits.setdefault(key, {})
                        index = line["line_number"]
                        rows[index] = rows.get(index, 0) + line["count"]

    expected = {path for path in sources if path.suffix in (".cpp", ".tpp")}
    if not expected <= hits.keys():
        missing = ", ".join(str(path) for path in sorted(expected - hits.keys()))
        raise SystemExit(f"Missing instrumentation for: {missing}")

    covered = total = 0
    summary = []
    for name, lines in sorted(hits.items()):
        hit = sum(count > 0 for count in lines.values())
        covered += hit
        total += len(lines)
        summary.append(f"{str(name):54} {hit:4}/{len(lines):4}  {100 * hit / len(lines):6.2f}%")
    summary.append(f"Total line coverage: {covered}/{total} ({100 * covered / total:.2f}%)")
    text = "\n".join(summary) + "\n"
    (report_dir / "coverage.txt").write_text(text, encoding="utf-8")
    print(text, end="")
    if covered * 100 <= total * 90:
        raise SystemExit("Coverage must be strictly greater than 90%.")


if __name__ == "__main__":
    main()
