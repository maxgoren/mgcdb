#ifndef repl_hpp
#define repl_hpp
#include <iostream>
#include "db_engine.hpp"
using namespace std;

class REPL {
    private:
        bool debug;
        TableDispatch engine;
    public:
        REPL(bool trace = false);
        void start();
};

#endif