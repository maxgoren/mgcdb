#ifndef buffer_hpp
#define buffer_hp
#include <iostream>
using namespace std;

class Buffer {
    private:
        string buffer;
        int npos;
        char eosChar;
        void init(string str) {
            buffer = str;
            npos = 0;
            eosChar = '$';
        }
    public:
        Buffer() {

        }
        void loadstring(string str) {
            init(str);
        }
        char getChar() {
            char k = npos < buffer.length() ? buffer[npos]:eosChar;
            return k;
        }
        char nextChar() {
            if (npos+1 < buffer.length()) {
                npos++;
                return buffer[npos];
            }
            npos++;
            return eosChar;
        }
        char prevChar() {
            if (npos-1 > -1) {
                npos--;
                return buffer[npos];
            }
            return eosChar;
        }
        bool done() {
            return npos >= buffer.length();
        }
};

#endif