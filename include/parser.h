#ifndef PARSER_H

#define PARSER_H
#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief Структура, представляющая позицию ячейки в таблице
 */
struct Cell {
  int column; // Номер столбца (начиная с 0)
  int row;    // Номер строки (начиная с 0)
};

/**
 * @brief Перечисление типов данных столбцов таблицы
 */
enum ColumnType {
  NUMERIC, // Столбец содержит только числовые данные
  STRING,  // Столбец содержит только строковые данные
  MIX,     // Столбец содержит данные разных типов
  UNKNOWN, // Тип данных не определен
};

/**
 * @brief Определяет тип данных столбца
 * @param head Заголовок столбца
 * @return Тип данных столбца
 */
ColumnType getTypeOfColumn(string head);

/**
 * @brief Определяет тип данных столбца
 * @param cells Вектор ячеек столбца
 * @return Тип данных столбца
 */
ColumnType getTypeOfColumn(vector<Cell> cells);

/**
 * @brief Читает строковый столбец таблицы по заголовку
 * @param head Заголовок столбца
 * @return Вектор строковых значений столбца
 */
vector<string> readStringColumn(string head);

/**
 * @brief Читает строковый столбец таблицы по набору ячеек
 * @param cells Вектор ячеек для чтения
 * @return Вектор строковых значений
 */
vector<string> readStringColumn(vector<Cell> cells);

/**
 * @brief Читает числовой столбец таблицы по заголовку
 * @param head Заголовок столбца
 * @return Вектор числовых значений столбца
 */
vector<double> readNumericColumn(string head);

/**
 * @brief Читает числовой столбец таблицы по набору ячеек
 * @param cells Вектор ячеек для чтения
 * @return Вектор числовых значений
 */
vector<double> readNumericColumn(vector<Cell> cells);

#endif // !PARSER_H
