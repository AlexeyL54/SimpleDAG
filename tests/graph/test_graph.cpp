#include "../../include/graph.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <string>
#include <vector>

using std::find;
using std::string;
using std::vector;

// ===========================================================================
// ВСПОМОГАТЕЛЬНЫЕ УТИЛИТЫ ДЛЯ ТЕСТИРОВАНИЯ DFS
// ===========================================================================
namespace dfs_test_utils {
vector<string> visited_nodes;

void resetVisited() { visited_nodes.clear(); }

void recordVisit(Node *node) { visited_nodes.push_back(node->id); }

bool wasVisited(const string &node_id) {
  return find(visited_nodes.begin(), visited_nodes.end(), node_id) !=
         visited_nodes.end();
}

int visitCount() { return visited_nodes.size(); }

string firstVisited() { return visited_nodes.empty() ? "" : visited_nodes[0]; }
} // namespace dfs_test_utils

// ===========================================================================
// ТЕСТЫ СОЗДАНИЯ И БАЗОВЫХ ОПЕРАЦИЙ
// ===========================================================================

// Тест создания отдельного узла
TEST(GRAPH, CreateNode) {
  Node *node = createNode("A");

  EXPECT_EQ(node->id, "A");
  EXPECT_EQ(node->next_head, nullptr);
  EXPECT_EQ(node->adjency_list_head, nullptr);
  EXPECT_EQ(node->adjency_list_tail, nullptr);

  delete node;
}

// Тест добавления узлов в граф
TEST(GRAPH, AddNode) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  addNode(node_a);
  addNode(node_b);

  EXPECT_EQ(firstNode(), node_a);
  EXPECT_EQ(nodesTotal(), 2);
  EXPECT_EQ(node_a->next_head, node_b);

  clearGraph();
}

// Тест очистки пустого графа
TEST(GRAPH, ClearGraphWithoutConnections) {
  Node *node = createNode("A");
  addNode(node);
  clearGraph();

  EXPECT_EQ(firstNode(), nullptr);
  EXPECT_EQ(nodesTotal(), 0);
}

// ===========================================================================
// ТЕСТЫ СОЕДИНЕНИЙ И СМЕЖНОСТИ
// ===========================================================================

// Тест создания связей между узлами
TEST(GRAPH, ConnectNodes) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  vector<Node *> to_another_nodes = {node_b, node_c};

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  connect(node_a, to_another_nodes);

  EXPECT_EQ(node_a->adjency_list_head->my_head, node_b);
  EXPECT_EQ(node_a->adjency_list_head->next_adjent->my_head, node_c);
  EXPECT_EQ(node_a->adjency_list_tail->my_head, node_c);

  clearGraph();
}

// Тест создания связей между узлами по id
TEST(GRAPH, ConnectNodesById) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  vector<string> to_another_nodes = {"B", "C"};

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  connect("A", to_another_nodes);

  EXPECT_EQ(node_a->adjency_list_head->my_head, node_b);
  EXPECT_EQ(node_a->adjency_list_head->next_adjent->my_head, node_c);
  EXPECT_EQ(node_a->adjency_list_tail->my_head, node_c);

  clearGraph();
}

// Тест получения списка смежных узлов
TEST(GRAPH, GetAdjentNodes) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  vector<Node *> to_another_nodes = {node_b, node_c};

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  connect(node_a, to_another_nodes);
  vector<Node *> adjent_nodes = adjentNodes(node_a);

  EXPECT_EQ(adjent_nodes, to_another_nodes);

  clearGraph();
}

// Тест очистки графа со связями
TEST(GRAPH, ClearGraphWithConnections) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  vector<Node *> to_another_nodes = {node_b, node_c};

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  connect(node_a, to_another_nodes);
  clearGraph();

  EXPECT_EQ(firstNode(), nullptr);
  EXPECT_EQ(nodesTotal(), 0);
}

// ===========================================================================
// ТЕСТЫ ПРОВЕРОК СОСТОЯНИЯ ГРАФА
// ===========================================================================

// Тест проверки наличия узла в графе по указателю
TEST(GRAPH, AlreadyInGraph) {
  Node *node = createNode("A");
  addNode(node);

  EXPECT_EQ(alreadyInGraph(node), true);

  clearGraph();
}

// Тест проверки наличия узла в графе по указателю
TEST(GRAPH, AlreadyInGraphById) {
  Node *a = createNode("A");
  Node *b = createNode("B");
  addNode(a);
  addNode(b);

  EXPECT_TRUE(alreadyInGraph("A"));
  EXPECT_TRUE(alreadyInGraph("B"));

  clearGraph();
}

// Тест получения указателя на узел по id
TEST(GRAPH, GetById) {
  Node *a = createNode("A");
  Node *b = createNode("B");
  addNode(a);
  addNode(b);

  EXPECT_EQ(getNodeById("A"), a);
  EXPECT_EQ(getNodeById("B"), b);

  clearGraph();
}

// Тест проверки пустоты графа
TEST(GRAPH, GraphIsEmpty) {
  Node *node = createNode("A");

  EXPECT_EQ(graphIsEmpty(), true);
  addNode(node);
  EXPECT_EQ(graphIsEmpty(), false);

  clearGraph();
}

// ===========================================================================
// ТЕСТЫ УДАЛЕНИЯ УЗЛОВ (СЛОЖНЫЕ СЦЕНАРИИ)
// ===========================================================================

// Тест удаления узлов без связей в различных позициях
TEST(GRAPH, DeleteNodeWithoutConnections) {
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  Node *node_d = createNode("D");

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  addNode(node_d);

  // Тест 1: удаление узла из середины (не последний и не первый)
  deleteNode(node_c);
  EXPECT_EQ(node_b->next_head, node_d);
  EXPECT_EQ(nodesTotal(), 3);

  // Тест 2: удаление последнего узла
  deleteNode(node_d);
  EXPECT_EQ(node_b->next_head, nullptr);
  EXPECT_EQ(lastNode(), node_b);
  EXPECT_EQ(node_b->next_head, nullptr);
  EXPECT_EQ(nodesTotal(), 2);

  // Тест 3: удаление первого узла
  deleteNode(node_a);
  EXPECT_EQ(firstNode(), node_b);
  EXPECT_EQ(lastNode(), node_b);
  EXPECT_EQ(node_b->next_head, nullptr);
  EXPECT_EQ(nodesTotal(), 1);

  // Тест 4: удаление единственного узла
  deleteNode(node_b);
  EXPECT_EQ(firstNode(), nullptr);
  EXPECT_EQ(lastNode(), nullptr);
  EXPECT_EQ(nodesTotal(), 0);

  clearGraph();
}

// Тест удаления узлов с активными связями
TEST(GRAPH, DeleteNodeWithConnections) {
  // Создание сложной структуры связей для тестирования
  Node *node_a = createNode("A");
  Node *node_b = createNode("B");
  Node *node_c = createNode("C");
  Node *node_d = createNode("D");
  Node *node_e = createNode("E");
  vector<Node *> to_nodes_b_c_d_e = {node_b, node_c, node_d, node_e};
  vector<Node *> to_nodes_d_e = {node_d, node_e};
  vector<Node *> adjent_nodes;

  addNode(node_a);
  addNode(node_b);
  addNode(node_c);
  addNode(node_d);
  addNode(node_e);
  connect(node_a, to_nodes_b_c_d_e);
  connect(node_b, to_nodes_d_e);

  // Тест: удаление узла из середины списка смежности
  deleteNode(node_d);
  adjent_nodes = {node_b, node_c, node_e};
  EXPECT_EQ(adjentNodes(node_a), adjent_nodes);
  adjent_nodes = {node_e};
  EXPECT_EQ(adjentNodes(node_b), adjent_nodes);

  // Тест: удаление последнего узла в списке смежности
  deleteNode(node_e);
  adjent_nodes = {node_b, node_c};
  EXPECT_EQ(adjentNodes(node_a), adjent_nodes);
  EXPECT_TRUE(adjentNodes(node_b).empty());
  EXPECT_EQ(node_b->adjency_list_head, nullptr);
  EXPECT_EQ(node_b->adjency_list_tail, nullptr);

  // Тест: удаление первого узла в списке смежности
  deleteNode(node_b);
  adjent_nodes = {node_c};
  EXPECT_EQ(adjentNodes(node_a), adjent_nodes);

  // Тест: удаление единственного узла в списке смежности
  deleteNode(node_c);
  EXPECT_TRUE(adjentNodes(node_a).empty());
  EXPECT_EQ(node_a->adjency_list_head, nullptr);
  EXPECT_EQ(node_a->adjency_list_tail, nullptr);

  clearGraph();
}

// ===========================================================================
// ТЕСТЫ ПОИСКА В ГЛУБИНУ (DFS)
// ===========================================================================

// Тест посещения всех узлов при обходе
TEST(GRAPH, DeepFirstSearch_VisitsAllNodes) {
  Node *a = createNode("A");
  Node *b = createNode("B");
  Node *c = createNode("C");

  addNode(a);
  addNode(b);
  addNode(c);
  connect(a, {b, c});

  dfs_test_utils::resetVisited();
  deepFirstSearch(dfs_test_utils::recordVisit);

  EXPECT_EQ(dfs_test_utils::visitCount(), 3);
  EXPECT_TRUE(dfs_test_utils::wasVisited("A"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("B"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("C"));

  clearGraph();
}

// Тест начала обхода с корневого узла
TEST(GRAPH, DeepFirstSearch_StartsFromRoot) {
  Node *a = createNode("A");
  Node *b = createNode("B");

  addNode(a);
  addNode(b);
  connect(a, {b});

  dfs_test_utils::resetVisited();
  deepFirstSearch(dfs_test_utils::recordVisit);

  EXPECT_EQ(dfs_test_utils::firstVisited(), "A");

  clearGraph();
}

// Тест обхода пустого графа
TEST(GRAPH, DeepFirstSearch_EmptyGraph) {
  dfs_test_utils::resetVisited();
  deepFirstSearch(dfs_test_utils::recordVisit);

  EXPECT_EQ(dfs_test_utils::visitCount(), 0);
}

// Тест обхода графа с одним узлом
TEST(GRAPH, DeepFirstSearch_SingleNode) {
  Node *a = createNode("A");
  addNode(a);

  dfs_test_utils::resetVisited();
  deepFirstSearch(dfs_test_utils::recordVisit);

  EXPECT_EQ(dfs_test_utils::visitCount(), 1);
  EXPECT_TRUE(dfs_test_utils::wasVisited("A"));

  clearGraph();
}

// Тест обхода сложного графа с множественными связями
TEST(GRAPH, DeepFirstSearch_ComplexGraph) {
  // Создание сложного графа для тестирования обхода:
  // A -> B -> C
  // |    |
  // v    v
  // D -> E
  Node *a = createNode("A");
  Node *b = createNode("B");
  Node *c = createNode("C");
  Node *d = createNode("D");
  Node *e = createNode("E");

  addNode(a);
  addNode(b);
  addNode(c);
  addNode(d);
  addNode(e);

  connect(a, {b, d});
  connect(b, {c, e});
  connect(d, {e});

  dfs_test_utils::resetVisited();
  deepFirstSearch(dfs_test_utils::recordVisit);

  EXPECT_EQ(dfs_test_utils::visitCount(), 5);
  EXPECT_TRUE(dfs_test_utils::wasVisited("A"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("B"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("C"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("D"));
  EXPECT_TRUE(dfs_test_utils::wasVisited("E"));
  EXPECT_EQ(dfs_test_utils::firstVisited(), "A");

  clearGraph();
}
