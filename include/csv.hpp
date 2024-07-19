#ifndef csv_hpp
#define csv_hpp
#include <iostream>
#include <vector>
using namespace std;

class CSV {
    private:
        using Row = vector<string>;
        vector<Row> _csv;
    public:
        CSV(vector<Row>& rows) {
            _csv = rows;
        }
        CSV() {
            
        }
        int rows() {
            return _csv.size();
        }
        vector<string> getHeader() {
            return _csv[0];
        }
        vector<string> getRow(int i) {
            return _csv[i];
        }
        void appendRow(vector<string> row) {
            _csv.push_back(row);
        }
};

#endif