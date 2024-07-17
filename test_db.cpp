#include <iostream>
#include <vector>
#include "./include/lexer.hpp"
#include "./include/parser.hpp"
#include "./include/db_engine.hpp"
using namespace std;

void printTokens(vector<Token>& tokens) {
    for (auto m : tokens) {
        cout<<"["<<tk_as_string[m.token]<<", "<<m.strval<<"]"<<endl;
    }
}

void startTest(vector<string>& queries) {
    TableDispatch engine;
    bool running = true;
    string input;
    Lexer lex;
    LLParser parser(false);
    int i = 0;
    while (running) {
        input = queries[i++];
        if (input == "quit" || input == "exit") {
            cout<<"[Goodbye.]"<<endl;
            return;
        }
        cout<<" -> "<<input<<endl;
        auto tokens = lex.lex(input);
        //printTokens(tokens);
        Query query = parser.parse(tokens);
        printQuery(query);
        engine.executeQuery(query);
    }
}

int main() {
    vector<string> queries = {
        "insert into boston (name, age, job) values (max, 37, programmer);",
        "insert into boston (name, age, job) values (platter, 2, developoose);",
        "insert into boston (name, age, job) values (duckster,11,pilot);",
        "insert into boston (name, age, job) values (lochnoose,2,octopoose);",
        "insert into boston (name, age, job) values (coosemac,3,highlander);",
        "select * from boston;",
        "select * from boston where age < 36 order by name;",
        "select * from boston where age < 36 order by age;",
        "create table shamire (job,stone,last);",
        "select * from shamire;",
        "insert into shamire (job,stone,last) values (quetzacotl,quartz,1000);",
        "select * from shamire;",
        "quit"
    };
    startTest(queries);
    return 0;
}