#ifndef csv_reader_hpp
#define csv_reader_hpp
#include <iostream>
#include <vector>
#include "filebuffer.hpp"
#include "csv.hpp"
using namespace std;

class CsvReader {
    private:
        FileBuffer fb;
    public:
        CsvReader();
        CSV readFile(string filename);
};

#endif