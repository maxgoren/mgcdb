#include "./include/repl.hpp"
#include "./include/lexer.hpp"
#include "./include/parser.hpp"
#include "./include/db_engine.hpp"

REPL::REPL(bool trace) {
    debug = trace;
}

void REPL::start() {
    bool running = true;
    string input;
    Lexer lex;
    LLParser parser(true);
    int i = 0;
    while (running) {
        cout<<"mgcdb> ";
        getline(cin, input);
        if (input == "quit" || input == "exit") {
            cout<<"[Goodbye.]"<<endl;
            return;
        }
        cout<<" -> "<<input<<endl;
        auto tokens = lex.lex(input);
        if (debug) printTokens(tokens);
        Query query = parser.parse(tokens);
        if (debug) printQuery(query);
        engine.executeQuery(query);
    }
}

void REPL::printTokens(vector<Token>& tokens) {
    for (auto m : tokens) {
        cout<<"["<<tk_as_string[m.token]<<", "<<m.strval<<"]"<<endl;
    }
}

