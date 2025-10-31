#include "../include/operations.h"
#include <functional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using std::function;
using std::get_if;
using std::runtime_error;
using std::string;
using std::vector;

map<string, FunctionVariant> operation_map = {
    {"sum", function<float(vector<float>)>(sum)},
    {"average", function<float(vector<float>)>(average)},
    {"concatinate", function<string(vector<string>)>(concatinate)},
};

/*
 * @brief Вызвать операцию из словаря операций
 * @param op_name указатель на ключ операции
 * @param arg входной параметр вызываемой операции
 * @return результат работы вызываемой функции
 */
float callOperation(const string &op_name, vector<float> arg) {
  auto map_elem = operation_map.find(op_name);
  if (map_elem == operation_map.end())
    throw runtime_error("Operation not found");

  auto *func = get_if<function<float(vector<float>)>>(&map_elem->second);
  if (!func)
    throw std::runtime_error("Invalid operation type");

  return (*func)(arg);
}

/*
 * @brief Вызвать операцию из словаря операций
 * @param op_name указатель на ключ операции
 * @param arg входной параметр вызываемой операции
 * @return результат работы вызываемой функции
 */
string callOperation(const string &op_name, vector<string> arg) {
  auto map_elem = operation_map.find(op_name);
  if (map_elem == operation_map.end())
    throw runtime_error("Operation not found");

  auto *func = get_if<function<string(vector<string>)>>(&map_elem->second);
  if (!func)
    throw std::runtime_error("Invalid operation type");

  return (*func)(arg);
}

/**
 * @brief Вычисляет сумму элементов числового вектора
 * @param source Входной вектор числовых данных
 * @return Сумма всех элементов
 */
float sum(vector<float> source) {
  float sum = 0;
  for (float num : source) {
    sum += num;
  }
  return sum;
}

/**
 * @brief Вычисляет среднее арифметическое элементов числового вектора
 * @param source Входной вектор числовых данных
 * @return Среднее значение элементов
 */
float average(vector<float> source) {
  float sum = 0;
  for (float num : source) {
    sum += num;
  }
  return sum / source.size();
}

/**
 * @brief Объединяет строки из вектора в одну строку
 * @param source Входной вектор строк
 * @return Результирующая объединенная строка
 */
string concatinate(vector<string> source) {
  string result = "";
  for (string str : source) {
    result += str;
  }
  return result;
}
