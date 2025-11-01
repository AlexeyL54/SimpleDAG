#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Element;
struct Head;

/**
 * @brief Структура элемента списка заголовков
 */
struct Head {
  string id;
  Element *adjency_list_head;
  Element *adjency_list_tail;
  Head *next_head; // Указатель на следующую вершину в списке заголовков
};

/**
 * @brief Структура вершины графа
 */
struct Element {
  Head *my_head;        // Указатель на заголовок этой вершины
  Element *next_adjent; // Указатель на следующий узел
};

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Head *createNode(string id);

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Head *firstNode();

/**
 * @brief Получает вектор всех узлов, следующих за указанным узлом
 * @param node Указатель на исходный узел
 * @return Вектор указателей на следующие узлы
 */
vector<Head *> adjentNodes(Head *);

/**
 * @brief Добавляет новый узел в граф, связывая его с существующим узлом
 * @param atom Указатель на добавляемый узел
 * @param to Указатель на узел назначения (по умолчанию - корневой узел)
 */
void addNode(Head *node, Head *to);

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param element Указатель на удаляемый узел
 */
void deleteNode(Head *element);

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
bool graphIsEmpty();

#endif // !GRAPH_H
