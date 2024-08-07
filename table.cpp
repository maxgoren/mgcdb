#include "./include/table.hpp"

Table::Table(vector<string>& fields) {
    init();
    addNewFields(fields);
}

Table::Table() {
    init();
}

int Table::rowCount() {
    return rows.size();
}

bool Table::hasColumn(string name) {
    return columnIndexByName.find(name) != columnIndexByName.end();
}

bool Table::validateColumnsExist(Query query) {
    for (string column : query.fields) {
        if (!hasColumn(column)) {
            cout<<"Error: no such column: "<<column<<endl;
            return false;
        }
    }
    return true;
}

Table Table::selectRows(Query query) {
    if (query.fields.size() == 1 && query.fields[0] == "*") {
        query.fields.clear();
        query.fields = orderedHeader();
    }
    if (validateColumnsExist(query) == false) {
        return Table();
    }
    Table result_set;
    for (auto row : rows) {
        Row nrow;
        for (auto field : query.fields) {
            int nextColumn = getColumnIndex(field);
            if (nextColumn < row.size()) {
                nrow.push_back(row[nextColumn]);
            } else {
                nrow.push_back("<nil>");
            }
        }
        if (checkWhereClause(query, row)) {
            vector<Row> tmp;
            tmp.push_back(nrow);
            if (!query.fieldAliases.empty()) {
                vector<string> tableHeader;
                for (string fa : query.fields) {
                    if (query.fieldAliases.find(fa) != query.fieldAliases.end()) {
                        tableHeader.push_back(query.fieldAliases[fa]);
                    } else {
                        cout<<"Invalid column name provided: "<<fa<<endl;
                        return Table();
                    }
                }
                result_set.addRow(tableHeader, tmp);
            } else {
                result_set.addRow(query.fields, tmp);
            }
        }
    }
    if (query.order_results) {
        result_set.orderBy(query.order_on_field);
    }
    return result_set;
}

int Table::updateRows(Query query) {
    int updated = 0;
    for (Row & row : rows) {
        if (checkWhereClause(query, row)) {
            row.at(getColumnIndex(query.fields[0])) = query.values[0][0];
            updated++;
        }
    }
    return updated;
}

int Table::removeRows(Query query) {
    vector<string> toRemove;
    for (Row row : rows) {
        if (checkWhereClause(query, row))
            toRemove.push_back(row[getColumnIndex("id")]);
    }
    for (auto victim : toRemove)
        eraseRowById(victim);
    return toRemove.size();
}

void Table::eraseRowById(string id) {
    int pos = 0;
    for (Row row : rows) {
        if (id == row[getColumnIndex("id")])
            break;
        pos++;
    }
    rows.erase(rows.begin()+pos);
}

Table& Table::orderBy(string field) {
    vector<Row> buff(rows.size(), vector<string>(col_cnt));
    mergesort(buff, 0, rows.size(), field);
    return *this;
}

int Table::addRow(vector<string>& cols, vector<vector<string>>& values) {
    if (values.empty() || cols.size() != values[0].size()) {
        cout<<"Error: column count doesnt match value count."<<endl;
        return 0;
    }
    for (int i = 0; i < cols.size(); i++) {
        if (columnIndexByName.find(cols[i]) == columnIndexByName.end()) {
            columnIndexByName[cols[i]] = col_cnt++;
        }
    }
    for (vector<string> vals : values) {
        Row nrow;
        validateRowWidth(cols, nrow);
        nrow.at(getColumnIndex("id")) = to_string(nextRowId());
        for (int i = 0; i < cols.size(); i++) {
            nrow.at(getColumnIndex(cols[i])) = vals[i];
        }
        rows.push_back(nrow);
    }
    return values.size();
}

void Table::printTable() {
    printHeader();
    int i = 0;
    for (auto row : rows) {
        cout<<"(";
        i = 0;
        while (i < row.size() - 1) {
            cout<<row[i]<<", ";
            i++;
        }
        cout<<row[i]<<")"<<endl;
    }
}

void Table::printHeader() {
    int n = 0;
    cout<<" ";
    for (string name : orderedHeader()) {
        cout<<name<<", ";
        n += (name.length() + 2);
    }
    cout<<endl;
    for (int i = 0; i < n; i++)
        cout<<"-";
    cout<<endl;
}

unsigned int Table::getColumnIndex(string name) {
    return columnIndexByName[name];
}

bool Table::compareRowsByField(Row& a, Row& b, string field) {
    string lhs = a[getColumnIndex(field)];
    string rhs = b[getColumnIndex(field)];
    if (isdigit(lhs[0]) && isdigit(rhs[0])) {
        return stod(lhs) < stod(rhs);
    }
    return lhs < rhs;
}

vector<string> Table::orderedHeader() {
    vector<pair<unsigned int, string>> cn;
    for (auto m : columnIndexByName) {
        cn.push_back(make_pair(m.second, m.first));
    }
    sort(cn.begin(), cn.end());
    vector<string> header;
    for (auto m : cn)
        header.push_back(m.second);
    return header;
}

void Table::addNewFields(vector<string>& cols) {
    for (int i = 0; i < cols.size(); i++) {
        if (columnIndexByName.find(cols[i]) == columnIndexByName.end()) {
            columnIndexByName[cols[i]] = col_cnt++;
        }
    }
} 

void Table::validateRowWidth(vector<string>& cols, vector<string>& nrow) {
    addNewFields(cols);
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].size() < col_cnt) {
            while (rows[i].size() < col_cnt) {
                rows[i].push_back("<nil>");
            }
        }
    }
    nrow.resize(col_cnt);
}


 bool Table::hasWhereClause(Query query) {
    return query.where_clause.size() > 0;
}

bool Table::checkWhereClause(Query query, vector<string>& nrow) {
    if (!hasWhereClause(query))
        return true;
    WhereClause wc = query.where_clause[0];
    string value = nrow[getColumnIndex(wc.fieldname)];
    string expected = wc.expectedValue;
    switch (wc.relop) {
        case RELOP_LESS: {
            string lhs = value;
            string rhs = expected;
            if (isdigit(lhs[0]) && isdigit(rhs[0])) {
                return stod(lhs) < stod(rhs);
            }
            return lhs < rhs;
        }
        case RELOP_GREATER: {
            string lhs = value;
            string rhs = expected;
            if (isdigit(lhs[0]) && isdigit(rhs[0])) {
                return stod(lhs) > stod(rhs);
            }
            return lhs > rhs;
        }
        case RELOP_EQUAL:
            return (expected == value);
        case RELOP_NOT_EQUAL:
            return (expected != value);
        default:
            break;
    }
    return true;
}

int Table::nextRowId() {
    return ++row_id;
}

void Table::init() {
    col_cnt = 0;
    row_cnt = 0;
    row_id = 0;
    columnIndexByName["id"] = col_cnt++;
}

void Table::insertionsort(int l, int r, string field) {
    for (int i = l; i < r; i++) {
        int j = i; auto v = rows[j];
        while (j > l && compareRowsByField(v, rows[j-1], field)) {
            rows[j] = rows[j - 1];
            j--;
        }
        rows[j] = v;
    }
}

void Table::merge(vector<Row>& buffer, int l, int m, int r, string field)  {
    for (int k = l; k < r; k++) {
        buffer[k] = rows[k];
    }
    for (int i = l, j = m, k = l; k < r; k++) {
        if (i == m) { rows[k] = buffer[j++]; continue; }
        if (j == r) { rows[k] = buffer[i++]; continue; }
        rows[k] = (compareRowsByField(buffer[i], buffer[j], field)) ? buffer[i++]:buffer[j++];
    }
}

void Table::mergesort(vector<Row>& buffer, int l, int r, string field) {
    if (r-l <= 3) {
        insertionsort(l, r, field);
        return;
    }
    int m = (l+r)/2;
    mergesort(buffer, l, m, field);
    mergesort(buffer, m, r, field);
    merge(buffer, l,m,r, field);
}