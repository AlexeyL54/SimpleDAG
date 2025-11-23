#include "../../include/graph.h"
#include "../../include/parser.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

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
  file << "path: test.csv\n"
       << "operations:\n"
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

void createYAMLWithUnknownFunc(const string &filename) {
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
       << "    column: 3\n"
       << "  unknown:\n"
       << "    func: smthg\n"
       << "    column: 3\n";
  file.close();
}

// Тест считывания схемы графа из потока ввода
TEST(PARSER, GetScheme) {
  vector<string> scheme = {"A-->B-->C", "B-->D-->C"};
  std::stringstream s("A-->B-->C\nB-->D-->C\nend\n");

  EXPECT_EQ(scheme, getScheme(s));
}

// Тест разделителя строки
TEST(PARSER, Split) {
  vector<string> splited = split("A-->B", "-->");

  EXPECT_EQ(splited[0], "A");
  EXPECT_EQ(splited[1], "B");
}

// Тест составления графа по схеме
TEST(PARSER, CreateGraphFromScheme) {
  vector<string> scheme = {"A-->B-->C", "B-->D-->C"};

  createGraphFromScheme(scheme);
  Node *a = getNodeById("A");
  Node *b = getNodeById("B");
  Node *c = getNodeById("C");
  Node *d = getNodeById("D");
  vector<Node *> b_adjents = adjentNodes(b);

  EXPECT_EQ(a->adjency_list_head->my_head, b);
  EXPECT_EQ(b_adjents[0], c);
  EXPECT_EQ(b_adjents[1], d);
  EXPECT_EQ(d->adjency_list_head->my_head, c);

  clearGraph();
}

// Тест определения типа столбца таблицы
TEST(PARSER, GetTypeOfColumn) {
  string path = "test_gettype.csv";
  createTestCSV(path);

  table::read(path);
  EXPECT_EQ(NUMERIC, table::getTypeOfColumn(0));
  EXPECT_EQ(STRING, table::getTypeOfColumn(1));
  EXPECT_EQ(NUMERIC, table::getTypeOfColumn(2));
  EXPECT_EQ(STRING, table::getTypeOfColumn(3));

  table::clear();
  fs::remove(path);
}

// Тест чтения числового столбца таблицы
TEST(PARSER, ReadNumericColumn) {
  string path = "test_numeric.csv";
  createTestCSV(path);

  table::read(path);
  vector<float> values1 = {1, 1, 1, 1, 1, 1, 1, 1, 1};
  vector<float> values2 = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_EQ(values1, table::readNumericColumn(0));
  EXPECT_EQ(values2, table::readNumericColumn(2));

  table::clear();
  fs::remove(path);
}

// Тест чтения строкового столбца таблицы
TEST(PARSER, ReadStringColumn) {
  string path = "test_string.csv";
  createTestCSV(path);

  table::read(path);
  vector<string> values1 = {"!", "@", "#", "$", "%", "^", "&", "*", "?"};
  vector<string> values2 = {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
  vector<string> values3 = {"1", "abc"};

  EXPECT_EQ(values1, table::readStringColumn(1));
  EXPECT_EQ(values2, table::readStringColumn(3));
  EXPECT_EQ(values3, table::readStringColumn(4));

  table::clear();
  fs::remove(path);
}

// Тест сбора всех id в конфигурации
TEST(PARSER, GetIds) {
  string path = "test_getids.yaml";
  createTestYAML(path);

  vector<string> ids = {"sum_elements_of_column_1", "sum_elements_of_column_2",
                        "find_average_of_column_1", "find_average_of_column_2",
                        "concat"};

  config::load(path);
  EXPECT_EQ(ids, config::getIds());
  config::clear();

  fs::remove(path);
}

// Тест получения номер столбца для операции по id
TEST(PARSER, GetColumnById) {
  string path = "test_getcolumn.yaml";
  createTestYAML(path);

  config::load(path);
  EXPECT_EQ(1, config::getColumnById("sum_elements_of_column_1"));
  EXPECT_EQ(2, config::getColumnById("sum_elements_of_column_2"));
  EXPECT_EQ(1, config::getColumnById("find_average_of_column_1"));
  EXPECT_EQ(2, config::getColumnById("find_average_of_column_2"));
  EXPECT_EQ(3, config::getColumnById("concat"));
  config::clear();

  fs::remove(path);
}

// Тест получения фунции по id
TEST(PARSER, GetOpTypeById) {
  string path = "test_getfunc.yaml";
  createTestYAML(path);

  config::load(path);
  EXPECT_EQ("sum", config::getFuncById("sum_elements_of_column_1"));
  EXPECT_EQ("sum", config::getFuncById("sum_elements_of_column_2"));
  EXPECT_EQ("average", config::getFuncById("find_average_of_column_1"));
  EXPECT_EQ("average", config::getFuncById("find_average_of_column_2"));
  EXPECT_EQ("concatinate", config::getFuncById("concat"));
  config::clear();

  fs::remove(path);
}

// Тест проверки функций из файла конфигурации
TEST(PARSER, CheckFunctions) {
  string correct = "test_correct.yaml";
  string incorrect = "test_unknown.yaml";

  map<string, string> unknown = {{"unknown", "smthg"}};
  map<string, string> empty;

  createTestYAML(correct);
  config::load(correct);
  EXPECT_EQ(empty, config::checkFunctions());
  config::clear();
  fs::remove(correct);

  createYAMLWithUnknownFunc(incorrect);
  config::load(incorrect);
  EXPECT_EQ(unknown, config::checkFunctions());
  config::clear();
  fs::remove(incorrect);
}

// Тест получения пути к файлу csv для обработки
TEST(PARSER, GetCSV) {
  string path = "test_getCSV.yaml";
  string csv_path = "test.csv";

  createTestYAML(path);
  config::load(path);

  EXPECT_EQ(csv_path, config::getCSV());

  config::clear();
  fs::remove(path);
}
