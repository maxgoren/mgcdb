#include "./include/repl.hpp"
#include "./include/db_engine.hpp"
#include "./include/util.hpp"

REPL::REPL(bool trace) {
    debug = trace;
    engine = TableDispatch(debug);
}

void REPL::start() {
    bool running = true;
    string input;
    int i = 0;
    while (running) {
        cout<<"mgcdb> ";
        getline(cin, input);    
        cout<<" -> "<<input<<endl;
        if (input == "quit" || input == "exit") {
            cout<<"[Goodbye.]"<<endl;
            return;
        } else if (input.substr(0, 4) == "load") {
            string filename = split(input, ' ')[1];
            string tablename = split(filename, '.')[0];
            engine.loadFromCSV(filename, tablename);
        } else {
            engine.runQuery(input);
        }
    }
}

