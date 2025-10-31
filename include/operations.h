#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "parser.h"

using std::function;
using std::map;
using std::string;
using std::variant;
using std::vector;

/*
 * @brief Определение возможных типов функций для словаря операций
 */
using FunctionVariant =
    variant<function<float(vector<float>)>,   // Функции для числовых данных
            function<string(vector<string>)>, // Функции для строковых данных
            function<float(vector<Cell>)>,    // Функции для работы с ячейками
            function<string(vector<Cell>)>    // Функции для работы с ячейками
            >;

/**
 * @brief Словарь операций, поддерживающий различные типы данных через variant
 * Ключ: идентификатор операции
 * Значение: функция соответствующего типа
 */
extern map<string, FunctionVariant> operation_map;

/*
 * @brief Вызвать операцию из словаря операций
 * @param op_name указатель на ключ операции
 * @param arg входной параметр вызываемой операции
 * @return результат работы вызываемой функции
 */
string callOperation(const string &op_name, vector<string> arg);

/*
 * @brief Вызвать операцию из словаря операций
 * @param op_name указатель на ключ операции
 * @param arg входной параметр вызываемой операции
 * @return результат работы вызываемой функции
 */
float callOperation(const string &op_name, vector<float> arg);

/**
 * @brief Вычисляет сумму элементов числового вектора
 * @param source Входной вектор числовых данных
 * @return Сумма всех элементов
 */
float sum(vector<float> source);

/**
 * @brief Вычисляет среднее арифметическое элементов числового вектора
 * @param source Входной вектор числовых данных
 * @return Среднее значение элементов
 */
float average(vector<float> source);

/**
 * @brief Объединяет строки из вектора в одну строку
 * @param source Входной вектор строк
 * @return Результирующая объединенная строка
 */
string concatinate(vector<string> source);

/**
 * @brief Выполняет поиск и замену в векторе строк
 * @param source Входной вектор строк
 * @param find Строка для поиска
 * @param replace Строка для замены
 * @return Вектор с выполненными заменами
 */
vector<string> findReplace(vector<string> source, string find, string replace);

#endif // !OPERATIONS_H
