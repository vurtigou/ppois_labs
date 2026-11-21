CXX ?= c++
GCOV ?= gcov
DOXYGEN ?= doxygen
PYTHON ?= python3
BUILD_DIR ?= build
CPPFLAGS += -Iinclude
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic

DOMAIN_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(wildcard src/*.cpp))
TEST_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(wildcard tests/*.cpp))
APP_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(wildcard app/*.cpp))
OBJECTS := $(DOMAIN_OBJECTS) $(TEST_OBJECTS) $(APP_OBJECTS)
COVERAGE_DIR := $(BUILD_DIR)/coverage

.PHONY: all test coverage docs clean
all: $(BUILD_DIR)/oceanarium

$(BUILD_DIR)/oceanarium: $(DOMAIN_OBJECTS) $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/oceanarium_tests: $(DOMAIN_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

test: $(BUILD_DIR)/oceanarium_tests $(BUILD_DIR)/oceanarium
	./$(BUILD_DIR)/oceanarium_tests
	sh tests/test_cli.sh ./$(BUILD_DIR)/oceanarium

coverage:
	$(MAKE) BUILD_DIR=$(COVERAGE_DIR) CXXFLAGS="-O0 -g -std=c++17 -Wall -Wextra -Wpedantic --coverage" coverage-test

.PHONY: coverage-test
coverage-test: $(BUILD_DIR)/oceanarium_tests $(BUILD_DIR)/oceanarium
	find $(BUILD_DIR) -name '*.gcda' -delete
	./$(BUILD_DIR)/oceanarium_tests
	sh tests/test_cli.sh ./$(BUILD_DIR)/oceanarium
	$(PYTHON) scripts/check_coverage.py "$(GCOV)" "$(BUILD_DIR)"

docs:
	$(DOXYGEN) Doxyfile

clean:
	rm -rf $(BUILD_DIR)

-include $(OBJECTS:.o=.d)
