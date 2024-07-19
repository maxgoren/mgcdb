#include "./include/csv_reader.hpp"
#include "./include/csv.hpp"
#include "./include/util.hpp"
CsvReader::CsvReader() {

}

CSV CsvReader::readFile(string filename) {
    vector<string> lines = fb.readFile(filename);
    CSV result;
    for (string line : lines) {
        result.appendRow(split(line, ','));
    }
    return result;
}