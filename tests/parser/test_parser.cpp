#include "../../include/graph.h"
#include "../../include/parser.h"
#include "../../libs/csv-parser/include/csv.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// Тест разделителя строки
TEST(PARSER, Split) {
  vector<string> splited = split("A-->B", "-->");

  EXPECT_EQ(splited[0], "A");
  EXPECT_EQ(splited[1], "B");
}

// Тест составления графа по схеме
TEST(PARSER, CreateGraphFromScheme) {
  vector<string> scheme = {"A-->B-->C", "B-->D-->C"};

  createGraphFromScheme(scheme);
  Node *a = getNodeById("A");
  Node *b = getNodeById("B");
  Node *c = getNodeById("C");
  Node *d = getNodeById("D");
  vector<Node *> b_adjents = adjentNodes(b);

  EXPECT_EQ(a->adjency_list_head->my_head, b);
  EXPECT_EQ(b_adjents[0], c);
  EXPECT_EQ(b_adjents[1], d);
  EXPECT_EQ(d->adjency_list_head->my_head, c);

  clearGraph();
}
