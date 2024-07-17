#ifndef lexer_hpp
#define lexer_hpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include "buffer.hpp"
using namespace std;

enum TK_TYPE {
    TK_SELECT, TK_INSERT, TK_CREATE, TK_UPDATE, TK_DELETE, TK_INTO, TK_FROM, TK_SET, TK_WHERE, TK_AND, TK_TABLE, TK_VALUES, TK_ORDER, TK_BY, TK_ID, TK_NUMBER, TK_STRING, TK_TRUE, TK_FALSE,
    TK_STAR, TK_ERROR, TK_GREATER, TK_LESS, TK_EQUAL, TK_NOT_EQUAL, TK_PERIOD, TK_COMMA,
    TK_SQUOTE, TK_LPAREN, TK_RPAREN, TK_SEMI, TK_EOS
 };

inline string tk_as_string[] = { 
    "TK_SELECT", "TK_INSERT", "TK_CREATE", "TK_UPDATE", "TK_DELETE", "TK_INTO", "TK_FROM", "TK_SET", "TK_WHERE", "TK_AND", "TK_TABLE", "TK_VALUES", "TK_ORDER", "TK_BY", "TK_ID", "TK_NUMBER", "TK_STRING", "TK_TRUE", "TK_FALSE",
    "TK_STAR", "TK_ERROR", "TK_GREATER", "TK_LESS", "TK_EQUAL", "TK_NOT_EQUAL", "TK_PERIOD", "TK_COMMA",
    "TK_SQUOTE", "TK_LPAREN", "TK_RPAREN", "TK_SEMI"
 };


struct Token {
    TK_TYPE token;
    string strval;
    Token(TK_TYPE tk, string s) {
        token = tk;
        strval = s;
    }
    Token() {
        token = TK_ERROR;
        strval = "<none>";
    }
};


class Lexer {
    private:
        Buffer buffer;
        unordered_map<string, Token> reservedWord;
        Token checkSpecials();
        Token checkReserved(string word);
        void skipwhitespace();
        Token extractNumber();
        Token extractWord();
        Token extractString();
        void initReserved();
    public:
        Lexer();
        vector<Token> lex(string str);
};


#endif