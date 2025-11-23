SRC = test_utils.cpp \
			../../src/graph.cpp \
			../../src/operations.cpp \
			../../src/utils.cpp \
			../../src/parser.cpp

YAML = ../../libs/Tiny_Yaml/yaml/yaml.cpp

GOOGLE_TEST = -lgtest -lgtest_main -lpthread

default:
	g++ $(SRC) $(YAML) $(CSV_SRC) $(GOOGLE_TEST) -I../../libs/csv-parser/include -o test_utils.out

