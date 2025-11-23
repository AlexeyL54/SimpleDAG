#include "../include/graph.h"
#include "../include/graphics.h"
#include "../include/operations.h"
#include "../include/parser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

using std::fstream;
using std::string;
using std::vector;

namespace fs = std::filesystem;

const string SETUP = "setup.conf";

namespace logger {
fstream log;

/*
 * @brief Открыть лог
 * @param путь к файлу лога
 */
void openLog(string path) {
  log.open(path, std::ios::out); // Исправлено: открываем глобальную переменную
  if (not log.is_open())
    std::cerr << "Failed to open log" << std::endl;
}

/*
 * @brief Записать результат выполненя операции над числовым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, float res) {
  if (log.is_open()) {
    log << id << " >> " << res << std::endl;
  }
}

/*
 * @brief Записать результат выполненя операции над строковым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, string res) {
  if (log.is_open()) {
    log << id << " >> " << res << std::endl;
  }
}

/*
 * @brief Записать предупреждения
 * @param massage сообщение
 */
void warning(string message) {
  if (log.is_open()) {
    log << message << std::endl;
  }
}

/*
 * @brief Закрыть лог
 */
void close() {
  if (log.is_open()) {
    log.close();
  }
}
}; // namespace logger

// =========================================================================

namespace tui {

/**
 * @brief Отображает список файлов в виде нумерованного меню
 * @param files вектор строк с именами файлов
 */
void displayFileMenu(const std::vector<std::string> &files) {
  std::cout << "\n=== Доступные файлы ===\n";
  for (size_t i = 0; i < files.size(); ++i) {
    std::cout << "[" << (i + 1) << "] " << files[i] << "\n";
  }
  std::cout << "[0] Выход\n";
  std::cout << "=======================\n";
}

/**
 * @brief Проверяет, является ли строка числом
 * @param str входная строка для проверки
 * @return true если строка представляет собой число, иначе false
 */
bool isNumber(const std::string &str) {
  if (str.empty())
    return false;

  for (char c : str) {
    if (!std::isdigit(static_cast<unsigned char>(c))) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Удаляет пробелы в начале и конце строки
 * @param str строка для обработки
 * @return обработанная строка без пробелов по краям
 */
std::string trimString(const std::string &str) {
  if (str.empty())
    return str;

  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");

  if (start == std::string::npos)
    return "";

  return str.substr(start, end - start + 1);
}

/**
 * @brief Обрабатывает ввод пользователя и проверяет его корректность
 * @param input ссылка на строку для сохранения ввода
 * @return true если ввод корректен, false если нужно повторить
 */
bool processUserInput(std::string &input) {
  std::getline(std::cin, input);
  input = trimString(input);

  if (!isNumber(input)) {
    std::cout << "❌ Ошибка: введите число!\n";
    return false;
  }

  return true;
}

/**
 * @brief Преобразует строку в число с обработкой исключений
 * @param str строка для преобразования
 * @param result ссылка для сохранения результата
 * @return true если преобразование успешно, false если произошла ошибка
 */
bool safeStringToInt(const std::string &str, int &result) {
  try {
    result = std::stoi(str);
    return true;
  } catch (const std::exception &) {
    std::cout << "❌ Ошибка: введите корректное число!\n";
    return false;
  }
}

/**
 * @brief Проверяет, находится ли число в допустимом диапазоне
 * @param number число для проверки
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return true если число в диапазоне, false если нет
 */
bool isInRange(int number, int min, int max) {
  if (number >= min && number <= max) {
    return true;
  } else {
    std::cout << "❌ Ошибка: число должно быть от " << min << " до " << max
              << "!\n";
    return false;
  }
}

/**
 * @brief Получает числовой ввод от пользователя с валидацией
 * @param prompt приглашение для ввода
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return валидное число в заданном диапазоне
 */
int getValidatedNumberInput(const std::string &prompt, int min, int max) {
  std::string input;
  int number;
  bool isValid = false;

  while (!isValid) {
    std::cout << prompt;

    bool inputValid = processUserInput(input);
    if (!inputValid) {
      continue; // Здесь continue допустим для повторения цикла
    }

    bool conversionValid = safeStringToInt(input, number);
    if (!conversionValid) {
      continue; // Здесь continue допустим для повторения цикла
    }

    isValid = isInRange(number, min, max);
  }

  return number;
}

/**
 * @brief Основная функция для выбора файла из списка
 * @param files вектор строк с путями к файлам
 * @return выбранный путь к файлу как строка, или пустая строка если выбран
 * выход
 */
std::string selectFileFromList(const std::vector<std::string> &files) {
  if (files.empty()) {
    std::cout << "⚠️  Список файлов пуст!\n";
    return "";
  }

  bool selectionMade = false;
  std::string selectedFile;

  while (!selectionMade) {
    displayFileMenu(files);

    std::string prompt =
        "Выберите файл (0-" + std::to_string(files.size()) + "): ";
    int choice =
        getValidatedNumberInput(prompt, 0, static_cast<int>(files.size()));

    if (choice == 0) {
      std::cout << "Выход из программы.\n";
      return "";
    }

    selectedFile = files[choice - 1];
    std::cout << "✅ Выбран файл: " << selectedFile << "\n";
    selectionMade = true;
  }

  return selectedFile;
}

// Альтернативная версия getValidatedNumberInput без continue
/**
 * @brief Получает числовой ввод от пользователя с валидацией (версия без
 * continue)
 * @param prompt приглашение для ввода
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return валидное число в заданном диапазоне
 */
int getValidatedNumberInputNoContinue(const std::string &prompt, int min,
                                      int max) {
  std::string input;
  int number = min - 1; // Начальное невалидное значение

  while (number < min || number > max) {
    std::cout << prompt;
    std::getline(std::cin, input);
    input = trimString(input);

    if (isNumber(input) && safeStringToInt(input, number)) {
      if (number >= min && number <= max) {
        break; // Валидный ввод - выходим из цикла
      } else {
        std::cout << "❌ Ошибка: число должно быть от " << min << " до " << max
                  << "!\n";
        number = min - 1; // Сбрасываем для продолжения цикла
      }
    } else {
      std::cout << "❌ Ошибка: введите число от " << min << " до " << max
                << "!\n";
    }
  }

  return number;
}

}; // namespace tui

// =========================================================================

/*
 * @brief Получить путь к файлу конфигурации, путь к файлу результатов
 * выполнения операций
 * @param config путь к файлу конфигурации
 * @param res_file путь к файлу результатов выполнения операций
 */
void getSourceFiles(string &config, string &res_file) {
  fstream file(SETUP);

  if (not file.is_open()) {
    std::cerr << "Failed to open setup file." << std::endl;
    return;
  }

  std::getline(file, config);
  std::getline(file, res_file);
}

/*
 * @brief Получить все пути файлов в директории с определенным расширением
 * @param path директория
 * @param extension расширение
 * @return вектор путей к найденным файлам
 */
vector<string> collectFiles(string path, string extension) {
  vector<string> files;

  for (const fs::directory_entry &entry : fs::directory_iterator(path)) {
    if (entry.path().string().find(extension) != string::npos) {
      files.push_back(entry.path());
    }
  }

  return files;
}

/*
 * @brief Процедура запуска операции в узле графа
 * @param node указатель на узел графа
 */
void procedure(Node *node) {
  string id = node->id;
  string type = config::getFuncById(id);
  int column = config::getColumnById(id);

  switch (table::getTypeOfColumn(column)) {

  case NUMERIC: {
    vector<float> num_vec = table::readNumericColumn(column);
    logger::writeResult(id, callOperation(type, num_vec));
    break;
  }

  case STRING: {
    vector<string> str_vec = table::readStringColumn(column);
    logger::writeResult(id, callOperation(type, str_vec));
    break;
  }

  case UNKNOWN: {
    string message = "Unknown type found in column " + std::to_string(column) +
                     ". Skipping " + id;
    logger::warning(message);
  }
  }
}
