#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Adjent;
struct Node;

/**
 * @brief Структура элемента списка заголовков
 */
struct Node {
  string id;
  Adjent *adjency_list_head;
  Adjent *adjency_list_tail;
  Node *next_head; // Указатель на следующую вершину в списке заголовков
};

/**
 * @brief Структура вершины графа
 */
struct Adjent {
  Node *my_head;       // Указатель на заголовок этой вершины
  Adjent *next_adjent; // Указатель на следующий узел
};

/**
 * @brief Создает новый узел графа с заданным идентификатором
 * @param id Уникальный идентификатор узла
 * @return Указатель на созданный узел
 */
Node *createNode(string id);

/**
 * @brief Создает новый элемент списка смежности
 * @param node Указатель на заголовок, соответствующий этой вершине
 * @return Указатель на созданный узел
 */
// Adjent *createAdjent(Node *node);

/**
 * @brief Получить количество узлов графа
 * @return количество узлов графа
 */
int nodesTotal();

/**
 * @brief Возвращает указатель на первый (корневой) узел графа
 * @return Указатель на корневой узел
 */
Node *firstNode();

/**
 * @brief Возвращает указатель на последний узел графа в списке заголовков
 * @return Указатель на последний узел в списке заголовков
 */
Node *lastNode();

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param node указатель на узел
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(Node *node);

/**
 * @brief Проверить, есть ли в графе определенный узел
 * @param id уникальный идентификатор узла
 * @return true, если узел найден, false - иначе
 */
bool alreadyInGraph(string id);

/*
 * @brief Получть узел по id
 * @param id уникальный идентификатор узла
 * @return указатель на узел, если найден, nullptr - если нет
 */
Node *getNodeById(string id);

/**
 * @brief Получает вектор всех узлов, следующих за указанным узлом
 * @param node Указатель на исходный узел
 * @return Вектор указателей на следующие узлы
 */
vector<Node *> adjentNodes(Node *node);

/**
 * @brief Связать узел с другими
 * @param node указатель на узел
 * @param to_another_nodes узлы, в которые нужно попасть из node
 */
void connect(Node *node, vector<Node *> to_another_nodes);

/*
 * @brief Связать узел с другими
 * @param id уникальный идентификатор узла
 * @param to_another_ids вектор кникальных идентификаторов узлов, в которые
 * нужно попасть из узла с идентификатором id
 */
void connect(string id, vector<string> to_another_ids);

/**
 * @brief Добавляет новый узел в граф, не связывая его с другими узлами
 * @param node Указатель на добавляемый узел
 */
void addNode(Node *node);

/**
 * @brief Удаляет узел из графа и все связанные с ним связи
 * @param node Указатель на удаляемый узел
 */
void deleteNode(Node *node);

/**
 * @brief Полностью очищает граф, удаляя все узлы
 */
void clearGraph();

/**
 * @brief Проверяет, содержит ли граф какие-либо узлы
 * @return true если граф пуст, false если содержит узлы
 */
bool graphIsEmpty();

/**
 * @brief Пройти один раз по всем узлам графа
 * @param procedure указатель на функцию операции над узлом
 */
void deepFirstSearch(void (*procedure)(Node *node));

#endif // !GRAPH_H
