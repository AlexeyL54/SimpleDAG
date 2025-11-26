#include "../../include/graph.h"
#include "../../include/operations.h"
#include "../../include/parser.h"
#include "../../include/utils.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using std::fstream;
using std::string;

namespace fs = std::filesystem;

// Вспомогательные функции для создания временных файлов
void createTestCSV(const string &filename) {
  std::ofstream file(filename);
  file << "1,!,1,a,1\n"
       << "1,@,2,b,abc\n"
       << "1,#,3,c,\n"
       << "1,$,4,d,\n"
       << "1,%,5,e,\n"
       << "1,^,6,f,\n"
       << "1,&,7,g,\n"
       << "1,*,8,h,\n"
       << "1,?,9,i,\n";
  file.close();
}

void createTestYAML(const string &filename) {
  std::ofstream file(filename);
  file << "operations:\n"
       << "  sum_elements_of_column_1:\n"
       << "    func: sum\n"
       << "    column: 1\n"
       << "  sum_elements_of_column_2:\n"
       << "    func: sum\n"
       << "    column: 2\n"
       << "  find_average_of_column_1:\n"
       << "    func: average\n"
       << "    column: 1\n"
       << "  find_average_of_column_2:\n"
       << "    func: average\n"
       << "    column: 2\n"
       << "  concat:\n"
       << "    func: concatinate\n"
       << "    column: 3\n";
  file.close();
}

// Вспомогательная функция чтения содержимого файла
string readTxt(string path) {
  string result;
  string line;
  fstream file(path);

  if (not file.is_open()) {
    std::cerr << "Failed to open txt file in utils test." << std::endl;
    return "";
  }

  result.clear();
  while (std::getline(file, line)) {
    if (!result.empty()) {
      result += "\n";
    }
    result += line;
  }
  file.close();

  return result;
}

// Вспомогательная функция создания setup.conf файла
void createSetupFile(const string &filename, const string &config = "test.yaml",
                     const string &result = "result.txt") {
  std::ofstream file(filename);
  file << config << "\n" << result << "\n";
  file.close();
}

// Тест чтения названий файлов конфигурации и логирования
TEST(UTILS, GetSourceFiles) {
  string setup_file = "setup.conf";
  createSetupFile(setup_file, "test_config.yaml", "test_result.txt");

  string config, res_file;
  getSourceFiles(config, res_file);

  EXPECT_EQ(config, "test_config.yaml");
  EXPECT_EQ(res_file, "test_result.txt");

  fs::remove(setup_file);
}

// Тест записи результатов обработки числового столбца
TEST(UTILS, WriteNumericResult) {
  string result_file = "test_numeric_result.txt";

  logger::openLog(result_file);
  logger::writeResult("sum", 1.5f);
  logger::close();

  EXPECT_EQ("sum >> 1.5", readTxt(result_file));
  fs::remove(result_file);
}

// Тест записи результатов обработки строкового столбца
TEST(UTILS, WriteStringResult) {
  string result_file = "test_string_result.txt";

  logger::openLog(result_file);
  logger::writeResult("concatinate", "abc");
  logger::close();

  EXPECT_EQ("concatinate >> abc", readTxt(result_file));
  fs::remove(result_file);
}

// Тест записи предупреждения
TEST(UTILS, Warning) {
  string result_file = "test_warning_result.txt";
  string message = "warning message";

  logger::openLog(result_file);
  logger::warning(message);
  logger::close();

  EXPECT_EQ("warning message", readTxt(result_file));
  fs::remove(result_file);
}

// Тест процедуры выполнения операции с числовым столбцом
TEST(UTILS, ProcedureNumeric) {
  string setup_file = "setup.conf";
  string config_file = "test_proc_config.yaml";
  string result_file = "test_proc_result.txt";
  string csv_file = "test_proc_data.csv";

  createSetupFile(setup_file, config_file, result_file);
  createTestCSV(csv_file);

  // Создаем упрощенную конфигурацию для теста
  std::ofstream config(config_file);
  config << "operations:\n"
         << "  test_sum:\n"
         << "    func: sum\n"
         << "    column: 0\n"; // Столбец с числами 1
  config.close();

  config::load(config_file);
  table::read(csv_file);

  Node *test_node = createNode("test_sum");
  logger::openLog(result_file);
  procedure(test_node);
  logger::close();

  EXPECT_EQ("test_sum >> 9", readTxt(result_file)); // Сумма 9 единиц

  config::clear();
  table::clear();
  clearGraph();

  fs::remove(setup_file);
  fs::remove(config_file);
  fs::remove(result_file);
  fs::remove(csv_file);
}

// Тест процедуры с строковым столбцом
TEST(UTILS, ProcedureString) {
  string setup_file = "setup.conf";
  string config_file = "test_proc_string_config.yaml";
  string result_file = "test_proc_string_result.txt";
  string csv_file = "test_proc_string_data.csv";

  createSetupFile(setup_file, config_file, result_file);
  createTestCSV(csv_file);

  std::ofstream config(config_file);
  config << "operations:\n"
         << "  test_concat:\n"
         << "    func: concatinate\n"
         << "    column: 3\n"; // Столбец с буквами a-i
  config.close();

  config::load(config_file);
  table::read(csv_file);

  Node *test_node = createNode("test_concat");
  logger::openLog(result_file);
  procedure(test_node);
  logger::close();

  EXPECT_EQ("test_concat >> abcdefghi", readTxt(result_file));

  config::clear();
  table::clear();
  clearGraph();

  fs::remove(setup_file);
  fs::remove(config_file);
  fs::remove(result_file);
  fs::remove(csv_file);
}

// Тест процедуры с вычислением среднего
TEST(UTILS, ProcedureAverage) {
  string setup_file = "setup.conf";
  string config_file = "test_proc_avg_config.yaml";
  string result_file = "test_proc_avg_result.txt";
  string csv_file = "test_proc_avg_data.csv";

  createSetupFile(setup_file, config_file, result_file);
  createTestCSV(csv_file);

  std::ofstream config(config_file);
  config << "operations:\n"
         << "  test_avg:\n"
         << "    func: average\n"
         << "    column: 2\n"; // Столбец с числами 1-9
  config.close();

  config::load(config_file);
  table::read(csv_file);

  Node *test_node = createNode("test_avg");
  logger::openLog(result_file);
  procedure(test_node);
  logger::close();

  EXPECT_EQ("test_avg >> 5", readTxt(result_file)); // Среднее 1-9 = 5

  config::clear();
  table::clear();
  clearGraph();

  fs::remove(setup_file);
  fs::remove(config_file);
  fs::remove(result_file);
  fs::remove(csv_file);
}

// Тест процедуры с неизвестным типом столбца
TEST(UTILS, ProcedureUnknownType) {
  string setup_file = "setup.conf";
  string config_file = "test_unknown_config.yaml";
  string result_file = "test_unknown_result.txt";

  createSetupFile(setup_file, config_file, result_file);

  std::ofstream config(config_file);
  config << "operations:\n"
         << "  test_unknown:\n"
         << "    func: sum\n"
         << "    column: 999\n"; // Несуществующий столбец
  config.close();

  table::clear(); // Очищаем таблицу

  config::load(config_file);

  Node *test_node = createNode("test_unknown");
  logger::openLog(result_file);
  procedure(test_node);
  logger::close();

  string result = readTxt(result_file);
  EXPECT_TRUE(result.find("Unknown type found") != string::npos);
  EXPECT_TRUE(result.find("Skipping test_unknown") != string::npos);

  config::clear();
  clearGraph();

  fs::remove(setup_file);
  fs::remove(config_file);
  fs::remove(result_file);
}
