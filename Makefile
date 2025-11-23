SRC = main.cpp \
			src/graph.cpp \
			src/parser.cpp \
			src/operations.cpp \
			src/utils.cpp

YAML = libs/Tiny_Yaml/yaml/yaml.cpp

default:
	g++ $(SRC) $(YAML) -o SimpleDAG.out

