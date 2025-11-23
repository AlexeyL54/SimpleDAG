#include "include/graph.h"
#include "include/graphics.h"
#include "include/operations.h"
#include "include/parser.h"
#include "include/utils.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

const string WORKING_DIR = "data";

int main() {
  cout << LOGO << endl;

  // выбрать файл конфигурации и загрузить конфигурацию
  vector<string> configs = collectFiles(WORKING_DIR, ".yaml");
  string config = tui::selectFileFromList(configs);
  config::load(config);

  // проверить конфигурацию на неизвестные функции
  map<string, string> unknown_functions = config::checkFunctions();
  if (not unknown_functions.empty()) {
    cout << "Найдены неизвестные функции в файле " << config << ":" << endl;

    for (auto func : unknown_functions) {
      cout << "id: " << func.first << ", функция: " << func.second << endl;
    }

    cout << "В файле конфигурации могут быть указаны только функции, которые "
            "есть в этом списке:"
         << endl;
    return 1;
  }

  // получить путь к файлу csv для обработки и считать содержимое файла
  string csv = WORKING_DIR + "/" + config::getCSV();
  table::read(csv);

  cout << endl;
  vector<string> ids = config::getIds();
  std::cout << "\n=== Доступные операции ===\n";
  for (size_t i = 0; i < ids.size(); i++)
    cout << "[" << i + 1 << "] " << ids[i] << endl;
  std::cout << "==========================\n";

  // получить дату для названия лога
  time_t now = time(0);
  char *dt = ctime(&now);
  logger::openLog(WORKING_DIR + "/" + dt);

  // получить схему графа и создать по ней граф
  cout << INSRUCTIONS << endl;
  vector<string> scheme = getScheme(std::cin);
  createGraphFromScheme(scheme, ids);

  // запустить выполнение операций
  deepFirstSearch(procedure);

  config::clear();
  table::clear();
  logger::close();
  clearGraph();

  return 0;
}
