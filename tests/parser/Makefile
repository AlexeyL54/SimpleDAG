SRC = test_parser.cpp ../../src/graph.cpp ../../src/parser.cpp

YAML = ../../libs/mini-yaml/yaml/Yaml.cpp

CSV_SRC = ../../libs/csv-parser/include/internal/basic_csv_parser.cpp \
          ../../libs/csv-parser/include/internal/col_names.cpp \
          ../../libs/csv-parser/include/internal/csv_format.cpp \
          ../../libs/csv-parser/include/internal/csv_reader.cpp \
          ../../libs/csv-parser/include/internal/csv_reader_iterator.cpp \
          ../../libs/csv-parser/include/internal/csv_row.cpp \
          ../../libs/csv-parser/include/internal/csv_row_json.cpp \
          ../../libs/csv-parser/include/internal/csv_stat.cpp \
          ../../libs/csv-parser/include/internal/csv_utility.cpp
GOOGLE_TEST = -lgtest -lgtest_main -lpthread

default:
	g++ $(SRC) $(YAML) $(CSV_SRC) $(GOOGLE_TEST) -I../../libs/csv-parser/include -o test_parser.out
