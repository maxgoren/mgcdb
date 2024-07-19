#ifndef buffer_hpp
#define buffer_hp
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class StringBuffer {
    private:
        string buffer;
        int npos;
        char eosChar;
        void init(string str);
    public:
        StringBuffer();
        void loadstring(string str);
        char getChar();
        char nextChar();
        char prevChar();
        bool done();
};

#endif