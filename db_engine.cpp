#include "./include/db_engine.hpp"
#include "./include/csv_reader.hpp"
#include "./include/csv.hpp"

TableDispatch::TableDispatch(bool trace) {
    debug = trace;
}

void TableDispatch::printTokens(vector<Token>& tokens) {
    for (auto m : tokens) {
        cout<<"["<<tk_as_string[m.token]<<", "<<m.strval<<"]"<<endl;
    }
}

void TableDispatch::runQuery(string& queryAsString) {
    auto tokens = lexer.lex(queryAsString);            
    if (debug) printTokens(tokens);
    Query query = parser.parse(tokens);
    if (debug) printQuery(query);
    executeQuery(query);
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
        case QT_DELETE:
            executeDelete(query);
    }
}

void TableDispatch::executeSelect(Query query) {
    if (tables.find(query.tableName) == tables.end()) {
        cout<<"No such table: "<<query.tableName<<endl;
    } else {
        Table result = tables[query.tableName].selectRows(query);
        result.printTable();
        cout<<result.rowCount()<<" rows selected."<<endl;
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
    int count = 0;
    if (tables.find(table) == tables.end()) {
        Table ntable;
        count = ntable.addRow(query.fields, query.values);
        tables.insert(make_pair(table, ntable));
    } else {
        count = tables[table].addRow(query.fields, query.values);
    }
    cout<<count<<" Rows added to table: "<<table<<endl;
}

void TableDispatch::executeUpdate(Query query) {
    if (tables.find(query.tableName) == tables.end()) {
        cout<<"Error dispatching query: table '"<<query.tableName<<"' - could not be found."<<endl;
        return;
    }
    int count = tables[query.tableName].updateRows(query);
    cout<<"Updated: "<<count<<" records."<<endl;
}

void TableDispatch::executeDelete(Query query) {
    if (tables.find(query.tableName) == tables.end()) {
        cout<<"Error dispatching query: table '"<<query.tableName<<"' - could not be found."<<endl;
        return;
    }
    int count = tables[query.tableName].removeRows(query);
    cout<<"Removed: "<<count<<" records."<<endl;
}

void TableDispatch::loadFromCSV(string filename, string tablename) {
    CsvReader csvreader;
    CSV csv = csvreader.readFile(filename);
    Query query;
    initQuery(&query);
    query.type = QT_CREATE;
    query.tableName = tablename;
    vector<string> header;
    for (string fieldname : csv.getHeader()) {
        string name;
        for (char c : fieldname) {
            if (isalpha(c) || isdigit(c)) {
                name.push_back(c);
            } else if (c == ' ') {
                name.push_back('_');
            }
        }
        header.push_back(name);
    }
    query.fields = header;
    executeCreate(query);
    query.type = QT_INSERT;
    for (int i = 1; i < csv.rows(); i++) {
        if (csv.getRow(i).size() == query.fields.size())
            query.values.push_back(csv.getRow(i));    
    }
    printQuery(query);
    executeInsert(query);
}