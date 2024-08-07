#!/bin/sh
g++ -g -c query.cpp
g++ -g -c buffer.cpp
g++ -g -c csv_reader.cpp
g++ -g -c db_engine.cpp
g++ -g -c table.cpp
g++ -g -c lexer.cpp
g++ -g -c parser.cpp
g++ -c util.cpp
g++ -c repl.cpp
g++ -c mgcdb.cpp
g++ *.o -o mgcdb
rm *.o
