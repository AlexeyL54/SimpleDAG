#include "../include/parser.h"
#include "../include/graph.h"
// #include "../libs/csv-parser/include/csv.hpp"
#include "../libs/mini-yaml/yaml/Yaml.hpp"
#include <memory>
#include <string>
#include <vector>

using csv::CSVReader;
using csv::CSVRow;
using std::string;
using std::vector;

namespace config {
Yaml::Node root;

/*
 * @brief Загрузить конфигурацию
 * @param path путь к конфигурационному файлу
 */
void loadConfig(string path) { Yaml::Parse(root, path); }

/*
 * @brief Получить параметры (номера столбцов) по id
 * @param id уникальный идентификотор операции
 * @return номер столбца
 */
int getColumnById(string id) { return root[id]["column"].As<int>(); }

/*
 * @brief Получить тип операции
 * @param id уникальный идентификатор операции
 * @return тип операции
 */
string getOpTypeById(string id) { return root[id]["func"].As<string>(); }

}; // namespace config

// ======================================================================

namespace table {

std::unique_ptr<CSVReader> reader;

/**
 * @brief Инициализировать объкт документа
 * @param path путь к документу
 */
void read(string path) { reader = std::make_unique<CSVReader>(path); }

/**
 * @brief Определяет тип данных столбца
 * @param reader указатель на объкт обработки файла csv
 * @param head Заголовок столбца
 * @return Тип данных столбца
 */
ColumnType getTypeOfColumn(int column) {
  bool has_numbers = false;
  bool has_strings = false;

  for (const CSVRow &row : *reader) {
    string field = row[column].get<>();
    if (row[column].is_num()) {
      has_numbers = true;
    } else {
      has_strings = true;
    }
  }
  if (has_numbers && has_strings)
    return MIX;

  if (has_numbers)
    return NUMERIC;

  if (has_strings)
    return STRING;

  return UNKNOWN;
}

/*
 * @brief Считать столбец чисел из файла
 * @param reader указатель на "документ"
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<float> readNumericColumn(int column) {
  vector<float> values;

  for (CSVRow &row : *reader) {
    values.push_back(row[column].get<float>());
  }
  return values;
}

/*
 * @brief Считать столбец строк из файла
 * @param reader указатель на "документ"
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<string> readStringColumn(int column) {
  vector<string> values;

  for (CSVRow &row : *reader) {
    values.push_back(row[column].get<string>());
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

/**
 * @brief Создать граф по схеме
 * @param scheme массив строк
 */
void createGraphFromScheme(vector<string> scheme) {
  Node *atom;
  vector<string> id;

  for (const string node : scheme) {
    id = split(node, "-->");
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
