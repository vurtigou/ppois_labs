CXX ?= g++
GCOV ?= gcov
PYTHON ?= python3
DOXYGEN ?= doxygen
CPPCHECK ?= cppcheck
BUILD_DIR ?= build
CPPFLAGS += -Iinclude
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic

MODEL_SOURCES := src/graph/GraphException.cpp src/model/Student.cpp
MODEL_OBJECTS := $(MODEL_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
CLI_OBJECT := $(BUILD_DIR)/src/cli/ConsoleMenu.o
APP_OBJECTS := $(BUILD_DIR)/main.o $(CLI_OBJECT)
TEST_OBJECTS := $(BUILD_DIR)/tests/test_lab4.o
OBJECTS := $(MODEL_OBJECTS) $(APP_OBJECTS) $(TEST_OBJECTS)

.PHONY: all test coverage coverage-run docs lint sanitize clean
all: $(BUILD_DIR)/lab4

$(BUILD_DIR)/lab4: $(MODEL_OBJECTS) $(APP_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/unit_tests: $(MODEL_OBJECTS) $(CLI_OBJECT) $(TEST_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

test: $(BUILD_DIR)/unit_tests $(BUILD_DIR)/lab4
	$(BUILD_DIR)/unit_tests
	$(PYTHON) tests/test_cli.py $(BUILD_DIR)/lab4

coverage:
	$(MAKE) BUILD_DIR=$(BUILD_DIR)/coverage CXXFLAGS="-std=c++17 -O0 -g -Wall -Wextra -Wpedantic --coverage" LDFLAGS="--coverage" coverage-run

coverage-run: $(BUILD_DIR)/unit_tests $(BUILD_DIR)/lab4
	find $(BUILD_DIR) -name '*.gcda' -delete
	$(BUILD_DIR)/unit_tests
	$(PYTHON) scripts/check_coverage.py "$(GCOV)" "$(BUILD_DIR)" unit-coverage.txt
	$(PYTHON) tests/test_cli.py $(BUILD_DIR)/lab4
	$(PYTHON) scripts/check_coverage.py "$(GCOV)" "$(BUILD_DIR)" coverage.txt

sanitize:
	$(MAKE) BUILD_DIR=$(BUILD_DIR)/sanitizers CXXFLAGS="-std=c++17 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer" LDFLAGS="-fsanitize=address,undefined" test

docs:
	$(DOXYGEN) Doxyfile

lint:
	$(CPPCHECK) --enable=warning,performance,portability --error-exitcode=1 --std=c++17 -Iinclude main.cpp src tests

clean:
	rm -rf "$(BUILD_DIR)"

-include $(OBJECTS:.o=.d)
