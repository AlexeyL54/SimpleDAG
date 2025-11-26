#ifndef ROUTER_H
#define ROUTER_H

#include "parser.h"
#include <string>
#include <vector>

namespace SimpleDAG {

/**
 * @brief Главная функция запуска приложения SimpleDAG
 * @param workingDir Рабочая директория с конфигурационными файлами и данными
 * @return Код завершения программы (0 - успех, 1 - ошибка)
 */
int run();

namespace Internal {

/**
 * @brief Загружает конфигурацию из YAML файла
 * @param workingDir Рабочая директория для поиска конфигурационных файлов
 * @return true если конфигурация успешно загружена, false в случае ошибки
 */
bool loadConfiguration(const std::string &workingDir);

/**
 * @brief Проверяет валидность загруженной конфигурации
 * @return true если конфигурация валидна, false если найдены неизвестные
 * функции
 */
bool validateConfiguration();

/**
 * @brief Загружает данные из CSV файла указанного в конфигурации
 * @return true если данные успешно загружены, false в случае ошибки
 */
bool loadDataTable();

/**
 * @brief Отображает список доступных операций из конфигурации
 */
void displayAvailableOperations();

/**
 * @brief Инициализирует систему логирования с временной меткой
 */
void initializeLogging();

/**
 * @brief Обрабатывает ввод и валидацию схемы графа от пользователя
 * @param ids Список идентификаторов доступных операций
 * @return true если схема успешно обработана, false в случае ошибки
 */
bool processGraphScheme(std::vector<std::string> &ids);

/**
 * @brief Отображает ошибки в строке схемы графа
 * @param schemeLine Строка схемы с ошибкой
 * @param errors Вектор ошибок для данной строки
 * @param lineNumber Номер строки в схеме
 */
void displaySchemeErrors(const std::string &schemeLine,
                         const std::vector<SchenmeError> &errors,
                         int lineNumber);

/**
 * @brief Проверяет схему на валидность и отображает ошибки
 * @param scheme Вектор строк схемы графа
 * @param ids Список идентификаторов доступных операций
 * @return true если схема валидна, false если найдены ошибки
 */
bool validateAndDisplayScheme(std::vector<std::string> &scheme,
                              std::vector<std::string> &ids);

/**
 * @brief Освобождает ресурсы и очищает глобальное состояние
 */
void cleanup();
} // namespace Internal
} // namespace SimpleDAG

#endif // !ROUTER_H
