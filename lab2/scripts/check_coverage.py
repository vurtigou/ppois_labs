"""Check executable-line coverage for versioned C++ application source."""
import gzip
import json
from collections import defaultdict
from pathlib import Path
import subprocess
import sys
import tempfile


def main(gcov, build_dir):
    project = Path.cwd()
    build = Path(build_dir).resolve()
    app_notes = [note for part in ("src", "app") for note in (build / part).glob("*.gcno")]
    test_notes = list((build / "tests").glob("*.gcno"))
    expected = {file.relative_to(project) for part in ("src", "app")
                for file in (project / part).glob("*.cpp")}
    if not expected or len(app_notes) != len(expected) or not test_notes:
        raise SystemExit("Missing application coverage instrumentation.")

    hits = defaultdict(dict)
    with tempfile.TemporaryDirectory() as temp:
        subprocess.run([gcov, "--json-format", *map(str, app_notes + test_notes)], cwd=temp,
                       check=True, stdout=subprocess.DEVNULL)
        for report in Path(temp).glob("*.gcov.json.gz"):
            with gzip.open(report, "rt") as stream:
                data = json.load(stream)
            for entry in data["files"]:
                file = (Path(data["current_working_directory"]) / entry["file"]).resolve()
                if not file.is_relative_to(project):
                    continue
                relative = file.relative_to(project)
                if relative.parts[0] not in ("src", "include", "app"):
                    continue
                for line in entry["lines"]:
                    number = line["line_number"]
                    hits[relative][number] = hits[relative].get(number, 0) + line["count"]

    if not expected <= hits.keys():
        raise SystemExit("Coverage report is missing one or more application sources.")
    total = covered = 0
    for file, lines in sorted(hits.items()):
        executed = sum(count > 0 for count in lines.values())
        covered += executed
        total += len(lines)
        print(f"{file!s:25} {executed:4}/{len(lines):4} "
              f"({100 * executed / len(lines):6.2f}%)")
    print(f"Total: {covered}/{total} executable lines ({100 * covered / total:.2f}%)")
    return 0 if covered * 100 > total * 90 else 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:]))
