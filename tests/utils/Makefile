SRC = test_utils.cpp \
      ../../src/graph.cpp \
      ../../src/operations.cpp \
      ../../src/utils.cpp \
      ../../src/parser.cpp

YAML = ../../libs/Tiny_Yaml/yaml/yaml.cpp
GOOGLE_TEST = -lgtest -lgtest_main -lpthread
INCLUDE = -I../../include -I../../libs

default:
	g++ $(SRC) $(YAML) $(GOOGLE_TEST) $(INCLUDE) -o test_utils.out

clean:
	rm -f test_utils.out
