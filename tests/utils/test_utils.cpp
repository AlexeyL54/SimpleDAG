#include "../../include/graph.h"
#include "../../include/operations.h"
#include "../../include/parser.h"
#include "../../include/utils.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <ostream>
#include <string>

using std::fstream;
using std::string;

string readTxt(string path) {
  string result;
  fstream file(path);

  if (not file.is_open()) {
    std::cerr << "Failed to open txt file in utils test." << std::endl;
    return "";
  }
  std::getline(file, result);
  file.close();

  return result;
}

TEST(UTILS, GetSourceFiles) {
  string config, res_file;

  getSourceFiles(config, res_file);

  EXPECT_EQ(config, "test.yaml");
  EXPECT_EQ(res_file, "result.txt");
}

TEST(UTILS, WriteNumericResult) {
  string config, res_file;

  getSourceFiles(config, res_file);
  logger::openLog(res_file);
  logger::writeResult("sum", 1);

  EXPECT_EQ("sum >> 1", readTxt(res_file));

  logger::close();
  system("rm result.txt");
}

TEST(UTILS, WriteStringResult) {
  string config, res_file;

  getSourceFiles(config, res_file);
  logger::openLog(res_file);
  logger::writeResult("concatinate", "abc");

  EXPECT_EQ("sum >> abc", readTxt(res_file));

  logger::close();
  system("rm result.txt");
}

TEST(UTILS, Warning) {
  string config, res_file;
  string message = "warning";

  getSourceFiles(config, res_file);
  logger::openLog(res_file);
  logger::warning(message);

  EXPECT_EQ(message, readTxt(res_file));

  logger::close();
  system("rm result.txt");
}
