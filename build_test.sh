#!/bin/sh
g++ -g -c query.cpp
g++ -g -c buffer.cpp
g++ -g -c csv_reader.cpp
g++ -g -c db_engine.cpp
g++ -g -c table.cpp
g++ -g -c lexer.cpp
g++ -g -c util.cpp
g++ -g -c parser.cpp
g++ -g -c test_db.cpp
g++ -g *.o -o testdb
rm *.o
