#include "../include/graph.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

Head *first_head_ptr = nullptr;

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Node *createNode(string id) {
  Node *new_node = new Node();
  new_node->id = id;
  new_node->ptr = nullptr;
  return new_node;
}

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Node *firstNode() {
  Node *first_node = first_head_ptr->node_ptr;
  return first_node;
}
