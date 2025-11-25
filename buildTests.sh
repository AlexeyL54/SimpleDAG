#!/bin/bash

cd tests

cd operations
make
cd ..

cd graph
make
cd ..

cd parser
make
cd ..

cd utils
make
cd ../..
