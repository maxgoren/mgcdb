#ifndef db_engine_hpp
#define db_engine_hpp
#include "table.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;


class TableDispatch {
    private:
        void executeSelect(Query query);
        void executeCreate(Query query);
        void executeInsert(Query query);
        void executeUpdate(Query query);
        void executeDelete(Query query);
        map<string, Table> tables;
    public:
        TableDispatch();
        void executeQuery(Query query);
};


#endif