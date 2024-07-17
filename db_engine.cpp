#include "./include/db_engine.hpp"

TableDispatch::TableDispatch() {

}

void TableDispatch::executeQuery(Query query) {
    switch (query.type) {
        case QT_CREATE:
            executeCreate(query);
            break;
        case QT_SELECT:
            executeSelect(query);
            break;
        case QT_INSERT:
            executeInsert(query);
            break;
        case QT_UPDATE:
            executeUpdate(query);
            break;
    }
}

void TableDispatch::executeSelect(Query query) {
    if (tables.find(query.tableName) == tables.end()) {
        cout<<"No such table: "<<query.tableName<<endl;
    } else {
        tables[query.tableName].selectRows(query).printTable();
    }
}

void TableDispatch::executeCreate(Query query) {
    if (tables.find(query.tableName) != tables.end()) {
        cout<<"Error creating table '"<<query.tableName<<"' - A table with that name already exists."<<endl;
        return;
    }
    Table ntable(query.fields);
    tables.insert(make_pair(query.tableName, ntable));
    cout<<"Table: "<<query.tableName<<" created."<<endl;
}

void TableDispatch::executeInsert(Query query) {
    string table = query.tableName;
    if (tables.find(table) == tables.end()) {
        Table ntable;
        ntable.addRow(query.fields, query.value);
        tables.insert(make_pair(table, ntable));
    } else {
        tables[table].addRow(query.fields, query.value);
    }
}

void TableDispatch::executeUpdate(Query query) {
    
}