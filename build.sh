#!/bin/sh
g++ -c query.cpp
g++ -c db_engine.cpp
g++ -c table.cpp
g++ -c lexer.cpp
g++ -c parser.cpp
g++ -c repl.cpp
g++ -c mgcdb.cpp
g++ *.o -o mgcdb
rm *.o
