#include "../include/graph.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

Head *first_head_ptr = nullptr;
Head *last_head_ptr = nullptr;

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Head *createNode(string id) {
  Head *new_node = new Head();
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
Element *createElement(Head *ptr) {
  Element *new_node = new Element();
  new_node->next_adjent = nullptr;
  new_node->my_head = ptr;
  return new_node;
}

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Head *firstNode() {
  Head *first_node = first_head_ptr;
  return first_node;
}

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param node указатель на узел
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(Head *node) {
  bool found = false;
  Head *next;

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
vector<Head *> adjentNodes(Head *head) {
  vector<Head *> nodes;
  Element *adjent = head->adjency_list_head;

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
void connect(Head *node, vector<Head *> to_another_nodes) {
  Element *adjent = nullptr;

  for (Head *to_node : to_another_nodes) {
    if (alreadyInGraph(to_node)) {
      adjent = createElement(to_node);
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
Element *getPreviousElement(Head *node, Element *elem) {
  Element *next, *previous;
  next = node->adjency_list_head;

  while (next and next != elem) {
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
Head *getPreviousHead(Head *node) {
  Head *next, *previous;
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
void addNode(Head *node) {
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
}

/*
 * @brief Удалить список смежности
 * @param head заголовок этого списка
 */
void deleteAdjentyList(Head *head) {
  Element *next = head->adjency_list_head;
  Element *current;

  while (next) {
    current = next;
    next = next->next_adjent;
    delete current;
  }
  head->adjency_list_head = nullptr;
  head->adjency_list_tail = nullptr;
}

/*
 * @brief Удалить элемент списка смежности
 * @param head узел графа из которого можно попасть в elem
 * @param elem элемент списка смежности
 */
void deleteElement(Head *head, Element *elem) {
  // если только один смежный узел
  if (head->adjency_list_head == elem and head->adjency_list_tail == elem) {
    head->adjency_list_head = nullptr;
    head->adjency_list_tail = nullptr;
  }
  // если первый, но не единственный смежный узел в списке смежности
  else if (head->adjency_list_head == elem and
           head->adjency_list_tail != elem) {
    head->adjency_list_head = elem->next_adjent;
  }
  // если не первый, не последний
  else if (head->adjency_list_head != elem and
           head->adjency_list_tail != elem) {
    Element *previous = getPreviousElement(head, elem);
    previous->next_adjent = elem->next_adjent;
  }
  // если последний, но не единственный
  else if (head->adjency_list_head != elem and
           head->adjency_list_tail == elem) {
    Element *previous = getPreviousElement(head, elem);
    previous->next_adjent = nullptr;
    head->adjency_list_tail = previous;
  }
  delete elem;
}

/*
 * @brief Удалить элемент списка заголовков
 * @param head заголовок
 */
void deleteHead(Head *head) {
  // если единственный
  if (first_head_ptr == head and last_head_ptr == head) {
    first_head_ptr = nullptr;
    last_head_ptr = nullptr;
  }
  // если первый, но не единственный
  else if (first_head_ptr == head and last_head_ptr != head) {
    first_head_ptr = head->next_head;
  }
  // если не первый и не последний
  else if (first_head_ptr != head and last_head_ptr != head) {
    Head *previous = getPreviousHead(head);
    previous->next_head = head->next_head;
  }
  // если последний, но не единственный
  else if (first_head_ptr != head and last_head_ptr == head) {
    Head *previous = getPreviousHead(head);
    previous->next_head = nullptr;
    last_head_ptr = previous;
  }

  delete head;
}

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param element Указатель на удаляемый узел
 */
void deleteNode(Head *node) {
  Head *next_head, *current_head;
  Element *next_element, *current_element;
  bool deleted = false;

  while (next_head and (not deleted)) {

    while (next_element) {
      if (next_element->my_head == node) {
        deleteElement(next_head, next_element);
      }
    }
    if (next_head == node) {
      deleteAdjentyList(next_head);
      deleteHead(next_head);
      deleted = true;
    }
  }
}

/**
 * @brief Полностью очищает граф, удаляя все узлы
 */
void clearGraph() {
  Head *next_head, *current_head;
  Element *next_element, *current_element;

  while (next_head) {

    while (next_element) {
      current_element = next_element;
      next_element = next_element->next_adjent;
      delete current_element;
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
