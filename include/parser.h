#ifndef PARSER_H
#define PARSER_H

#include "../libs/csv-parser/include/csv.hpp"
#include "../libs/mini-yaml/yaml/Yaml.hpp"
#include <string>
#include <vector>

using csv::CSVReader;
using std::string;
using std::vector;

/**
 * @brief Перечисление типов данных столбцов таблицы
 */
enum ColumnType {
  NUMERIC, // Столбец содержит только числовые данные
  STRING,  // Столбец содержит только строковые данные
  MIX,     // Столбец содержит данные разных типов
  UNKNOWN, // Тип данных не определен
};

namespace config {

/*
 * @brief Загрузить конфигурацию
 * @param path путь к конфигурационному файлу
 */
void loadConfig(string path);

/*
 * @brief Получить параметры (номера столбцов) по id
 * @param id уникальный идентификотор операции
 * @return номер столбца
 */
int getColumnById(string id);

/*
 * @brief Получить тип операции
 * @param id уникальный идентификатор операции
 * @return тип операции
 */
string getOpTypeById(string id);
}; // namespace config

namespace table {

/**
 * @brief Определяет тип данных столбца
 * @param reader указатель на объкт обработки файла csv
 * @param head Заголовок столбца
 * @return Тип данных столбца
 */
ColumnType getTypeOfColumn(int column);
/*
 * @brief Считать столбец чисел из файла
 * @param reader указатель на "документ"
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<float> readNumericColumn(int column);

/*
 * @brief Считать столбец строк из файла
 * @param reader указатель на "документ"
 * @param column номер столбца
 * @return вектор чисел столбца
 */
vector<string> readStringColumn(int column);

}; // namespace table

/**
 * @brief Разделить строку на подстроки по разделителю
 * @param s строка
 * @param delimiter разделитель
 * @return вектор подстрок
 */
vector<string> split(const string &s, const string &delimiter);

/**
 * @brief Создать граф по схеме
 * @param scheme массив строк
 */
void createGraphFromScheme(vector<string> scheme);

#endif // !PARSER_H
