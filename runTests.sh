#!/bin/bash

cd tests

cd operations
./test_operations.out
cd ..

cd graph
./test_graph.out
cd ..

cd parser
./test_parser.out
cd ..

cd utils
./test_utils.out
cd ../..
