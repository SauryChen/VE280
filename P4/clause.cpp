//
// Created by MX Chen on 2020/11/9.
//
#include <iostream>
#include "clause.h"
using namespace std;

Clause::Clause():numLiterals(0),value(-1)
{
    // EFFECTS: constructs an empty clause.
}

void Clause::addLiteral(Literal literal){
    // MODIFIES: this
    // EFFECTS: add a literal to the current clause.
    literals[numLiterals++] = literal;
}

int Clause::evaluate(const int *val) const {
    // REQUIRES: the i-th element in val[] represents the value of the i-th variable:
    // 1(true), 0(false), or -1(unknown).
    // The length of val[] should be exactly equal to the number of variables.
    // EFFECTS: given an assignment of variables, evaluate and return the value of the clause:
    // 1: the clause evaluates to true; 0: the clause evaluates to false; -1: the value of the clause is unknown
    int evaluation;
    int count_unknown = 0;
    int count_negative = 0;
    for(unsigned int literal_number = 0; literal_number < numLiterals; literal_number++){
        if ((!literals[literal_number].negative && val[literals[literal_number].ID] == 1) ||
            (literals[literal_number].negative && val[literals[literal_number].ID] == 0)){
            evaluation = 1;
            //cout << evaluation << endl;
            return evaluation;
        }
        else if((literals[literal_number].negative && val[literals[literal_number].ID] == 1) ||
                (!literals[literal_number].negative && val[literals[literal_number].ID] == 0)){
            count_negative += 1;
        }
        else if (val[literals[literal_number].ID] == -1){
            count_unknown += 1;
        }
    }
    if(value == 1){
        evaluation = 1;
        return evaluation;
    }
    else if (value == 0){
        evaluation = 0;
        return evaluation;
    }
    else if (count_negative == numLiterals){
        evaluation = 0;
        //cout << evaluation << endl;
        return evaluation;
    }
    else if(count_unknown != 0 && (count_negative + count_unknown == numLiterals)){
        evaluation = -1;
        //cout << evaluation << endl;
        return evaluation;
    }
    else{
        cout << "Enter in Clause::evaluate.else block." << endl;
        for(int i = 0; i < numLiterals; i++){
            cout << val[i] << " ";
        }
        cout << endl;
        return 0;
    }
}


void Clause::eliminate(Literal literal) {
    // MODIFIES: this
    // EFFECTS: given a literal, simplify the current clause by assuming that this literal is true.
    // The order of the other literals in the clause keeps unchanged.
    if (literal.negative == false) {
        for (unsigned int count_literal = 0; count_literal < numLiterals; count_literal++) {
            if (literals[count_literal].ID == literal.ID && literals[count_literal].negative == false) {
                numLiterals = 0;
                value = 1;
                //Clause::print();
                break;
            }
            else if (literals[count_literal].ID == literal.ID && literals[count_literal].negative == true) {
                if (numLiterals > 1) {
                    int victim = count_literal;
                    if (victim != MAX_LITERALS) {
                        numLiterals--;
                        while (victim < numLiterals) {
                            literals[victim] = literals[victim + 1];
                            victim++;
                        }
                    }
                    //Clause::print();
                }
                else if (numLiterals == 1) {
                    numLiterals = 0;
                    value = 0;
                    //Clause::print();
                    break;
                }
            }
        }
    }
    else if (literal.negative == true){
        for (unsigned int count_literal = 0; count_literal < numLiterals; count_literal++) {
            if (literals[count_literal].ID == literal.ID && literals[count_literal].negative == true) {
                numLiterals = 0;
                value = 1;
                //Clause::print();
                break;
            } else if (literals[count_literal].ID == literal.ID && literals[count_literal].negative == false) {
                if (numLiterals == 1) {
                    numLiterals = 0;
                    value = 0;
                    //Clause::print();
                    break;
                }
                else if (numLiterals > 1) {
                    int victim = count_literal;
                    if (victim != MAX_LITERALS) {
                        numLiterals--;
                        while (victim < numLiterals) {
                            literals[victim] = literals[victim + 1];
                            victim++;
                        }
                    }
                    //Clause::print();
                }
            }
        }
    }
}

void Clause::print() const {
    // EFFECTS: print the clause in the following form:
    // If numLiterals = 0; simply print (0) or (1) according to the "value" attribute.
    // Otherwise, print the CNF in the form like: (-x0 | x1 | -x2).
    // If there is only a single literal, print like: (x0).
    if (numLiterals == 0){
        cout << "(" << value << ")";
    }
    else{
        cout << "(";
        for (int i = 0; i < numLiterals; i++){
            if (literals[i].negative == true){
                cout << "-";
            }
            cout << "x" << literals[i].ID;
            if (i < numLiterals - 1){
                cout << " | " ;
            }
        }
        cout << ")" ;
    }
}

unsigned int Clause::getNumLiterals() const {
    // EFFECTS: return numLiterals of the Clause instance.
    //cout << numLiterals;
    return numLiterals;
}

Literal Clause::getLiteral(unsigned int i) const {
    // EFFECTS: return the i-th literal of the Clause instance, i.e., this -> literals[i]
    //cout << literals[i].ID;
    //cout << literals[i].negative;
    return literals[i];
}

Clause::~Clause() {
    // EFFECTS: destruct a Clause instance.
}