#ifndef ROUTER_H
#define ROUTER_H

#include <string>

using std::string;

/**
 * @brief Основная функция запуска приложения
 */
void runApp();

/**
 * @brief Предоставляет пользователю выбор конфигурационного файла
 * @return Имя файла конфигурации
 */
string chooseConfig();

/**
 * @brief Предоставляет пользователю выбор файла csv
 * @return Имя файла csv
 */
string chooseSourceFile();

/**
 * @brief Запрашивает у пользователя сценарий операций
 */
void manageTasks();

/**
 * @brief Запрашивает у пользователя имя файла для сохранения результатов
 * @return Имя файла для сохранения
 */
string getFileName();

/**
 * @brief Запускает выполнение операций согласно построенному графу
 */
void runOperations();

/**
 * @brief Сохраняет результаты выполнения операций в файл
 */
void saveResults();

#endif // !ROUTER_H
