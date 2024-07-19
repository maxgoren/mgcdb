#ifndef buffer_hpp
#define buffer_hp
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class FileBuffer {
    private:
        vector<string> lines;
        ifstream infile;
    public:
        FileBuffer();
        vector<string> readFile(string filename);
};

#endif