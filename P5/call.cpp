#include "dlist.h"
#include <iostream>
#include <string>
using namespace std;

struct customer{
    int timestamp;
    string name;
    string status;
    int duration;
};

struct caller{
    int call_time;
    customer* get_serve;
    Dlist<customer>* member_type;
};

struct total_list{
    Dlist <customer> regular_list;
    Dlist <customer> silver_list;
    Dlist <customer> gold_list;
    Dlist <customer> platinum_list;
};

static customer* check_serve(Dlist<customer> &customer_list, int tick){
    if(!customer_list.isEmpty()){
        customer *first_one = customer_list.removeFront();
        customer_list.insertFront(first_one);
        if(first_one->timestamp <= tick) {
            return first_one;
        }
    }
    return nullptr;
}

static void print_time(int tick){
    cout << "Starting tick #" << tick << endl;
}

int main(){
    total_list total;
    unsigned int number_of_events;
    cin >> number_of_events;
    caller answer={0, nullptr, nullptr};
    customer *customer_list = new customer[number_of_events];
    for(unsigned int i = 0; i < number_of_events; i++){
        customer* input_customer = new customer;
        unsigned int time = 0;
        cin >> time;
        string name_status[2]={"name","status"};
        for (unsigned int j = 0; j < 2; j++){
            cin >> name_status[j];
        }
        int call_duration = 0;
        cin >> call_duration;
        input_customer->timestamp = time;
        input_customer->name = name_status[0];
        input_customer->status = name_status[1];
        input_customer->duration = call_duration;
        customer_list[i] = *input_customer;

        if(input_customer->status == "regular"){
            total.regular_list.insertBack(input_customer);
        }
        if(input_customer->status == "silver"){
            total.silver_list.insertBack(input_customer);
        }
        if(input_customer->status == "gold"){
            total.gold_list.insertBack(input_customer);
        }
        if(input_customer->status == "platinum"){
            total.platinum_list.insertBack(input_customer);
        }
    }

    int tick = 0;
    while((!total.regular_list.isEmpty()) || (!total.silver_list.isEmpty()) || (!total.gold_list.isEmpty()) || (!total.platinum_list.isEmpty())){
        print_time(tick);
        unsigned int count_customer = 0;
        while(count_customer < number_of_events){
            if(customer_list[count_customer].timestamp == tick){
                cout << "Call from " << customer_list[count_customer].name << " a " << customer_list[count_customer].status << " member" << endl;
            }
            count_customer ++;
        }

        if(!answer.get_serve){
            if(check_serve(total.platinum_list,tick)){
                answer.call_time = tick;
                answer.get_serve = check_serve(total.platinum_list,tick);
                answer.member_type  = &total.platinum_list;
                cout << "Answering call from " << answer.get_serve->name << endl;
            }
            else if(check_serve(total.gold_list,tick)){
                answer.call_time = tick;
                answer.get_serve = check_serve(total.gold_list,tick);
                answer.member_type  = &total.gold_list;
                cout << "Answering call from " << answer.get_serve->name << endl;
            }
            else if(check_serve(total.silver_list,tick)){
                answer.call_time = tick;
                answer.get_serve = check_serve(total.silver_list,tick);
                answer.member_type  = &total.silver_list;
                cout << "Answering call from " << answer.get_serve->name << endl;
            }
            else if(check_serve(total.regular_list,tick)){
                answer.call_time = tick;
                answer.get_serve = check_serve(total.regular_list,tick);
                answer.member_type  = &total.regular_list;
                cout << "Answering call from " << answer.get_serve->name << endl;
            }
        }
        if(answer.get_serve){
            if((tick - answer.call_time + 1) >= answer.get_serve->duration){
                customer* served = answer.member_type->removeFront();
                answer.get_serve = nullptr;
                delete served;
            }
        }
        tick ++;
    }
    print_time(tick);
    delete[] customer_list;
    return 0;
}