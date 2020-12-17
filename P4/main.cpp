#include "clause.h"
#include "cnf.h"
#include "solver.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]){
    Solver *solver;
    if (string(argv[1]) == "ENUM"){
        solver = getEnum();
    }
    else{
        solver = getDeduct();
    }
    int numVar;
    int numCls;
    cin >> numVar >> numCls;
    //cout << "Num var is " << numVar <<endl;
    //cout << "Num clause is " << numCls << endl;
    CNF cnf;
    cnf.setNumVars(numVar);
    // Don't know why but here exists a null cin.
    string useless;
    getline(cin,useless);

    for(int i = 0; i < numCls; i++) {
        string line;
        //cout << "here" << endl;
        getline(cin, line);
        //cout << line << endl;
        //cout << line.length() << endl;
        Clause add_cls;
        Literal add_lit;
        for (int i = 0; i < line.length(); i++) {
            //cout << "i is " << i << ", line[i] is " << line[i] << endl;
            int id;
            bool really_useless;
            stringstream ss;
            if (line[i] == 'x') {
                i++;
                ss << line[i];
                ss >> id;
                add_lit.ID = id;
                add_lit.negative = false;
                ss.clear();
                add_cls.addLiteral(add_lit);
                /*
                cout << "Print cls: ";
                add_cls.print();
                cout << endl;
                 */
            }
            else if (line[i] == ' ' || line[i] == '|') {
                // 假装无事发生
                really_useless = true;
            }
            else if (line[i] == '-') {
                i += 2;
                ss << line[i];
                ss >> id;
                add_lit.ID = id;
                add_lit.negative = true;
                ss.clear();
                add_cls.addLiteral(add_lit);
                /*
                cout << "Print cls: ";
                add_cls.print();
                cout << endl;
                 */
            }
        }
        cnf.addClause(add_cls);
    }

    string command;
    cin >> command;
    while(command != "EXIT"){
        if (command == "PRINT"){
            cnf.print();
        }
        if (command == "EVAL"){
            int val[numVar];
            for (int num = 0; num < numVar; num ++){
                cin >> val[num];
            }
            solver->evaluate(cnf,val);
        }
        if (command == "SAT"){
            solver->solve(cnf);
        }
        cin >> command;
    }
    if (command == "EXIT"){
        cout << "Bye!" << endl;
    }
    return 0;
}
