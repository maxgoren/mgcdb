#include "./include/query.hpp"

void initQuery(Query* query) {
    query->type = QT_NONE;
    query->tableName = "none";
    query->order_results = false;
    query->order_on_field = "-";
    query->fields.clear();
    query->value.clear();
    query->where_clause.clear();
}

void printQuery(Query query) {
    cout<<"--------------------------"<<endl;
    cout<<"Query Type: "<<qt_as_string[query.type]<<endl;
    cout<<"Table Name: "<<query.tableName<<endl;
    cout<<"Fields: ";
    for (auto m : query.fields) {
        cout<<m<<", ";
    }
    cout<<endl;
    cout<<"Values: ";
    for (auto row : query.value) {
        for (auto val : row) {
            cout<<val<<", ";
        }
        cout<<endl;
    }
    cout<<endl;
    for (auto wc : query.where_clause) {
        cout<<"Where Clause: ";
        cout<<wc.fieldname;
        switch (wc.relop) {
            case RELOP_EQUAL: cout<<" = "; break;
            case RELOP_GREATER: cout<<" > "; break;
            case RELOP_LESS: cout<<" < "; break;
            case RELOP_NOT_EQUAL: cout<<" <> "; break;
        }
        cout<<wc.expectedValue<<endl;
    }
    if (query.order_results) {
        cout<<"Order by: "<<query.order_on_field<<endl;
    }
    cout<<"--------------------------"<<endl;
}