#ifndef db_engine_hpp
#define db_engine_hpp
#include "table.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "parser.hpp"
#include "lexer.hpp"
using namespace std;


class TableDispatch {
    private:
        void printTokens(vector<Token>& tokens);
        void executeSelect(Query query);
        void executeCreate(Query query);
        void executeInsert(Query query);
        void executeUpdate(Query query);
        void executeDelete(Query query);
        void executeQuery(Query query);
        map<string, Table> tables;
        Lexer lexer;
        LLParser parser;
        bool debug;
    public:
        TableDispatch(bool trace = false);
        void runQuery(string& query);
        void loadFromCSV(string filename, string tablename);
};


#endif