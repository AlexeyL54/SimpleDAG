#ifndef PARSER_H
#define PARSER_H

#include "../libs/Tiny_Yaml/yaml/yaml.hpp"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

/**
 * @brief Перечисление типов данных столбцов таблицы
 */
enum ColumnType {
  NUMERIC, // Столбец содержит только числовые данные
  STRING,  // Столбец содержит только строковые данные
  UNKNOWN, // Тип данных не определен
};

/*
 * @brief Перечисление типов возможных ошибок при создании схемы графа
 */
enum SchenmeError {
  INDEX_OUT_OF_RANGE,   // не возможно извлечь id по индексу
  UNEXPECTED_DELIMITER, // разделитель найден на в начале или конце строки ввода
  EMPTY,                // пустой ввод
  UNEXPECTED_SYMBOLS,   // найден символ, не являющийся цифрой или разделителем
};

namespace config {

/*
 * @brief Загрузить конфигурацию
 * @param path путь к конфигурационному файлу
 */
void load(string path);

/*
 * Очистить объект конфигурации
 */
void clear();

/*
 * @brief Проверить наличие операций из конфигурации
 * @return словарь ненайденных функций, где ключ - id операции, значение -
 * функция
 */
map<string, string> checkFunctions();

/*
 * @brief Получить id всех доступных операций
 */
vector<string> getIds();

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
string getFuncById(string id);

/*
 * @brief Получить путь к файлу csv для обработки
 * @return путь к файлу
 */
string getCSV();
}; // namespace config

namespace table {

/**
 * @brief Инициализировать объкт документа
 * @param path путь к документу
 */
void read(string path, char delimiter = ',');

/**
 * @brief Очистить таблицу
 */
void clear();

/*
 * @brief Получить id всех доступных операций
 */
vector<string> getIds();

/**
 * @brief Определить, является ли строка числом
 * @param s строка
 * @return true если строка является числом, false - иначе
 */
bool isNumneric(string &s);

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

/*
 * @brief Считать схему графа из потокового ввода
 * @param is указатель на поток ввода @brief Считать схему графа из потокового
 * ввода
 */
vector<string> getScheme(std::istream &is);

/*
 * @brief Проверить ввод схемы
 * @param scheme схема графа
 * @param ids id операций
 * @return вектор ошибок для каждой ветви схемы
 */
vector<vector<SchenmeError>> checkScheme(vector<string> &scheme,
                                         vector<string> &ids);

/**
 * @brief Создать граф по схеме
 * @param scheme массив строк
 */
void createGraphFromScheme(vector<string> scheme);

/**
 * @brief Создать граф по схеме индексов id
 * @param scheme вектор строк с индексами id операций
 * @param ids вектор id операций
 */
void createGraphFromScheme(vector<string> scheme, vector<string> ids);

#endif // !PARSER_H
