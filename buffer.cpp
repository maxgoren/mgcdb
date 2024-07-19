#include "./include/stringbuffer.hpp"
#include "./include/filebuffer.hpp"
#include <vector>
using namespace std;

StringBuffer::StringBuffer() {

}

void StringBuffer::init(string str) {
    buffer = str;
    npos = 0;
    eosChar = '$';
}

void StringBuffer::loadstring(string str) {
    init(str);
}

char StringBuffer::getChar() {
    char k = npos < buffer.length() ? buffer[npos]:eosChar;
    return k;
}

char StringBuffer::nextChar() {
    if (npos+1 < buffer.length()) {
        npos++;
        return buffer[npos];
    }
    npos++;
    return eosChar;
}

char StringBuffer::prevChar() {
    if (npos-1 > -1) {
        npos--;
        return buffer[npos];
    }
    return eosChar;
}

bool StringBuffer::done() {
    return npos >= buffer.length();
}

FileBuffer::FileBuffer() {

}

vector<string> FileBuffer::readFile(string filename) {
    infile.open(filename, ios::in);
    if (!infile.is_open()) {
        cout<<"Error: couldn't open file "<<filename<<endl;
        return lines;
    }
    string current_line;
    while (infile.good()) {
        getline(infile, current_line);
        lines.push_back(current_line);
    }
    infile.close();
    return lines;
}