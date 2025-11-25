#include "../include/router.h"
#include "../include/graph.h"
#include "../include/graphics.h"
#include "../include/operations.h"
#include "../include/parser.h"
#include "../include/utils.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace SimpleDAG {

const string WORKING_DIR = "data";

/**
 * @brief Главная функция запуска приложения SimpleDAG
 * @param workingDir Рабочая директория с конфигурационными файлами и данными
 * @return Код завершения программы (0 - успех, 1 - ошибка)
 */
int run() {
  // Загрузить конфигурацию
  if (!Internal::loadConfiguration(WORKING_DIR)) {
    return 1;
  }

  // Проверить конфигурацию
  if (!Internal::validateConfiguration()) {
    return 1;
  }

  // Загрузить данные
  if (!Internal::loadDataTable()) {
    return 1;
  }

  // Показать доступные операции
  Internal::displayAvailableOperations();

  // Инициализировать логирование
  Internal::initializeLogging();

  // Обработать схему графа
  vector<string> ids = config::getIds();
  if (!Internal::processGraphScheme(ids)) {
    Internal::cleanup();
    return 1;
  }

  // Запустить выполнение операций
  deepFirstSearch(procedure);

  Internal::cleanup();
  return 0;
}

namespace Internal {

/**
 * @brief Загружает конфигурацию из YAML файла
 * @param workingDir Рабочая директория для поиска конфигурационных файлов
 * @return true если конфигурация успешно загружена, false в случае ошибки
 */
bool loadConfiguration(const std::string &workingDir) {
  vector<string> configs = collectFiles(workingDir, ".yaml");
  if (configs.empty()) {
    cout << "Не найдены конфигурационные файлы в директории " << workingDir
         << endl;
    return false;
  }

  string config = tui::selectFileFromList(configs);
  if (config.empty()) {
    cout << "Файл конфигурации не выбран" << endl;
    return false;
  }

  config::load(config);
  return true;
}

/**
 * @brief Проверяет валидность загруженной конфигурации
 * @return true если конфигурация валидна, false если найдены неизвестные
 * функции
 */
bool validateConfiguration() {
  auto unknown_functions = config::checkFunctions();
  if (!unknown_functions.empty()) {
    cout << "Найдены неизвестные функции в файле конфигурации:" << endl;

    for (const auto &func : unknown_functions) {
      cout << "id: " << func.first << ", функция: " << func.second << endl;
    }

    cout << "В файле конфигурации могут быть указаны только функции, которые "
            "есть в этом списке:"
         << endl;
    return false;
  }
  return true;
}

/**
 * @brief Загружает данные из CSV файла указанного в конфигурации
 * @return true если данные успешно загружены, false в случае ошибки
 */
bool loadDataTable() {
  string csvFile = config::getCSV();
  if (csvFile.empty()) {
    cout << "Не указан файл данных в конфигурации" << endl;
    return false;
  }

  string csvPath = WORKING_DIR + "/" + csvFile;
  table::read(csvPath);
  return true;
}

/**
 * @brief Отображает список доступных операций из конфигурации
 */
void displayAvailableOperations() {
  cout << endl;
  vector<string> ids = config::getIds();
  std::cout << "\n=== Доступные операции ===" << endl;
  for (size_t i = 0; i < ids.size(); i++) {
    cout << "[" << i + 1 << "] " << ids[i] << endl;
  }
  std::cout << "==========================" << endl;
}

/**
 * @brief Инициализирует систему логирования с временной меткой
 */
void initializeLogging() {
  time_t now = time(0);
  char *dt = ctime(&now);
  logger::openLog(WORKING_DIR + "/" + dt);
}

/**
 * @brief Обрабатывает ввод и валидацию схемы графа от пользователя
 * @param ids Список идентификаторов доступных операций
 * @return true если схема успешно обработана, false в случае ошибки
 */
bool processGraphScheme(std::vector<std::string> &ids) {
  cout << INSRUCTIONS << endl;

  vector<string> scheme;

  do {
    scheme = getScheme(std::cin);

    // Проверить схему и вывести ошибки если есть
    if (!validateAndDisplayScheme(scheme, ids)) {
      cout << "\n⚠️  Пожалуйста, исправьте ошибки и введите схему заново:"
           << endl;
    } else {
      break; // Выходим из цикла если схема валидна
    }

  } while (true);

  cout << "✅ Схема графа корректна!" << endl;
  createGraphFromScheme(scheme, ids);
  return true;
}

/**
 * @brief Проверяет схему на валидность и отображает ошибки
 * @param scheme Вектор строк схемы графа
 * @param ids Список идентификаторов доступных операций
 * @return true если схема валидна, false если найдены ошибки
 */
bool validateAndDisplayScheme(vector<string> &scheme, vector<string> &ids) {
  vector<vector<SchenmeError>> schemeErrors = checkScheme(scheme, ids);
  bool isValid = true;

  for (size_t i = 0; i < schemeErrors.size(); ++i) {
    if (!schemeErrors[i].empty()) {
      if (isValid) {
        cout << "\n❌ Обнаружены ошибки в схеме:" << endl;
        isValid = false;
      }
      displaySchemeErrors(scheme[i], schemeErrors[i], i + 1);
    }
  }
  return isValid;
}

/**
 * @brief Отображает ошибки в строке схемы графа
 * @param schemeLine Строка схемы с ошибкой
 * @param errors Вектор ошибок для данной строки
 * @param lineNumber Номер строки в схеме
 */
void displaySchemeErrors(const std::string &schemeLine,
                         const std::vector<SchenmeError> &errors,
                         int lineNumber) {
  cout << "Строка " << lineNumber << " \"" << schemeLine << "\": ";

  for (const auto &error : errors) {
    switch (error) {
    case EMPTY:
      cout << "пустая строка; ";
      break;
    case UNEXPECTED_DELIMITER:
      cout << "неожиданный разделитель; ";
      break;
    case UNEXPECTED_SYMBOLS:
      cout << "недопустимые символы; ";
      break;
    case INDEX_OUT_OF_RANGE:
      cout << "индекс вне диапазона; ";
      break;
    }
  }
  cout << endl;
}

/**
 * @brief Освобождает ресурсы и очищает глобальное состояние
 */
void cleanup() {
  config::clear();
  table::clear();
  logger::close();
  clearGraph();
}

} // namespace Internal
} // namespace SimpleDAG
