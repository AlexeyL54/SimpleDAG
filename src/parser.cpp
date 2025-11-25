#include "../include/parser.h"
#include "../include/graph.h"
#include "../include/operations.h"
#include "../libs/Tiny_Yaml/yaml/yaml.hpp"
#include <cstdio>
#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

const string SYMBOLS = "0123456789->";

namespace config {

std::unique_ptr<TINY_YAML::Yaml> root;  // объект дерева конфигурации
const string OPERATIONS = "operations"; // поле доступных операций
const string SOURCE = "source";         // поле файлов csv для обработки
const string FUNC = "func";             // поле - название функции
const string COLUMN = "column";         // поле - номер столбца
const string PATH = "path";             // поле - путь к файлу csv для обработки

/*
 * @brief Загрузить конфигурацию
 * @param path путь к конфигурационному файлу
 */
void load(string path) {
  try {
    root = std::make_unique<TINY_YAML::Yaml>(path);
  } catch (const std::exception &e) {
    std::cerr << "Error loading config: " << e.what() << std::endl;
    root.reset();
  }
}

/*
 * Очистить объект конфигурации
 */
void clear() { root.reset(); }

/*
 * @brief Получить id всех доступных операций
 * @param вектор id операций
 */
std::vector<std::string> getIds() {
  std::vector<std::string> ids;

  if (!root) {
    std::cerr << "Config not loaded" << std::endl;
    return ids;
  }

  try {
    TINY_YAML::Node &operationsNode = (*root)[OPERATIONS];
    ids = operationsNode.getChildIds();

  } catch (const std::exception &e) {
    std::cerr << "Error getting operation IDs: " << e.what() << std::endl;
  }

  return ids;
}

/*
 * @brief Проверить наличие операций из конфигурации
 * @return словарь ненайденных функций, где ключ - id операции, значение -
 * функция
 */
map<string, string> checkFunctions() {
  map<string, string> unknown;
  string func;

  if (!root) {
    std::cerr << "Config not loaded" << std::endl;
    return unknown;
  }

  try {
    vector<string> ids = getIds();

    for (string id : ids) {
      func = getFuncById(id);

      if (operation_map.count(func) == 0) {
        unknown.insert({id, func});
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error checking functions: " << e.what() << std::endl;
  }

  return unknown;
};

/*
 * @brief Получить параметры (номера столбцов) по id
 * @param id уникальный идентификотор операции
 * @return номер столбца
 */
int getColumnById(string id) {
  if (!root) {
    std::cerr << "Config not loaded" << std::endl;
    return -1;
  }

  try {
    TINY_YAML::Node &operation = (*root)[OPERATIONS][id];

    if (!operation.hasChild(COLUMN)) {
      std::cerr << "Operation " << id << " has no column defined" << std::endl;
      return -1;
    }

    std::string columnStr = operation[COLUMN].getData<std::string>();
    return std::stoi(columnStr);

  } catch (const std::exception &e) {
    std::cerr << "Error getting column for operation " << id << ": " << e.what()
              << std::endl;
    return -1;
  }
}

/*
 * @brief Получить тип операции
 * @param id уникальный идентификатор операции
 * @return тип операции
 */
string getFuncById(string id) {
  if (!root) {
    std::cerr << "Config not loaded" << std::endl;
    return "";
  }

  try {
    TINY_YAML::Node &operation = (*root)[OPERATIONS][id];

    if (!operation.hasChild(FUNC)) {
      std::cerr << "Operation " << id << " has no function defined"
                << std::endl;
      return "";
    }
    return operation[FUNC].getData<std::string>();

  } catch (const std::exception &e) {
    std::cerr << "Error getting function for operation " << id << ": "
              << e.what() << std::endl;
    return "";
  }
}

/*
 * @brief Получить путь к файлу csv для обработки
 * @return путь к файлу
 */
string getCSV() {
  if (!root) {
    std::cerr << "Config not loaded" << std::endl;
    return "";
  }

  try {
    TINY_YAML::Node &data = (*root)[PATH];
    return data.getData<string>();

  } catch (const std::exception &e) {
    std::cerr << "Error getting path to data" << std::endl;
    return "";
  }
}

}; // namespace config

// ======================================================================

namespace table {

vector<vector<string>> table;

/**
 * @brief Инициализировать объкт документа
 * @param path путь к документу
 */
void read(string path, char delimiter) {
  std::ifstream file(path);
  string buffer;
  vector<string> row;

  if (not file.is_open()) {
    std::cerr << "Failed to open file " << path << "in table::read"
              << std::endl;
    return;
  }

  while (std::getline(file, buffer)) {
    std::stringstream s(buffer);

    while (std::getline(s, buffer, delimiter)) {
      row.push_back(buffer);
    }

    table.push_back(row);
    row.clear();
  }
  file.close();
}

/**
 * @brief Очистить таблицу
 */
void clear() { table.clear(); }

/**
 * @brief Определить, является ли строка числом
 * @param s строка
 * @return true если строка является числом, false - иначе
 */
bool isNumneric(string &s) {
  try {
    size_t pos;
    std::stod(s, &pos);
    return pos == s.length();
  } catch (const std::invalid_argument &e) {
    return false;
  } catch (const std::out_of_range &e) {
    return false;
  }
}

/**
 * @brief Определяет тип данных столбца
 * @param reader указатель на объкт обработки файла csv
 * @param head Заголовок столбца
 * @return Тип данных столбца
 */
ColumnType getTypeOfColumn(int column) {
  bool has_numbers = false;
  bool has_strings = false;

  for (vector<string> &row : table) {

    if (column < row.size()) {

      string field = row[column];
      if (isNumneric(row[column])) {
        has_numbers = true;
      } else {
        has_strings = true;
      }
    }
    if (has_numbers and has_strings == false)
      return NUMERIC;
    if (has_strings)
      return STRING;
  }

  return UNKNOWN;
}

/*
 * @brief Считать столбец чисел из файла
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<float> readNumericColumn(int column) {
  vector<float> values;

  for (vector<string> &row : table) {
    if (column < row.size())
      values.push_back(stod(row[column]));
  }
  return values;
}

/*
 * @brief Считать столбец строк из файла
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<string> readStringColumn(int column) {
  vector<string> values;

  for (vector<string> &row : table) {
    if (column < row.size())
      values.push_back(row[column]);
  }
  return values;
}
}; // namespace table

// ======================================================================

/**
 * @brief Разделить строку на подстроки по разделителю
 * @param s строка
 * @param delimiter разделитель
 * @return вектор подстрок
 */
vector<string> split(const string &s, const string &delimiter) {
  vector<string> tokens;
  size_t start = 0;
  size_t end = s.find(delimiter);

  while (end != string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delimiter.length();
    end = s.find(delimiter, start);
  }
  // добавить последний токен или всю строку, если разделитель не найден
  tokens.push_back(s.substr(start));
  return tokens;
}

/*
 * @brief Подсчитать количество вхождений подстроки в строку
 * @param n1 строка
 * @param n2 подстрока
 * @return количество вхождений
 */
int countSubstrOccurrences(string str1, string str2) {
  int n1 = str1.length();
  int n2 = str2.length();

  // основное условие
  if (n1 == 0 || n1 < n2)
    return 0;

  // условие для выхода рекурсии
  // проверить первую подстроку
  if (str1.substr(0, n2).compare(str2) == 0)
    return countSubstrOccurrences(str1.substr(1), str2) + 1;

  // либо вернуть оставшийся индекс
  return countSubstrOccurrences(str1.substr(1), str2);
}

/*
 * @brief Считать схему графа из потокового ввода
 * @param is указатель на поток ввода @brief Считать схему графа из потокового
 * ввода
 */
vector<string> getScheme(std::istream &is) {
  vector<string> lines;
  string line;

  while (std::getline(is, line) && line != "end") {
    lines.push_back(line);
  }
  return lines;
}

vector<SchenmeError> checkScheme(vector<string> &scheme, vector<string> ids) {
  vector<SchenmeError> errors;
  vector<string> indexes;

  for (string s : scheme) {
    indexes = split(s, "->");

    // если разделитель в начале или в конце
    if (s.find("->") == 0 or s.find("->") == s.length() - 1)
      errors.push_back(UNEXPECTED_DELIMITER);

    // если количество разделителей не верно
    if (countSubstrOccurrences(s, "->") != indexes.size() - 1)
      errors.push_back(DELIMITER_MISMATCH);

    // если есть не цифры и не разделители
    if (s.find_first_not_of(SYMBOLS) == string::npos)
      errors.push_back(NOT_INDEX);
  }

  return errors;
}

/**
 * @brief Создать граф по схеме
 * @param scheme массив строк
 */
void createGraphFromScheme(vector<string> scheme) {
  Node *atom;
  vector<string> id;

  for (const string node : scheme) {
    id = split(node, "->");
    atom = createNode(id[0]);
    addNode(atom);

    for (size_t i = 1; i < id.size(); i++) {
      if (!alreadyInGraph(id[i])) {
        atom = createNode(id[i]);
        addNode(atom);
      }
      connect(id[i - 1], {id[i]});
    }
  }
}

/**
 * @brief Создать граф по схеме индексов id
 * @param scheme вектор строк с индексами id операций
 * @param ids вектор id операций
 */
void createGraphFromScheme(vector<string> scheme, vector<string> ids) {
  Node *atom;
  vector<string> indexes;

  for (const string node : scheme) {
    indexes = split(node, "->");
    atom = createNode(ids[std::stoi(indexes[0]) - 1]);
    addNode(atom);

    for (size_t i = 1; i < indexes.size(); i++) {
      if (!alreadyInGraph(ids[std::stoi(indexes[i]) - 1])) {
        atom = createNode(ids[std::stoi(indexes[i]) - 1]);
        addNode(atom);
      }
      connect(ids[std::stoi(indexes[i - 1]) - 1],
              {ids[std::stoi(indexes[i]) - 1]});
    }
  }
}
