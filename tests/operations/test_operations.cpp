#include "../include/operations.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

TEST(OPERATIONS, Sum) {
  vector<float> source = {1, 2, 3};
  EXPECT_EQ(sum(source), 6);
}

TEST(OPERATIONS, Average) {
  vector<float> source = {1, 2, 3};
  EXPECT_EQ(average(source), 2);
}

TEST(OPERATIONS, Concatinate) {
  vector<string> source = {"a", "bc", "def"};
  EXPECT_EQ(concatinate(source), "abcdef");
}

TEST(OPERATIONS, SumFromMap) {
  vector<float> source = {1, 2, 3};
  EXPECT_EQ(callOperation("sum", source), 6);
}

TEST(OPERATIONS, AverageFromMap) {
  vector<float> source = {1, 2, 3};
  EXPECT_EQ(callOperation("average", source), 2);
}

TEST(OPERATIONS, ConcatinateFromMap) {
  vector<string> source = {"a", "bc", "def"};
  EXPECT_EQ(callOperation("concatinate", source), "abcdef");
}
