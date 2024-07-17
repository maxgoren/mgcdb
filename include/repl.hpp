#ifndef repl_hpp
#define repl_hpp
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "db_engine.hpp"
using namespace std;

class REPL {
    private:
        void printTokens(vector<Token>& tokens);
        bool debug;
        TableDispatch engine;
    public:
        REPL(bool trace = false);
        void start();
};

#endif