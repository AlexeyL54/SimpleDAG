#include "../include/graph.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

Node *first_head_ptr = nullptr;
Node *last_head_ptr = nullptr;
int nodes_total = 0;

/**
 * @brief Получить количество узлов графа
 * @return количество узлов графа
 */
int nodesTotal() { return nodes_total; }

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Node *firstNode() { return first_head_ptr; }

/**
 * @brief Возвращает указатель на последний узел графа в списке заголовков
 * @return Указатель на последний узел в списке заголовков
 */
Node *lastNode() { return last_head_ptr; }

/**
 * @brief Проверяет, содержит ли граф какие-либо узлы
 * @return true если граф пуст, false если содержит узлы
 */
bool graphIsEmpty() { return first_head_ptr == nullptr; }

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Node *createNode(string id) {
  Node *new_node = new Node;
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
  Adjent *new_adjent = new Adjent;
  new_adjent->next_adjent = nullptr;
  new_adjent->my_head = node;
  return new_adjent;
}

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param node указатель на узел
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(Node *node) {
  bool found = false;
  Node *next = first_head_ptr;

  while (next and (not found)) {
    if (next == node)
      found = true;
    next = next->next_head;
  }
  return found;
}

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param id уникальный идентификатор узла
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(string id) {
  bool found;
  Node *next = first_head_ptr;

  while (next and (not found)) {
    if (next->id == id)
      found = true;
    next = next->next_head;
  }
  return found;
}

/*
 * @brief Получить узел по id
 * @param id уникальный идентификатор узла
 * @return указатель на узел, если найден, nullptr - если нет
 */
Node *getNodeById(string id) {
  bool found = false;
  Node *next = first_head_ptr;
  Node *node_by_id = nullptr;

  while (next and (not found)) {
    if (next->id == id) {
      found = true;
      node_by_id = next;
    }
    next = next->next_head;
  }
  return node_by_id;
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
 * @param to_another_nodes вектор уникальных идентификаторов узлов, в которые
 * нужно попасть из node
 */
void connect(Node *node, vector<Node *> to_another_nodes) {
  Adjent *adjent = nullptr;

  // Проверяем, что node добавлен в граф
  if (!alreadyInGraph(node)) {
    return;
  }

  for (Node *to_node : to_another_nodes) {

    if (alreadyInGraph(to_node)) {
      adjent = createAdjent(to_node);

      if (!node->adjency_list_head) {
        node->adjency_list_head = adjent;
        node->adjency_list_tail = adjent;
      } else {
        node->adjency_list_tail->next_adjent = adjent;
        node->adjency_list_tail = adjent;
      }
    }
  }
}

/*
 * @brief Связать узел с другими
 * @param id уникальный идентификатор узла
 * @param to_another_ids вектор уникальных идентификаторов узлов, в которые
 * нужно попасть из узла с идентификатором id
 */
void connect(string id, vector<string> to_another_ids) {
  Adjent *adjent = nullptr;
  Node *to_node;
  Node *node = getNodeById(id);

  // Проверяем, что node добавлен в граф
  if (!alreadyInGraph(id)) {
    return;
  }

  for (string to_id : to_another_ids) {

    if (alreadyInGraph(to_id)) {
      to_node = getNodeById(to_id);
      adjent = createAdjent(to_node);

      if (!node->adjency_list_head) {
        node->adjency_list_head = adjent;
        node->adjency_list_tail = adjent;
      } else {
        node->adjency_list_tail->next_adjent = adjent;
        node->adjency_list_tail = adjent;
      }
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
  if (!node || !adjent || node->adjency_list_head == adjent) {
    return nullptr;
  }

  Adjent *current = node->adjency_list_head;
  while (current && current->next_adjent != adjent) {
    current = current->next_adjent;
  }
  return current;
}

/*
 * @brief Получить предыдущий элемент списка смежности
 * @param node указатель на заголовок
 * @elem элемент списка смежности этого заголовка
 * @return предыдущий элемент списка смежности
 */
Node *getPreviousNodeInList(Node *node) {
  if (!first_head_ptr || first_head_ptr == node) {
    return nullptr;
  }

  Node *current = first_head_ptr;
  while (current && current->next_head != node) {
    current = current->next_head;
  }
  return current;
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
 * @param head узел графа, из которого можно попасть в adjent
 * @param adjent элемент списка смежности
 */
void deleteAdjent(Node *node, Adjent *adjent) {
  if (!node || !adjent)
    return;

  // Если это первый элемент в списке
  if (node->adjency_list_head == adjent) {
    node->adjency_list_head = adjent->next_adjent;
    // Если это также последний элемент
    if (node->adjency_list_tail == adjent) {
      node->adjency_list_tail = nullptr;
    }
  } else {
    // Ищем предыдущий элемент
    Adjent *prev = getPreviousAdjentInList(node, adjent);
    if (prev) {
      prev->next_adjent = adjent->next_adjent;
      // Если удаляем последний элемент
      if (node->adjency_list_tail == adjent) {
        node->adjency_list_tail = prev;
      }
    }
  }
  delete adjent;
}

/*
 * @brief Удалить элемент списка заголовков
 * @param head заголовок
 */
void deleteNodeFromList(Node *node) {
  if (!first_head_ptr)
    return;

  // Если это первый элемент
  if (first_head_ptr == node) {
    first_head_ptr = node->next_head;
    // Если это также последний элемент
    if (last_head_ptr == node) {
      last_head_ptr = nullptr;
    }
  } else {
    // Ищем предыдущий элемент
    Node *prev = getPreviousNodeInList(node);
    if (prev) {
      prev->next_head = node->next_head;
      // Если удаляем последний элемент
      if (last_head_ptr == node) {
        last_head_ptr = prev;
      }
    }
  }
}

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param element Указатель на удаляемый узел
 */
void deleteNode(Node *node) {
  if (!node || !alreadyInGraph(node))
    return;

  // Удаляем все связи, ведущие к этому узлу из других узлов
  Node *current_head = first_head_ptr;
  while (current_head) {
    if (current_head != node) {
      Adjent *current_adjent = current_head->adjency_list_head;
      Adjent *prev_adjent = nullptr;

      while (current_adjent) {
        if (current_adjent->my_head == node) {
          Adjent *to_delete = current_adjent;
          current_adjent = current_adjent->next_adjent;

          if (prev_adjent) {
            prev_adjent->next_adjent = current_adjent;
          } else {
            current_head->adjency_list_head = current_adjent;
          }

          // Обновляем tail если нужно
          if (current_head->adjency_list_tail == to_delete) {
            current_head->adjency_list_tail = prev_adjent;
          }

          delete to_delete;
        } else {
          prev_adjent = current_adjent;
          current_adjent = current_adjent->next_adjent;
        }
      }
    }
    current_head = current_head->next_head;
  }

  deleteAdjentyList(node);
  deleteNodeFromList(node);
  delete node;
  nodes_total--;
}

/**
 * @brief Полностью очищает граф, удаляя все узлы
 */
void clearGraph() {
  if (!first_head_ptr)
    return;

  Node *next_head = first_head_ptr;
  Node *current_head;
  Adjent *next_adjent = first_head_ptr->adjency_list_head;
  Adjent *current_adjent;

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
  first_head_ptr = nullptr;
  last_head_ptr = nullptr;
  nodes_total = 0;
}

/*
 * @brief Вспомогательная функция рекурсивного обхода графа
 * @param procedure указатель на функцию операции над узлом
 * @param node указатель на посещаемый узел
 * @param visited_nodes массив указателей на посещенные узлы
 * @param last_visited индекс последнего посещенного узла в массиве (изначально
 * -1)
 */
void deepFirstSearchRecursive(void (*procedure)(Node *node), Node *node,
                              Node **visited_nodes, int *last_visited) {
  Adjent *next = node->adjency_list_head;

  for (int i = 0; i <= *last_visited; i++) {
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
void deepFirstSearch(void (*procedure)(Node *node)) {
  if (!first_head_ptr)
    return;

  Node **visited_nodes = new Node *[nodes_total];
  int last_visited = -1;

  deepFirstSearchRecursive(procedure, first_head_ptr, visited_nodes,
                           &last_visited);

  delete[] visited_nodes;
}
