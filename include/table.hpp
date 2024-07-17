#ifndef table_hpp
#define table_hpp
#include "query.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class Table {
    private:
        typedef vector<string> Row;
        map<string, unsigned int> columnIndexByName;
        vector<Row>    rows;
        int            col_cnt;
        int            row_cnt;
        int            row_id;
        unsigned int   getColumnIndex(string name);
        bool           compareRowsByField(Row& a, Row& b, string field);
        vector<string> orderedHeader();
        void           addNewFields(vector<string>& cols);
        void           validateRowWidth(vector<string>& cols, vector<string>& nrow);
        bool           hasWhereClause(Query query);
        bool           checkWhereClause(Query query, vector<string>& nrow);
        int            nextRowId();
        void           init();
        void           insertionsort(int l, int r, string field);
        void           merge(vector<Row>& buffer, int l, int m, int r, string field);
        void           mergesort(vector<Row>& buffer, int l, int r, string field);
        void           printHeader();
    public:
        Table(vector<string>& fields);
        Table();
        Table& orderBy(string field);
        void addRow(vector<string>& cols, vector<string>& values);
        void printTable();
        Table selectRows(Query query);
};


#endif