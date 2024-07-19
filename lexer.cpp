#include "./include/lexer.hpp"

Lexer::Lexer() {
    initReserved();
}
vector<Token> Lexer::lex(string str) {
    vector<Token> tokens;
    buffer.loadstring(str);
    Token current;
    while (!buffer.done()) {
        skipwhitespace();
        if (buffer.getChar() == '\'') {
            current = extractString();
        } else if (isalpha(buffer.getChar())) {
            current = extractWord();
        } else if (isdigit(buffer.getChar())) {
            current = extractNumber();
        } else {
            current = checkSpecials();
            buffer.nextChar();
        }
        if (current.token == TK_ERROR) {
            cout<<"Error during lexing of '"<<str<<"'"<<endl;
            tokens.clear();
            return tokens;
        } else {
            tokens.push_back(current);
        }
    }
    return tokens;
}

Token Lexer::checkSpecials() {
    switch (buffer.getChar()) {
        case '*': return Token(TK_STAR, "*");
        case '.': return Token(TK_PERIOD, ".");
        case ',': return Token(TK_COMMA, ",");
        case '<': { 
            buffer.nextChar();
            if (buffer.getChar() == '>')
                return Token(TK_NOT_EQUAL, "<>");
            else buffer.prevChar();
            return Token(TK_LESS, "<");
        }
        case '>': return Token(TK_GREATER, ">");
        case '=': return Token(TK_EQUAL, "=");
        case '(': return Token(TK_LPAREN, "(");
        case ')': return Token(TK_RPAREN, ")");
        case ';': return Token(TK_SEMI, ";");
    }
    return Token(TK_ERROR, "unknown symbol.");
}

Token Lexer::checkReserved(string word) {
    if (reservedWord.find(word) != reservedWord.end()) {
        return reservedWord[word];
    }
    return Token(TK_ID, word);
}

void Lexer::skipwhitespace() {
    while (buffer.getChar() == ' ' || buffer.getChar() == '\t' || buffer.getChar() == '\r')
        buffer.nextChar();
}

Token Lexer::extractNumber() {
    string num;
    while (isdigit(buffer.getChar())) {
        num.push_back(buffer.getChar());
        buffer.nextChar();
    }
    return Token(TK_NUMBER, num);
}

Token Lexer::extractWord() {
    string word;
    while (isalpha(buffer.getChar()) || buffer.getChar() == '_' || buffer.getChar() == '-') {
        word.push_back(buffer.getChar());
        buffer.nextChar();
    }
    return checkReserved(word);
}

Token Lexer::extractString() {
    string str;
    buffer.nextChar();
    while (buffer.getChar() != '\'' && !buffer.done()) {
        str.push_back(buffer.getChar());
        buffer.nextChar();
    }
    if (buffer.getChar() != '\'' && buffer.done()) {
        cout<<"Error: Unterminated string."<<endl;
    } else buffer.nextChar();
    return Token(TK_STRING, str);
}

void Lexer::initReserved() {
    reservedWord["and"] = Token(TK_AND, "and"); 
    reservedWord["as"] = Token(TK_AS, "as");
    reservedWord["by"] = Token(TK_BY, "by");
    reservedWord["create"] = Token(TK_CREATE, "create");
    reservedWord["delete"] = Token(TK_DELETE, "delete");
    reservedWord["false"] = Token(TK_FALSE, "false");
    reservedWord["from"] = Token(TK_FROM, "from");
    reservedWord["insert"] = Token(TK_INSERT, "insert");
    reservedWord["into"] = Token(TK_INTO, "into");
    reservedWord["join"] = Token(TK_JOIN, "join");
    reservedWord["order"] = Token(TK_ORDER, "order");
    reservedWord["select"] = Token(TK_SELECT, "select");
    reservedWord["set"] = Token(TK_SET, "set");
    reservedWord["table"] = Token(TK_TABLE, "table");
    reservedWord["true"] = Token(TK_TRUE, "true");
    reservedWord["update"] = Token(TK_UPDATE, "update");
    reservedWord["values"] = Token(TK_VALUES, "values");
    reservedWord["where"] = Token(TK_WHERE, "where");
}