#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief Структура вершины графа
 */
struct Element {
  string id;    // Уникальный идентификатор узла
  Element *ptr; // Указатель на следующий узел
};

typedef Element Node;

/**
 * @brief Структура элемента списка заголовков
 */
struct Head {
  Node *node_ptr;      // Указатель на вершину
  Head *next_head_ptr; // Указатель на следующую вершину в списке
};

// Глобальные указатели для управления графом
extern Node *first_node;     // Указатель на первый узел графа
extern Head *first_head_ptr; // Указатель на первый заголовок графа

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Node *createNode(string id);

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Node *firstNode();

/**
 * @brief Получает вектор всех узлов, следующих за указанным узлом
 * @param node Указатель на исходный узел
 * @return Вектор указателей на следующие узлы
 */
vector<Node *> nextNodes(Node *);

/**
 * @brief Добавляет новый узел в граф, связывая его с существующим узлом
 * @param atom Указатель на добавляемый узел
 * @param to Указатель на узел назначения (по умолчанию - корневой узел)
 */
void addNode(Node *atom, Node *to = first_node);

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param element Указатель на удаляемый узел
 */
void deleteNode(Node *element);

/**
 * @brief Проверяет граф на наличие циклов
 * @return true если граф ациклический, false если обнаружены циклы
 */
bool isGraphAcyclic();

/**
 * @brief Полностью очищает граф, удаляя все узлы
 */
void clearGraph();

/**
 * @brief Проверяет, содержит ли граф какие-либо узлы
 * @return true если граф пуст, false если содержит узлы
 */
bool isGraphEmpty();

#endif // !GRAPH_H
