#ifndef UTILS_H
#define UTILS_H

#include "../include/graph.h"
#include "../include/operations.h"
#include "../include/parser.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

using std::fstream;
using std::ifstream;
using std::string;
using std::vector;

namespace logger {

/*
 * @brief Открыть лог
 * @param путь к файлу лога
 */
void openLog(string path);

/*
 * @brief Записать результат выполненя операции над числовым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, float res);

/*
 * @brief Записать результат выполненя операции над строковым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, string res);

/*
 * @brief Записать предупреждения
 * @param massage сообщение
 */
void warning(string message);

/*
 * @brief Закрыть лог
 */
void close();
}; // namespace logger

// =========================================================================

/*
 * @brief Получить путь к файлу конфигурации, путь к файлу результатов
 * выполнения операций
 * @param config путь к файлу конфигурации
 * @param res_file путь к файлу результатов выполнения операций
 */
void getSourceFiles(string &config, string &res_file);

/*
 * @brief Процедура запуска операции в узле графа
 * @param node указатель на узел графа
 */
void procedure(Node *node);

#endif // !UTILS_H
