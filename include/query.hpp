#ifndef query_hpp
#define query_hpp
#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum RelationalOperator {
    RELOP_LESS, RELOP_GREATER, RELOP_EQUAL, RELOP_NOT_EQUAL
};

struct WhereClause {
    string fieldname;
    RelationalOperator relop;
    string expectedValue;
};


enum QueryType {
    QT_CREATE, QT_INSERT, QT_DELETE, QT_SELECT, QT_UPDATE, QT_NONE
};

inline string qt_as_string[] = {"CREATE", "INSERT", "QT_DELETE", "SELECT", "UPDATE", "NONE"};

struct Query {
    QueryType type;
    string tableName;
    bool order_results;
    string order_on_field;
    map<string, string> fieldAliases;
    vector<string> fields;
    vector<vector<string>> values;
    vector<WhereClause> where_clause;
};

void initQuery(Query* query);
void printQuery(Query query);

#endif