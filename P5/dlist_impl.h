//
// Created by 陈胖企 on 2020/11/29.
//
#ifndef PROJECT5_DLIST_IMPL_H
#define PROJECT5_DLIST_IMPL_H
#include "dlist.h"

template <class T>
bool Dlist<T>::isEmpty() const {
    // EFFECTS: returns true if list is empty, false otherwise
    return (!first);
}

template <class T>
void Dlist<T>::insertFront(T *op) {
    // MODIFIES: this
    // EFFECTS: inserts op at the front of the list
    node *np = new node;
    np->op = op;
    np->prev = nullptr;
    if (!isEmpty()){
        np->next = first;
        first->prev = np;
    }
    else{
        np->next = nullptr;
        last = np;
    }
    first = np;
}

template <class T>
void Dlist<T>::insertBack(T *op) {
    // MODIFIES: this
    // EFFECTS: inserts op at the back of the list
    node *np = new node;
    np->op = op;
    np->next = nullptr;
    if(!isEmpty()){
        np->prev = last;
        last->next = np;
    }
    else{
        np->prev = nullptr;
        np->next = nullptr;
        first = np;
    }
    last = np;
}

template <class T>
T *Dlist<T>::removeFront() {
    // MODIFIES: this
    // EFFECTS: removes and returns first object from non-empty list
    //          throws an instance of emptyList if empty
    if (isEmpty()){
        emptyList e;
        throw e;
    }
    node *victim = first;
    T *result = victim->op;
    if(victim->next == nullptr){
        first = nullptr;
        last = nullptr;
    }
    else{
        first = victim->next;
        first->prev = nullptr;
    }
    delete victim;
    return result;
}

template <class T>
T *Dlist<T>::removeBack() {
    // MODIFIES: this
    // EFFECTS: removes and returns last object from non-empty list
    //          throws an instance of emptyList if empty
    if(isEmpty()){
        emptyList e;
        throw e;
    }
    node* victim = last;
    T* result = victim->op;
    if(victim->prev == nullptr){
        first = nullptr;
        last = nullptr;
    }
    else{
        last = victim->prev;
        last->next = nullptr;
    }
    delete victim;
    return result;
}

template <class T>
Dlist<T>::Dlist()
    // constructor
    :first(0),
     last(0)
{}

template <class T>
Dlist<T>::Dlist(const Dlist<T> &l):first(0),last(0){
    // copy constructor
    copyAll(l);
}

template <class T>
Dlist<T> &Dlist<T>::operator=(const Dlist<T> &l) {
    // assignment operator
    if(this != &l){
        removeAll();
        copyAll(l);
    }
    return *this;
}

template <class T>
Dlist<T>::~Dlist(){
    // destructor
    removeAll();
}

template <class T>
void Dlist<T>::removeAll() {
    // EFFECTS: called by destructor/operator = to remove and destroy all list elements
    while(!isEmpty()){
        T *result = removeFront();
        delete result;
    }
}

template <class T>
void Dlist<T>::copyAll(const Dlist<T> &l) {
    // EFFECTS: called by copy constructor/operator= to copy elements from a source instance l to this instance
    if(!l.isEmpty()){
        node *np = l.first;
        while(np){
            T* insert = new T(*np->op);
            insertBack(insert);
            np = np->next;
        }
    }
}
#endif //PROJECT5_DLIST_IMPL_H
