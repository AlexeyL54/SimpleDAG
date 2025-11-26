SRC = test_operations.cpp ../../src/operations.cpp
GOOGLE_TEST = -lgtest -lgtest_main -lpthread
INCLUDE = -I../../include -I../../libs/Tiny_Yaml/yaml

default:
	g++ $(SRC) $(GOOGLE_TEST) $(INCLUDE) -o test_operations.out

clean:
	rm -f test_operations.out
