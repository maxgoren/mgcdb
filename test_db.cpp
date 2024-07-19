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
    int i = 0;
    while (i < queries.size()) {
        input = queries[i++];
        cout<<" -> "<<input<<endl;
        engine.runQuery(input);
    }
    engine.loadFromCSV("address.csv", "address");
    input = "select * from address;";
    cout<<" -> "<<input<<endl;
    engine.runQuery(input);
}

int main() {
    vector<string> queries = {
        "create table employees (name, age, job);",
        "insert into employees (name, age, job) values ('max', 37, 'programmer');",
        "insert into employees (name, age, job) values ('platter', 22, 'developoose'), ('duckster',29,'pilot'), ('lochnoose',42,'octopoose'), ('coosemac',33,'highlander');",
        "select * from employees;",
        "select * from employees where age > 30 order by name;",
        "select * from employees where age < 30 order by age;",
        "update employees set job = 'fighter pilot' where name = 'duckster'",
        "select name, job from employees",
        "delete from employees where name = 'max';",
        "select name from employees"
    };
    startTest(queries);
    return 0;
}