//
// Created by MX Chen on 2020/11/17.
//
#include "solver.h"
#include "cnf.h"
#include "clause.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

class Enumerative : public Solver{
public:
    void solve(const CNF &cnf);
    void evaluate(const CNF &cnf, const int val[]);
};

class Deductive : public Solver{
public:
    void solve(const CNF &cnf);
    void evaluate(const CNF &cnf, const int val[]);
};


void Enumerative::solve(const CNF &cnf) {
    // EFFECTS: handle the "SAT" command. Solve if the CNF expression is satisfiable, or SAT;
    //          and output corresponding solution info.

    unsigned int numVar = cnf.getNumVars();
    bool SAT = true;
    int evaluation = 0;
    int val[numVar];
    for (unsigned count_val = 0; count_val <numVar; count_val++){
        val[count_val] = 0;
    }
    while (SAT){
        //cout << "Here" << endl;
        SAT = false;
        for(unsigned int i = 0; i < numVar; i++){
            if (val[i] == 0){
                SAT = true;
            }
        }
        //cout << "here" << endl;
        //cout << "The evaluation is " << cnf.evaluate(val) << endl;
        if(cnf.evaluate(val) == 1){
            evaluation = 1;
            cout << "The expression is SAT with one solution:" << endl;
            for(unsigned int i = 0; i < numVar - 1; i++){
                cout << val[i] << " ";
            }
            cout << val[numVar - 1] << endl;
            break;
        }
        //cout << "here" << endl;
        int displace = numVar - 1;
        while(displace >= 0){
            if (val[displace] == 1){
                val[displace]=0;
                displace--;
            }
            if (val[displace]==0) {
                val[displace]++;
                break;
            }
        }
        //cout << "The evaluation is " << cnf.evaluate(val) << endl;
        //cout << "The SAT is " << SAT << endl;
    }
    //cout << "HERE"  << endl;
    if ((SAT == false) && (evaluation == 0)){
        cout << "The expression is unSAT!" << endl;
    }
}

void Enumerative::evaluate(const CNF &cnf, const int *val) {
    int evaluation;
    evaluation = cnf.evaluate(val);
    if (evaluation == 1){
        cout << "The value of the Boolean expression is: True" << endl;
    }
    else if(evaluation == 0){
        cout << "The value of the Boolean expression is: False" << endl;
    }
    else if (evaluation == -1){
        cout << "The value of the Boolean expression is: Unknown" << endl;
    }
    else{
        cout << "Entering enumerative::evaluate.else block, no situation satisfy." << endl;
    }
}

static CNF decision(int count_val, int val[], const CNF &cnf){
    // EFFECTS: i is the unassigned variable with the smallest index
    //          val[i] is assigned to 0 or 1 in helper function and simplify the expression.
    CNF new_cnf = cnf;
    Literal eliminate_literal;
    eliminate_literal.ID = count_val;
    if (val[count_val] == 0){
        eliminate_literal.negative = true;
    }
    if (val[count_val] == 1){
        eliminate_literal.negative = false;
    }
    new_cnf.eliminate(eliminate_literal);
    return new_cnf;
}

static bool deduct_helper(int *val, const CNF &cnf){
    // EFFECTS: helper function for deductive::solver.
    //          return true if it is SAT, return false if it is unSAT.
    CNF new_cnf = cnf;
    unsigned int numVar = new_cnf.getNumVars();
    //cout << "Number of Cls is " << numCls << endl;
    //cout << "new_cnf.evaluation is " << new_cnf.evaluate(val) << endl;
    // Unit Propagate
    while(new_cnf.hasUnit()){
        Literal unit = new_cnf.getUnit();
        if(unit.negative){
            cout << "Unit propagate x" << unit.ID << " = 0:" << endl;
        }
        else{
            cout << "Unit propagate x" << unit.ID << " = 1:" << endl;
        }
        new_cnf = new_cnf.unitPropagate(unit,val);
        new_cnf.print();
        /*
        cout << "the value of val is : ";
        for(int i = 0; i< numVar; i++){
            cout << val[i] << " ";
        }
        cout << endl;
        cout << "the evaluation is: " << new_cnf.evaluate(val) << endl;
         */
    }

    // Cannot determine SAT/unSAT,
    // pick the unassigned variable with the smallest index, assign it with 0;
    if(new_cnf.evaluate(val) == 1){
        return true;
    }
    if(new_cnf.evaluate(val) == -1){
        for (unsigned int count_var = 0; count_var < numVar; count_var++){
            if (val[count_var] == -1){
                val[count_var] = 0;
                cout << "Make decision x" << count_var << " = 0:" << endl;
                CNF print_cnf_decide = decision(count_var,val,new_cnf);
                print_cnf_decide.print();
                if (deduct_helper(val,print_cnf_decide)){
                    return true;
                }
                // Modifies the previous decision
                val[count_var] = 1;
                cout << "Reverse previous decision x" << count_var << " = 1:" << endl;
                CNF print_cnf_reverse = decision(count_var,val,new_cnf);
                print_cnf_reverse.print();
                if(deduct_helper(val,print_cnf_reverse)){
                    return true;
                }
            }
        }
    }
    return false;
}

void Deductive::solve(const CNF &cnf) {
    cout << "Start deductive solver for:" << endl;
    cnf.print();
    unsigned int numVar = cnf.getNumVars();
    int val[numVar];
    for(unsigned int i = 0; i < numVar; i++){
        val[i] = -1;
    }
    //cout << "The evaluation of cnf is: " << cnf.evaluate(val) << endl;
    if(deduct_helper(val,cnf)){
        cout << "The expression is SAT!" << endl;
    }
    else{
        cout << "The expression is unSAT" << endl;
    }
}


void Deductive::evaluate(const CNF &cnf, const int *val) {
    int evaluation;
    evaluation = cnf.evaluate(val);
    if (evaluation == 1){
        cout << "The value of the Boolean expression is: True" << endl;
    }
    else if(evaluation == 0){
        cout << "The value of the Boolean expression is: False" << endl;
    }
    else if (evaluation == -1){
        cout << "The value of the Boolean expression is: Unknown" << endl;
    }
    else{
        cout << "Entering enumerative::evaluate.else block, no situation satisfy." << endl;
    }
}

static Enumerative enumer;
extern Solver *getEnum(){
    return & enumer;
}

static Deductive deduct;
extern Solver *getDeduct(){
    return & deduct;
}