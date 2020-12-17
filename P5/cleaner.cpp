#include "dlist.h"
#include <iostream>
#include <string>
using namespace std;

struct Find_Bracket{
    // If bool add is true, then it is the add bracket; otherwise it is the del bracket;
    // If bool other is true, then it not add nor del bracket.
    unsigned int position;
    bool add;
    bool other;
};

struct Add_Pos{
    unsigned int begin_pos;
    unsigned int end_pos;
};

struct Del_Pos{
    unsigned int begin_pos;
    unsigned int end_pos;
};

int main(){
    // Initialize two lists.
    Dlist<Find_Bracket> *bracket_position = new Dlist<Find_Bracket>;
    Add_Pos add_pos={0,0}; // to install the position of add after pop out from bracket_position
    Del_Pos del_pos={0,0}; // to install the position of del after pop out from bracket_position
    Find_Bracket find_bracket_add={0,true,false};
    Find_Bracket find_bracket_del={0, false,false};
    Find_Bracket find_bracket_other={0,false,true};

    unsigned int add_num;
    cin >> add_num;
    string *add_commands = new string[add_num];
    for (unsigned int i = 0; i < add_num; i++){
        cin >> add_commands[i];
    }
    unsigned int del_num;
    cin >> del_num;
    string *del_commands = new string[del_num];
    for (unsigned int i = 0; i < del_num; i++){
        cin >> del_commands[i];
    }
    // read lines by char
    getchar();
    string line;
    while(getline(cin,line)){
        char iterator;
        for(unsigned int iterate_line = 0; iterate_line < line.length(); iterate_line++){
            bool judege_add = false;
            bool judge_delete = false;
            iterator = line[iterate_line];
            if(iterator == '%'){
                if (iterate_line == 0){
                    break;
                }
                else if ((iterate_line >= 1) && (line[iterate_line-1] != '\\')){
                    break;
                }
            }
            else if(iterator == '{'){
                if(iterate_line <= 4){
                    continue;
                }
                else if(iterate_line == 5){
                    string commands = line.substr(iterate_line-5,5);
                    for (unsigned int iterate_add_commands = 0; iterate_add_commands < add_num; iterate_add_commands ++){
                        if(commands == add_commands[iterate_add_commands]){
                            find_bracket_add = {0,true,false};
                            find_bracket_add.position = iterate_line;
                            find_bracket_add.add = true;
                            find_bracket_add.other = false;
                            bracket_position->insertBack(&find_bracket_add);
                            /*
                            cout << "insert" << endl;
                            cout << "iterate_line " << iterate_line << endl;
                            Find_Bracket *checkfive = bracket_position->removeBack();
                            cout << "the five " << checkfive->position << endl;
                            bracket_position->insertBack(checkfive);
                            cout << "Has inserted" << endl;
                             */
                            judege_add = true;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    for (unsigned int iterate_del_commands = 0 ; iterate_del_commands < del_num; iterate_del_commands ++){
                        if(commands == del_commands[iterate_del_commands]){
                            find_bracket_del={0, false,false};
                            find_bracket_del.position = iterate_line;
                            find_bracket_del.add = false;
                            find_bracket_del.other = false;
                            bracket_position->insertBack(&find_bracket_del);
                            judge_delete = true;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    if((judege_add == false) && (judge_delete == false)){
                        find_bracket_other={0,false,true};
                        find_bracket_other.position = iterate_line;
                        find_bracket_other.add = false;
                        find_bracket_other.other = true;
                        bracket_position->insertBack(&find_bracket_other);
                    }
                }
                else if (line[iterate_line - 6]!='%'){
                    string commands = line.substr(iterate_line-5,5);
                    for (unsigned int iterate_add_commands = 0; iterate_add_commands < add_num; iterate_add_commands ++){
                        if(commands == add_commands[iterate_add_commands]){
                            Find_Bracket find_bracket_add = {0,true,false};
                            find_bracket_add.position = iterate_line;
                            find_bracket_add.add = true;
                            find_bracket_add.other = false;
                            bracket_position->insertBack(&find_bracket_add);
                            judege_add = true;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    for (unsigned int iterate_del_commands = 0 ; iterate_del_commands < del_num; iterate_del_commands ++){
                        if(commands == del_commands[iterate_del_commands]){
                            Find_Bracket find_bracket_del={0, false,false};
                            find_bracket_del.position = iterate_line;
                            find_bracket_del.add = false;
                            find_bracket_del.other = false;
                            bracket_position->insertBack(&find_bracket_del);
                            judge_delete = true;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    if((judege_add == false) && (judge_delete == false)){
                        //cout << "Find other bracket" << endl;
                        Find_Bracket find_bracket_other={0,false,true};
                        find_bracket_other.position = iterate_line;
                        find_bracket_other.add = false;
                        find_bracket_other.other = true;
                        bracket_position->insertBack(&find_bracket_other);
                    }
                }
                else{
                    //cout << "Find other bracket" << endl;
                    Find_Bracket find_bracket_other={0,false,true};
                    find_bracket_other.position = iterate_line;
                    find_bracket_other.add = false;
                    find_bracket_other.other = true;
                    bracket_position->insertBack(&find_bracket_other);
                }
            }

            else if((iterator == '}') && (!bracket_position->isEmpty())){
                Find_Bracket *bracket = bracket_position->removeBack();
                //cout << bracket->add << " " << bracket->position << " " << bracket->other << endl;
                if((bracket->add) && (!bracket->other)){
                    add_pos.begin_pos = bracket->position;
                    add_pos.end_pos = iterate_line;
                    //cout << "ADD begin pos is " << add_pos.begin_pos << endl;
                    //cout << "ADD end pos is " << add_pos.end_pos << endl;
                    line.erase(add_pos.end_pos, 1);
                    line.erase(add_pos.begin_pos - 5, 6);
                    iterate_line = iterate_line - 7;
                }
                else if((!bracket->add) && (!bracket->other)){
                    del_pos.begin_pos = bracket->position;
                    del_pos.end_pos = iterate_line;
                    //cout << "DEL begin pos is " << del_pos.begin_pos << endl;
                    //cout << "DEL end pos is " << del_pos.end_pos << endl;
                    unsigned int erase_length = del_pos.end_pos - del_pos.begin_pos + 6;
                    line.erase(del_pos.begin_pos - 5, erase_length);
                    iterate_line = iterate_line - erase_length - 1;
                }
                else if (bracket->other){
                    continue;
                }
            }
        }
        cout << line << endl;
        line.clear();
    }
    delete [] add_commands;
    delete [] del_commands;
    delete bracket_position;
    return 0;
}