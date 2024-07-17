#include "./include/parser.hpp"

LLParser::LLParser(bool trace) {
    debug = trace;
}

Query LLParser::parse(vector<Token>& parse) {
    init(parse); 
    switch (currentSymbol()) {
        case TK_INSERT: {
            query.type = QT_INSERT;
            parseStep = LL_INSERT_INTO_TABLE;
            pop();
            return parseInsert();
        }
        break;
        case TK_SELECT: {
            query.type = QT_SELECT;
            parseStep = LL_SELECT_FIELD;
            pop();
            return parseSelect();
        }
        break;
        case TK_UPDATE: {
            query.type = QT_UPDATE;
            parseStep = LL_UPDATE_TABLE;
            pop();
            return parseUpdate();
        }
        break;
        case TK_CREATE:
            query.type = QT_CREATE;
            parseStep = LL_CREATE_TABLE;
            pop();
            return parseCreate();
        default:
            ll_error("unknown token: " + currentToken.strval);
            break;
    }
    return query;
}

Query LLParser::parseCreate() {
    while (currentSymbol() != TK_EOS && parseStep != LL_PARSE_ERROR) {
        status();
        switch (parseStep) {
            case LL_CREATE_TABLE:
                if (currentSymbol() == TK_TABLE) {
                    parseStep = LL_CREATE_TABLE_NAME;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error(" ");
                }
                break;
            case LL_CREATE_TABLE_NAME:
                if (currentSymbol() == TK_ID) {
                    parseStep = LL_CREATE_TABLE_OPEN_PAREN;
                    query.tableName = currentToken.strval;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error(" ");
                }
                break;
            case LL_CREATE_TABLE_OPEN_PAREN:
                if (currentSymbol() == TK_LPAREN) {
                    parseStep = LL_CREATE_TABLE_FIELD_NAME;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error(" ");
                }
                break;
            case LL_CREATE_TABLE_FIELD_NAME:
                if (currentSymbol() == TK_ID) {
                    parseStep = LL_CREATE_TABLE_COMMA_OR_CLOSE_PAREN;
                    query.fields.push_back(currentToken.strval);
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error(" ");
                }
                break;
            case LL_CREATE_TABLE_COMMA_OR_CLOSE_PAREN:
                if (currentSymbol() == TK_RPAREN) {
                    parseStep = LL_SUCCESS;
                } else if (currentSymbol() == TK_COMMA) {
                    parseStep = LL_CREATE_TABLE_FIELD_NAME;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error(" ");
                }
                break;
            default:
                break;
        }
        pop();
    }
    return query;
}

Query LLParser::parseUpdate() {
    while (currentSymbol() != TK_EOS && parseStep != LL_PARSE_ERROR) {
        status();
        switch (parseStep) {
            case LL_UPDATE_TABLE:
                if (currentSymbol() == TK_ID) {
                    parseStep = LL_UPDATE_SET;
                    query.tableName = currentToken.strval;
                }  else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect table name but found " + currentToken.strval);
                }
                break;
            case LL_UPDATE_SET:
                if (currentToken.token == TK_SET) {
                    parseStep = LL_UPDATE_FIELD;
                    cout<<"Matched 'SET'"<<endl;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect 'SET' but found " + currentToken.strval);
                }
                break;
            case LL_UPDATE_FIELD:
                parseStep = LL_UPDATE_EQUALS;
                query.fields.push_back(currentToken.strval);
                break;
            case LL_UPDATE_EQUALS:
                parseStep = LL_UPDATE_VALUE;    
                cout<<"Matched '='"<<endl;                    
                break;
            case LL_UPDATE_VALUE:
                parseStep = LL_UPDATE_COMMA;
                query.value.push_back(currentToken.strval);
                break;
            case LL_UPDATE_COMMA: {
                if (currentSymbol() == TK_COMMA) {
                    parseStep = LL_UPDATE_FIELD;
                } else if (currentSymbol() == TK_WHERE) {
                    parseStep = LL_WHERE_CLAUSE;
                    pop();
                    parseWhereClause();
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
            }
        }
        pop();
    }
    return query;
}


Query LLParser::parseSelect() {
    while (currentSymbol() != TK_EOS && parseStep != LL_PARSE_ERROR && parseStep != LL_SUCCESS) {
        status();
        switch (parseStep) {
            case LL_SELECT_FIELD:
                if (currentSymbol() == TK_STAR) {
                    query.fields.push_back(currentToken.strval);
                    parseStep = LL_SELECT_FROM;
                } else if (currentSymbol() == TK_ID) {
                    query.fields.push_back(currentToken.strval);
                    if (peek().token == TK_FROM) {
                        parseStep = LL_SELECT_FROM;
                    } else {
                        parseStep = LL_SELECT_COMMA;
                    }
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
            case LL_SELECT_COMMA:
                if (currentSymbol() == TK_COMMA) {
                    parseStep = LL_SELECT_FIELD;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Huh. Weird.");
                }
                break;
            case LL_SELECT_FROM:
                if (currentToken.token == TK_FROM) {
                    parseStep = LL_SELECT_FROM_TABLE;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Unexpected token, expected FROM but found " + currentToken.strval);
                }
                break;
            case LL_SELECT_FROM_TABLE:
                if (currentSymbol() == TK_ID) {
                    query.tableName = currentToken.strval;
                    if (peek().token == TK_WHERE) {
                        parseStep = LL_WHERE_CLAUSE;
                        pop();
                        parseWhereClause();
                    } else if (peek().token == TK_ORDER) {
                        parseStep = LL_SELECT_ORDER;
                    } else {
                        parseStep = LL_SUCCESS;
                        return query;
                    }
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Expected table name.");
                }
                break;
            case LL_SELECT_ORDER:
                if (currentSymbol() == TK_ORDER) {
                    parseStep = LL_SELECT_ORDER_BY;
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
            case LL_SELECT_ORDER_BY:
                if (currentSymbol() == TK_BY) {
                    parseStep = LL_SELECT_ORDER_BY_FIELD;
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
            case LL_SELECT_ORDER_BY_FIELD:
                if (currentSymbol() == TK_ID) {
                    query.order_results = true;
                    query.order_on_field = currentToken.strval;
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
        }
        pop();
    }
    return query;
}

Query LLParser::parseInsert() {
    while (currentSymbol() != TK_EOS && parseStep != LL_PARSE_ERROR) {
        status();
        switch (parseStep) {
            case LL_INSERT_INTO_TABLE:
                if (currentSymbol() == TK_INTO) {
                    parseStep = LL_INSERT_INTO_TABLE_NAME;
                } else {
                    ll_error("Syntax Error: Expected 'INTO', but found " + currentToken.strval);
                    parseStep = LL_PARSE_ERROR;
                }
                break; 
            case LL_INSERT_INTO_TABLE_NAME:
                if (currentSymbol() == TK_ID) {
                    query.tableName = currentToken.strval;
                    parseStep = LL_INSERT_FIELDS_OPEN_PAREN;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect TABLE NAME but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_FIELDS_OPEN_PAREN:
                if (currentSymbol() == TK_LPAREN) {
                    parseStep = LL_INSERT_FIELDS;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect '(' but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_FIELDS:
                if (currentSymbol() == TK_ID) {
                    query.fields.push_back(currentToken.strval);
                    parseStep = LL_INSERT_FIELDS_COMMA_OR_CLOSE_PAREN;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect FIELD NAME but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_FIELDS_COMMA_OR_CLOSE_PAREN:
                if (currentSymbol() == TK_COMMA) {
                    parseStep = LL_INSERT_FIELDS;
                } else if (currentSymbol() == TK_RPAREN) {
                    parseStep = LL_INSERT_VALUES_RWORD;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect ',' or ')' but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_VALUES_RWORD:
                if (currentSymbol() == TK_VALUES) {
                    parseStep = LL_INSERT_VALUES_OPEN_PAREN;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Syntax Error: Missing 'VALUES' in INSERT statement.");
                }
                break;
            case LL_INSERT_VALUES_OPEN_PAREN:
                if (currentSymbol() == TK_LPAREN) {
                    parseStep = LL_INSERT_VALUES;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched token, expected '(' but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_VALUES:
                if (currentSymbol() == TK_ID || currentSymbol() == TK_STRING || currentSymbol() == TK_NUMBER || currentSymbol() == TK_TRUE || currentSymbol() == TK_FALSE) {
                    parseStep = LL_INSERT_VALUES_COMMA_OR_CLOSE_PAREN;
                    query.value.push_back(currentToken.strval);
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect VALUES but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_VALUES_COMMA_OR_CLOSE_PAREN:
                if (currentToken.token == TK_COMMA) {
                    if (peek().token == TK_LPAREN) {
                        parseStep == LL_INSERT_VALUES_COMMA_BEFORE_OPEN_PAREN;
                    } else {
                        parseStep = LL_INSERT_VALUES;
                    }
                } else if (currentToken.token == TK_RPAREN) {
                    parseStep = LL_SUCCESS;
                    return query;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Mismatched Token, expect ',' or ')' but found " + currentToken.strval);
                }
                break;
            case LL_INSERT_VALUES_COMMA_BEFORE_OPEN_PAREN:
                    if (currentSymbol() == TK_LPAREN) {
                        parseStep = LL_INSERT_VALUES;
                    } else {
                        parseStep = LL_PARSE_ERROR;
                        ll_error("I'm not really sure. This shouldn't happen.");
                    }
                break;
            default:
                cout<<"Don't know what to do next!";
                parseStep = LL_PARSE_ERROR;
        }
        pop();
    }
    return query;
}

void LLParser::parseWhereClause() {
    WhereClause wc;
    while (currentSymbol() != TK_EOS && parseStep != LL_PARSE_ERROR) {
        status();
        switch (parseStep) {
            case LL_WHERE_CLAUSE:
                parseStep = LL_WHERE_FIELD;
                break;
            case LL_WHERE_FIELD:
                if (currentSymbol() == TK_ID) {
                    parseStep = LL_WHERE_OPERATOR;
                    wc.fieldname = currentToken.strval;
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Error parsing Where clause, expected field name, found: " + currentToken.strval);
                }
                break;
            case LL_WHERE_OPERATOR:
                if (currentSymbol() == TK_LESS || currentSymbol() == TK_GREATER || currentSymbol() == TK_EQUAL) {
                    parseStep = LL_WHERE_VALUE;
                    switch (currentSymbol()) {
                        case TK_LESS:   
                            wc.relop = RELOP_LESS;
                            break;
                        case TK_GREATER:
                            wc.relop = RELOP_GREATER;
                            break;
                        case TK_EQUAL:
                            wc.relop = RELOP_EQUAL;
                            break;
                    }
                } else {
                    parseStep = LL_PARSE_ERROR;
                    ll_error("Error parsing where clause: Expected rel op, found : " + currentToken.strval);
                }
                break;
            case LL_WHERE_VALUE:
                if (currentSymbol() == TK_ID || currentSymbol() == TK_NUMBER) {
                        wc.expectedValue = currentToken.strval;
                    if (peek().token == TK_ORDER) {
                        parseStep = LL_SELECT_ORDER;
                        query.where_clause.push_back(wc);
                        return;
                    }
                    parseStep = LL_WHERE_AND;
                } else {
                    parseStep = LL_PARSE_ERROR;
                }
                break;
            case LL_WHERE_AND:
                query.where_clause.push_back(wc);
                parseStep = LL_WHERE_FIELD;
                break;
        }
        pop();
    }
}

TK_TYPE LLParser::currentSymbol() {
    return currentToken.token;
}

Token LLParser::peek() {
    if (tok_pos+1 < tokens.size()) {
        return tokens[tok_pos+1];
    }
    return Token(TK_EOS, "$");
}

void LLParser::pop() {
    if (tok_pos + 1 < tokens.size()) {
        tok_pos++;
        currentToken = tokens[tok_pos];
        return;
    }
    tok_pos++;
    currentToken = Token(TK_EOS, "<fin>");
}

void LLParser::ll_error(string s) {
    cout<<s<<endl;
}

void LLParser::init(vector<Token>& toks) {
    tokens = toks;
    currentToken = tokens[0];
    tok_pos = 0;
    parseStep = LL_STEP_TYPE;
    initQuery(&query);
}
void LLParser::status() {
    if (debug) {
        cout<<"Parser Step:   "<<LL_step_as_string[parseStep]<<endl;
        cout<<"Parsing Token: "<<tk_as_string[currentToken.token]<<endl;
    }
}
