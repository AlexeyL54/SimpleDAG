#!/bin/bash

set -e

echo "Building all tests..."
echo "Current directory: $(pwd)"
echo "Directory contents:"
ls -la

# Проверяем существование необходимых папок
echo "Checking required directories..."
[ -d "libs" ] && echo "✅ libs directory exists" || echo "❌ libs directory missing"
[ -d "libs/Tiny_Yaml" ] && echo "✅ Tiny_Yaml exists" || echo "❌ Tiny_Yaml missing"
[ -f "libs/Tiny_Yaml/yaml/yaml.hpp" ] && echo "✅ yaml.hpp exists" || echo "❌ yaml.hpp missing"

build_test() {
  local test_dir=$1
  local test_name=$2

  echo "Building $test_name..."
  echo "Entering tests/$test_dir"
  cd "tests/$test_dir"
  echo "Current dir: $(pwd)"
  echo "Running make..."
  make default
  cd ../..

  if [ -f "tests/$test_dir/${test_name}.out" ]; then
    echo "✅ $test_name built successfully"
  else
    echo "❌ Failed to build $test_name - executable not found"
    exit 1
  fi
}

# Сборка тестов
build_test "operations" "test_operations"
build_test "graph" "test_graph"
build_test "parser" "test_parser"
build_test "utils" "test_utils"

echo "All tests built successfully!"
