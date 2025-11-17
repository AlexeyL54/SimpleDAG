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
using std::string;
using std::vector;

const string SETUP = "setup.conf";

namespace logger {
fstream log;

/*
 * @brief Открыть лог
 * @param путь к файлу лога
 */
void openLog(string path) {
  fstream log(path);
  if (not log.is_open())
    std::cerr << "Failed to open log" << std::endl;
}

/*
 * @brief Записать результат выполненя операции над числовым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, float res) {
  log << id << " >> " << res << std::endl;
}

/*
 * @brief Записать результат выполненя операции над строковым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, string res) {
  log << id << " >> " << res << std::endl;
}

/*
 * @brief Записать предупреждения
 * @param massage сообщение
 */
void warning(string message) { log << message << std::endl; }

/*
 * @brief Закрыть лог
 */
void close() { log.close(); }
}; // namespace logger

// =========================================================================

/*
 * @brief Получить путь к файлу конфигурации, путь к файлу результатов
 * выполнения операций
 * @param config путь к файлу конфигурации
 * @param res_file путь к файлу результатов выполнения операций
 */
void getSourceFiles(string &config, string &res_file) {
  fstream file(SETUP);

  if (not file.is_open()) {
    std::cerr << "Failed to open setup file." << std::endl;
    return;
  }

  std::getline(file, config);
  std::getline(file, res_file);
}

/*
 * @brief Процедура запуска операции в узле графа
 * @param node указатель на узел графа
 */
void procedure(Node *node) {
  string id = node->id;
  string type = config::getOpTypeById(id);
  int column = config::getColumnById(id);

  switch (table::getTypeOfColumn(column)) {

  case NUMERIC: {
    vector<float> num_vec = table::readNumericColumn(column);
    logger::writeResult(id, callOperation(type, num_vec));
    break;
  }

  case STRING: {
    vector<string> str_vec = table::readStringColumn(column);
    logger::writeResult(id, callOperation(type, str_vec));
    break;
  }

  case MIX: {
    string message = "Different types found in column " +
                     std::to_string(column) + ". Skipping " + id;
    logger::warning(message);
  }

  case UNKNOWN: {
    string message = "Unknown type found in column " + std::to_string(column) +
                     ". Skipping " + id;
    logger::warning(message);
  }
  }
}
