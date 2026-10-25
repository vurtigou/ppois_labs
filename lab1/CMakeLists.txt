# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab1_OOP LANGUAGES CXX)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем папку с заголовочными файлами (.h)
include_directories(include)

# Если при сборке передан флаг -DENABLE_COVERAGE=ON, включаем покрытие
if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# ==========================================
# СБОРКА ПРОГРАММ С МЕНЮ (CLI)
# ==========================================

# 1. Меню для класса Set (лежит в папке app)
add_executable(set_app app/main_set.cpp src/Set.cpp)

# 2. Меню для класса Polynomial (лежит в папке app)
add_executable(poly_app app/main_polynomial.cpp src/Polynomial.cpp)

# ==========================================
# СБОРКА ЮНИТ-ТЕСТОВ
# ==========================================

# 3. Тесты для класса Set (лежат в папке tests)
add_executable(set_tests tests/tests_set.cpp src/Set.cpp)

# 4. Тесты для класса Polynomial (лежат в папке tests)
add_executable(poly_tests tests/tests_polynomial.cpp src/Polynomial.cpp)
