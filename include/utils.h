#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief Собирает список файлов в указанной директории
 * @param path Путь к директории
 * @return Вектор имен файлов
 */
vector<string> collectFilesIn(string path);

/**
 * @brief Выполняет операцию над данными
 */
void executeOperation();

/**
 * @brief Проверяет корректность конфигурационного файла
 * @param path Путь к конфигурационному файлу
 * @return true если конфиг корректен, false если есть ошибки
 */
bool checkConfig(string path);

#endif // !UTILS_H
