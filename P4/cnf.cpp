//
// Created by MX Chen on 2020/11/10.
//

#include <iostream>
#include "cnf.h"
#include "clause.h"
using namespace std;

CNF::CNF()
    :numClauses(0),
    numVars(0)
{
    // EFFECTS: construct an empty CNF expression.
}

void CNF::addClause(Clause cls) {
    // MODIFIES: this
    // EFFECTS: add a clause to the current CNF expression.
    clauses[numClauses++] = cls;
}

CNF CNF::eliminate(Literal literal) {
    // MODIFIES: this
    // EFFECTS: given a literal, simplify the current CNF expression by assuming that this literal is true.
    for (unsigned int num_clause = 0; num_clause < numClauses; num_clause++){
        //clauses[num_clause].print();
        clauses[num_clause].eliminate(literal);
        // clauses[num_clause].print();
    }
    numVars --;
    return *this;
}

int CNF::evaluate(const int *val) const {
    // REQUIRES: the i-th element in val[] represents the value of the i-th variables:
    // 1(true), 0(false), or -1(unknown).
    // The length of val[] should be exactly equal to the number of variables.
    // EFFECTS: given an assignment of variables, evaluate and return the value of the CNF expression:
    // 1: the CNF expression evaluates to true;
    // 0: the CNF expression evaluates to false;
    // -1: the value of CNF expression is unknown.
    int count_unknown_clauses = 0;
    int count_positive_clauses = 0;
    int evaluation_clauses;
    for (unsigned int num_clause = 0; num_clause < numClauses; num_clause++){
        int evaluation = clauses[num_clause].evaluate(val);
        if (evaluation == -1){
            count_unknown_clauses += 1;
        }
        else if (evaluation == 0){
            evaluation_clauses = 0;
            //cout << "clause "<< num_clause << " evaluate to 0" << endl;
            return evaluation_clauses;
        }
        else if (evaluation == 1){
            count_positive_clauses += 1;
        }
        else {
            cout << "This clause " << num_clause << " is neither true, false or unknown." << endl;
        }
    }
    if (count_positive_clauses == numClauses){
        evaluation_clauses = 1;
        return evaluation_clauses;
    }
    else if ((count_unknown_clauses >= 1) && (count_unknown_clauses + count_positive_clauses == numClauses)){
        evaluation_clauses = -1;
        return evaluation_clauses;
    }
    else {
        cout << "Entering the CNF.evaluation else block" << endl;
        return 0;
    }
}


bool CNF::hasUnit() const {
    // EFFECTS: check whether there exists a unit clause in this CNF expression.
    for (unsigned int num_clause = 0; num_clause < numClauses; num_clause++){
        if (clauses[num_clause].getNumLiterals() == 1){
            return true;
        }
    }
    return false;
}

Literal CNF::getUnit() const {
    // REQUIRES: the current CNF expression has at least one unit clause.
    // EFFECTS: return the only literal of the left most unit clause.
    for (unsigned int num_clause = 0; num_clause < numClauses; num_clause ++){
        if (clauses[num_clause].getNumLiterals()==1){
            return clauses[num_clause].getLiteral(0);
        }
    }
    cout << "Entering cnf.getUnit else block" << endl;
    cout << "return one randomly to eliminate warnings" << endl;
    return clauses[0].getLiteral(0);
}

CNF CNF::unitPropagate(Literal unit, int val[]) const {
    // MODIFIES: val
    //EFFECTS: given a literal and assuming it to be true, infer the value of its variable,
    //         and return a simplified CNF expression with the variable eliminated from "this" CNF expression.
    //         Note that "this" CNF expression is not changed.
    if (unit.negative){
        val[unit.ID] = 0;
        }
    else if (!unit.negative){
        val[unit.ID] = 1;
    }
    CNF modified_CNF;
    for(int count_clause = 0 ;count_clause < numClauses; count_clause ++){
        modified_CNF.addClause(clauses[count_clause]);
    }
    modified_CNF.eliminate(unit);
    modified_CNF.setNumVars(numVars - 1);
    return modified_CNF;
}

void CNF::print() const {
    // EFFECTS: print the CNF in form like: (-x0) & (x1 | x2) & (1).
    for(unsigned int count_clause = 0 ; count_clause < numClauses - 1; count_clause ++){
        clauses[count_clause].print();
        cout << " & ";
    }
    clauses[numClauses - 1].print();
    cout << endl;
}

unsigned int CNF::getNumVars() const {
    // EFFECTS: return the number of variables in the CNF expression.
    return numVars;
}

unsigned int CNF::getNumClauses() const{
    // EFFECTS: return the number of clauses in the CNF expression.
    return numClauses;
}

void CNF::setNumVars(unsigned int n) {
    // MODIFIES: this
    // EFFECTS: set the number of variables of the CNF expression.
    this->numVars = n;
}

CNF::~CNF() {
    // EFFECTS: destruct a CNF instance.
}
