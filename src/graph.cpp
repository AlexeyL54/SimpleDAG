#include "../include/graph.h"
#include <cstddef>
#include <string>
#include <vector>

using std::string;
using std::vector;

Node *first_head_ptr = nullptr;
Node *last_head_ptr = nullptr;
int nodes_total = 0;

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Node *createNode(string id) {
  Node *new_node = new Node();
  new_node->id = id;
  new_node->next_head = nullptr;
  new_node->adjency_list_head = nullptr;
  new_node->adjency_list_tail = nullptr;
  return new_node;
}

/**
 * @brief Создает новый элемент списка смежности
 * @param ptr Указатель на заголовок, соответствующих этой вершине
 * @return Указатель на созданный узел
 */
Adjent *createAdjent(Node *node) {
  Adjent *new_adjent = new Adjent();
  new_adjent->next_adjent = nullptr;
  new_adjent->my_head = node;
  return new_adjent;
}

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Node *firstNode() {
  Node *first_node = first_head_ptr;
  return first_node;
}

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param node указатель на узел
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(Node *node) {
  bool found = false;
  Node *next;

  while (next and (not found)) {
    if (next == node)
      found = true;
    next = next->next_head;
  }
  return found;
}

/**
 * @brief Получает вектор всех узлов, следующих за указанным узлом
 * @param node Указатель на исходный узел
 * @return Вектор указателей на следующие узлы
 */
vector<Node *> adjentNodes(Node *node) {
  vector<Node *> nodes;
  Adjent *adjent = node->adjency_list_head;

  while (adjent) {
    nodes.push_back(adjent->my_head);
    adjent = adjent->next_adjent;
  }
  return nodes;
}

/*
 * @brief Связать узел с другими
 * @param node указатель на узел
 * @param adjents узлы, в которые нужно попасть из node
 */
void connect(Node *node, vector<Node *> to_another_nodes) {
  Adjent *adjent = nullptr;

  for (Node *to_node : to_another_nodes) {
    if (alreadyInGraph(to_node)) {
      adjent = createAdjent(to_node);
      node->adjency_list_tail->next_adjent = adjent;
      node->adjency_list_tail = adjent;
    }
  }
}

/*
 * @brief Получить предыдущий элемент списка смежности
 * @param node указатель на заголовок
 * @elem элемент списка смежности этого заголовка
 * @return предыдущий элемент списка смежности
 */
Adjent *getPreviousAdjentInList(Node *node, Adjent *adjent) {
  Adjent *next, *previous;
  next = node->adjency_list_head;

  while (next and next != adjent) {
    previous = next;
    next = next->next_adjent;
  }
  return previous;
}

/*
 * @brief Получить предыдущий элемент списка смежности
 * @param node указатель на заголовок
 * @elem элемент списка смежности этого заголовка
 * @return предыдущий элемент списка смежности
 */
Node *getPreviousNodeInList(Node *node) {
  Node *next, *previous;
  next = first_head_ptr;

  while (next and next != node) {
    previous = next;
    next = next->next_head;
  }
  return previous;
}

/**
 * @brief Добавляет новый узел в граф, не связывая его с другими узлами
 * @param atom Указатель на добавляемый узел
 * @param to Указатель на узел назначения (по умолчанию - корневой узел)
 */
void addNode(Node *node) {
  // если граф был пуст
  if (!first_head_ptr) {
    first_head_ptr = node;
    last_head_ptr = node;
  }
  // если в графе уже были узлы
  else {
    last_head_ptr->next_head = node;
    last_head_ptr = node;
  }
  nodes_total++;
}

/*
 * @brief Удалить список смежности
 * @param head заголовок этого списка
 */
void deleteAdjentyList(Node *node) {
  Adjent *next = node->adjency_list_head;
  Adjent *current;

  while (next) {
    current = next;
    next = next->next_adjent;
    delete current;
  }
  node->adjency_list_head = nullptr;
  node->adjency_list_tail = nullptr;
}

/*
 * @brief Удалить элемент списка смежности
 * @param head узел графа из которого можно попасть в adjent
 * @param adjent элемент списка смежности
 */
void deleteAdjent(Node *node, Adjent *adjent) {
  // если только один смежный узел
  if (node->adjency_list_head == adjent and node->adjency_list_tail == adjent) {
    node->adjency_list_head = nullptr;
    node->adjency_list_tail = nullptr;
  }
  // если первый, но не единственный смежный узел в списке смежности
  else if (node->adjency_list_head == adjent and
           node->adjency_list_tail != adjent) {
    node->adjency_list_head = adjent->next_adjent;
  }
  // если не первый, не последний
  else if (node->adjency_list_head != adjent and
           node->adjency_list_tail != adjent) {
    Adjent *previous = getPreviousAdjentInList(node, adjent);
    previous->next_adjent = adjent->next_adjent;
  }
  // если последний, но не единственный
  else if (node->adjency_list_head != adjent and
           node->adjency_list_tail == adjent) {
    Adjent *previous = getPreviousAdjentInList(node, adjent);
    previous->next_adjent = nullptr;
    node->adjency_list_tail = previous;
  }
  delete adjent;
}

/*
 * @brief Удалить элемент списка заголовков
 * @param head заголовок
 */
void deleteNodeFromList(Node *node) {
  // если единственный
  if (first_head_ptr == node and last_head_ptr == node) {
    first_head_ptr = nullptr;
    last_head_ptr = nullptr;
  }
  // если первый, но не единственный
  else if (first_head_ptr == node and last_head_ptr != node) {
    first_head_ptr = node->next_head;
  }
  // если не первый и не последний
  else if (first_head_ptr != node and last_head_ptr != node) {
    Node *previous = getPreviousNodeInList(node);
    previous->next_head = node->next_head;
  }
  // если последний, но не единственный
  else if (first_head_ptr != node and last_head_ptr == node) {
    Node *previous = getPreviousNodeInList(node);
    previous->next_head = nullptr;
    last_head_ptr = previous;
  }

  delete node;
}

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param element Указатель на удаляемый узел
 */
void deleteNode(Node *node) {
  Node *next_head;
  Adjent *next_adjent_in_list;

  while (next_head) {

    while (next_adjent_in_list) {
      if (next_adjent_in_list->my_head == node) {
        deleteAdjent(next_head, next_adjent_in_list);
      }
      next_adjent_in_list = next_adjent_in_list->next_adjent;
    }
    if (next_head == node) {
      deleteAdjentyList(next_head);
      deleteNodeFromList(next_head);
      nodes_total--;
      return;
    }
    next_head = next_head->next_head;
  }
}

/**
 * @brief Полностью очищает граф, удаляя все узлы
 */
void clearGraph() {
  Node *next_head, *current_head;
  Adjent *next_adjent, *current_adjent;

  while (next_head) {

    while (next_adjent) {
      current_adjent = next_adjent;
      next_adjent = next_adjent->next_adjent;
      delete current_adjent;
    }
    current_head = next_head;
    next_head = next_head->next_head;
    delete current_head;
  }
}

/**
 * @brief Проверяет, содержит ли граф какие-либо узлы
 * @return true если граф пуст, false если содержит узлы
 */
bool graphIsEmpty() {
  bool empty = (first_head_ptr == nullptr);
  return empty;
}

/*
 * @brief Вспомогательная функция рекурсивного обхода графа
 * @param procedure указатель на функцию операции над узлом
 * @param node указатель на посещаемый узел
 * @param visited_nodes массив указателей на посещенные узлы
 * @param last_visited индекс последнего посещенного узла в массиве (изначально
 * -1)
 */
void deepFirstSearchRecursive(void *(procedure)(Node * node), Node *node,
                              Node **visited_nodes, size_t *last_visited) {
  Adjent *next = node->adjency_list_head;

  for (size_t i = 0; i <= *last_visited; i++) {
    if (visited_nodes[i] == node) {
      return;
    }
  }
  procedure(node);
  ++(*last_visited);
  visited_nodes[(*last_visited)] = node;

  while (next) {
    deepFirstSearchRecursive(procedure, next->my_head, visited_nodes,
                             last_visited);
    next = next->next_adjent;
  }
}

/**
 * @brief Пройти один раз по всем узлам графа
 * @param procedure указатель на функцию операции над узлом
 */
void deepFirstSearch(void *(procedure)(Node * node)) {
  static Node **visited_nodes = new Node *[nodes_total];
  size_t last_visited = -1;

  deepFirstSearchRecursive(procedure, first_head_ptr, visited_nodes,
                           &last_visited);

  delete[] visited_nodes;
}
