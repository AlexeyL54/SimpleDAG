SRC = test_graph.cpp ../../src/graph.cpp
GOOGLE_TEST = -lgtest -lgtest_main -lpthread
INCLUDE = -I../../include -I../../libs

default:
	g++ $(SRC) $(GOOGLE_TEST) $(INCLUDE) -o test_graph.out

clean:
	rm -f test_graph.out
