SRC = test_parser.cpp ../../src/graph.cpp ../../src/parser.cpp ../../src/operations.cpp

YAML = ../../libs/Tiny_Yaml//yaml/yaml.cpp

GOOGLE_TEST = -lgtest -lgtest_main -lpthread

default:
	g++ $(SRC) $(YAML) $(GOOGLE_TEST) -o test_parser.out
