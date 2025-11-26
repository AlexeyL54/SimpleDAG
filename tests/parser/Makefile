SRC = test_parser.cpp ../../src/graph.cpp ../../src/parser.cpp ../../src/operations.cpp
YAML = ../../libs/Tiny_Yaml/yaml/yaml.cpp
GOOGLE_TEST = -lgtest -lgtest_main -lpthread
INCLUDE = -I../../include -I../../libs

default:
	g++ $(SRC) $(YAML) $(GOOGLE_TEST) $(INCLUDE) -o test_parser.out

clean:
	rm -f test_parser.out
