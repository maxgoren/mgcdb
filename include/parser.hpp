#ifndef parse_hpp
#define parse_hpp
#include <vector>
#include "query.hpp"
#include "lexer.hpp"
using namespace std;


enum LL_STEP {
    //Start symbol
    LL_STEP_TYPE, 
    //create a table
    LL_CREATE_TABLE, LL_CREATE_TABLE_NAME, LL_CREATE_TABLE_OPEN_PAREN, LL_CREATE_TABLE_FIELD_NAME, LL_CREATE_TABLE_COMMA_OR_CLOSE_PAREN,
    //parse an update statement
    LL_UPDATE_TABLE, LL_UPDATE_SET, LL_UPDATE_FIELD, LL_UPDATE_EQUALS, LL_UPDATE_VALUE, LL_UPDATE_COMMA, 
    //parse select statement
    LL_SELECT_FIELD, LL_SELECT_FIELD_AS, LL_SELECT_FIELD_AS_ALIAS, LL_SELECT_COMMA, LL_SELECT_FROM, LL_SELECT_FROM_TABLE, LL_SELECT_ORDER, LL_SELECT_ORDER_BY, LL_SELECT_ORDER_BY_FIELD,
    //parse where clause on update or select
    LL_WHERE_CLAUSE, LL_WHERE_FIELD, LL_WHERE_OPERATOR, LL_WHERE_VALUE, LL_WHERE_AND,
    //insert values into table
    LL_INSERT_INTO_TABLE, LL_INSERT_INTO_TABLE_NAME, LL_INSERT_FIELDS_OPEN_PAREN, LL_INSERT_FIELDS, 
    LL_INSERT_FIELDS_COMMA_OR_CLOSE_PAREN, LL_INSERT_VALUES_RWORD, LL_INSERT_VALUES_OPEN_PAREN,
    LL_INSERT_VALUES, LL_INSERT_VALUES_COMMA_OR_CLOSE_PAREN, LL_INSERT_VALUES_COMMA_BEFORE_OPEN_PAREN, 
    //delete rows from table
    LL_DELETE_ROWS, LL_DELETE_ROWS_FROM, LL_DELETE_ROWS_FROM_TABLE_NAME,
    //Pass/Fail parsing.
    LL_PARSE_ERROR, LL_SUCCESS
};

inline string LL_step_as_string[] = {
    "LL_STEP_TYPE", 
    "LL_CREATE_TABLE", "LL_CREATE_TABLE_NAME", "LL_CREATE_TABLE_OPEN_PAREN", "LL_CREATE_TABLE_FIELD_NAME", "LL_CREATE_TABLE_COMMA_OR_CLOSE_PAREN",
    "LL_UPDATE_TABLE", "LL_UPDATE_SET", "LL_UPDATE_FIELD", "LL_UPDATE_EQUALS", "LL_UPDATE_VALUE", "LL_UPDATE_COMMA", 
    "LL_SELECT_FIELD", "LL_SELECT_FIELD_AS", "LL_SELECT_FIELD_AS_ALIAS", "LL_SELECT_COMMA", "LL_SELECT_FROM", "LL_SELECT_FROM_TABLE", "LL_SELECT_ORDER", "LL_SELECT_ORDER_BY", "LL_SELECT_ORDER_BY_FIELD",
    "LL_WHERE_CLASE", "LL_WHERE_FIELD", "LL_WHERE_OPERATOR", "LL_WHERE_VALUE", "LL_WHERE_AND",
    "LL_INSERT_INTO_TABLE", "LL_INSERT_INTO_TABLE_NAME", "LL_INSERT_FIELDS_OPEN_PAREN", "LL_INSERT_FIELDS", 
    "LL_INSERTFIELDS_COMMA_OR_CLOSE_PAREN", "LL_INSERT_VALUES_RWORD", "LL_INSERT_VALUES_OPEN_PAREN",
    "LL_INSERT_VALUES", "LL_INSERT_VALUES_COMMA_OR_CLOSE_PAREN", "LL_INSERT_VALUES_COMMA_BEFORE_OPEN_PAREN", 
    "LL_DELETE_ROWS", "LL_DELETE_ROWS_FROM", "LL_DELETE_ROWS_FROM_TABLE_NAME",
    "LL_PARSE_ERROR", "LL_SUCCESS"
};

class LLParser {
    private:
        Query query;
        LL_STEP parseStep;
        vector<Token> tokens;
        Token currentToken;
        int tok_pos;
        bool debug;
        TK_TYPE currentSymbol();
        Token peek();
        void pop();
        void ll_error(string s);
        void init(vector<Token>& toks);
        void status();
        Query parseCreate();
        Query parseUpdate();
        Query parseInsert();
        Query parseSelect();
        Query parseDelete();
        void parseWhereClause();
    public:
        LLParser(bool trace = false);
        Query parse(vector<Token>& parse);
};

#endif