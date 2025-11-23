#ifndef UTILS_H
#define UTILS_H

#include "../include/graph.h"
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
using std::ifstream;
using std::string;
using std::vector;

namespace logger {

/*
 * @brief Открыть лог
 * @param путь к файлу лога
 */
void openLog(string path);

/*
 * @brief Записать результат выполненя операции над числовым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, float res);

/*
 * @brief Записать результат выполненя операции над строковым столбцом
 * @param id уникальный идентификатор операции
 * @param res результат выполнения
 */
void writeResult(string id, string res);

/*
 * @brief Записать предупреждения
 * @param massage сообщение
 */
void warning(string message);

/*
 * @brief Закрыть лог
 */
void close();
}; // namespace logger

// =========================================================================

namespace tui {
/**
 * @brief Напечатать стилизованную надпись в консоли
 */
void printLogo();

/**
 * @brief Вывести инструкцию о вводе схемы графа
 */
void printSchemeInstruction();

/**
 * @brief Отображает список файлов в виде нумерованного меню
 * @param files вектор строк с именами файлов
 */
void displayFileMenu(const std::vector<std::string> &files);

/**
 * @brief Проверяет, является ли строка числом
 * @param str входная строка для проверки
 * @return true если строка представляет собой число, иначе false
 */
bool isNumber(const std::string &str);

/**
 * @brief Удаляет пробелы в начале и конце строки
 * @param str строка для обработки
 * @return обработанная строка без пробелов по краям
 */
std::string trimString(const std::string &str);

/**
 * @brief Обрабатывает ввод пользователя и проверяет его корректность
 * @param input ссылка на строку для сохранения ввода
 * @return true если ввод корректен, false если нужно повторить
 */
bool processUserInput(std::string &input);

/**
 * @brief Преобразует строку в число с обработкой исключений
 * @param str строка для преобразования
 * @param result ссылка для сохранения результата
 * @return true если преобразование успешно, false если произошла ошибка
 */
bool safeStringToInt(const std::string &str, int &result);

/**
 * @brief Проверяет, находится ли число в допустимом диапазоне
 * @param number число для проверки
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return true если число в диапазоне, false если нет
 */
bool isInRange(int number, int min, int max);

/**
 * @brief Получает числовой ввод от пользователя с валидацией
 * @param prompt приглашение для ввода
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return валидное число в заданном диапазоне
 */
int getValidatedNumberInput(const std::string &prompt, int min, int max);

/**
 * @brief Основная функция для выбора файла из списка
 * @param files вектор строк с путями к файлам
 * @return выбранный путь к файлу как строка, или пустая строка если выбран
 * выход
 */
std::string selectFileFromList(const std::vector<std::string> &files);

/**
 * @brief Получает числовой ввод от пользователя с валидацией (версия без
 * continue)
 * @param prompt приглашение для ввода
 * @param min минимальное допустимое значение
 * @param max максимальное допустимое значение
 * @return валидное число в заданном диапазоне
 */
int getValidatedNumberInputNoContinue(const std::string &prompt, int min,
                                      int max);
}; // namespace tui

// =========================================================================

/*
 * @brief Получить путь к файлу конфигурации, путь к файлу результатов
 * выполнения операций
 * @param config путь к файлу конфигурации
 * @param res_file путь к файлу результатов выполнения операций
 */
void getSourceFiles(string &config, string &res_file);

/*
 * @brief Получить все пути файлов в директории с определенным расширением
 * @param path директория
 * @param extension расширение
 * @return вектор путей к найденным файлам
 */
vector<string> collectFiles(string path, string extension);

/*
 * @brief Процедура запуска операции в узле графа
 * @param node указатель на узел графа
 */
void procedure(Node *node);

#endif // !UTILS_H
