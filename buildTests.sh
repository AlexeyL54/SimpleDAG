#!/bin/bash

set -e

echo "Building all tests..."
echo "Current directory: $(pwd)"

# Конфигурация путей для разных ОС
if [[ "$OSTYPE" == "darwin"* ]]; then
  echo "Building on macOS"
  INCLUDE_FLAGS="-I../../include -I../../libs/Tiny_Yaml/yaml -I/opt/homebrew/include"
  LIB_FLAGS="-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"
  COMPILER="g++ -std=c++17"
else
  echo "Building on Linux"
  INCLUDE_FLAGS="-I../../include -I../../libs/Tiny_Yaml/yaml"
  LIB_FLAGS="-lgtest -lgtest_main -lpthread"
  COMPILER="g++ -std=c++17"
fi

# Функция сборки одного теста
build_test() {
  local test_name="$1"
  local source_files="$2"
  local test_dir="$3"

  echo "Building $test_name..."

  cd "tests/$test_dir"
  $COMPILER $source_files $INCLUDE_FLAGS $LIB_FLAGS -o "${test_name}.out"
  cd ../..

  if [ -f "tests/$test_dir/${test_name}.out" ]; then
    echo "✅ $test_name - SUCCESS"
  else
    echo "❌ $test_name - FAILED"
    exit 1
  fi
}

# Сборка всех тестов
echo "Starting test builds..."

build_test "test_operations" \
  "test_operations.cpp ../../src/operations.cpp" \
  "operations"

build_test "test_graph" \
  "test_graph.cpp ../../src/graph.cpp" \
  "graph"

build_test "test_parser" \
  "test_parser.cpp ../../src/graph.cpp ../../src/parser.cpp ../../src/operations.cpp ../../libs/Tiny_Yaml/yaml/yaml.cpp" \
  "parser"

build_test "test_utils" \
  "test_utils.cpp ../../src/graph.cpp ../../src/operations.cpp ../../src/utils.cpp ../../src/parser.cpp ../../libs/Tiny_Yaml/yaml/yaml.cpp" \
  "utils"

echo "All tests built successfully!"
