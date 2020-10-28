#include <iostream>
#include "io.h"
#include <math.h>

int main() {
    string prompt_montly_payment = "Please enter the monthly payment: ";
    string prompt_rate = "Please enter the interest rate: ";
    string prompt_duration = "Please enter the duration of the loan, in years: ";

    double monthly_payment = GetParam(prompt_montly_payment, 1, 100000);
    double interest_rate = GetParam(prompt_rate, 0, 1);
    double duration = GetParam(prompt_duration, 1, 100); // duration有小数情况

    double input_month = 12 * duration;

    int total_month = ceil(input_month);

    PrintHeader();

    double balance = 0;
    double principal = 0;
    double interest = 0;


    for (int month = total_month; month >= 1; month--)
    {
        balance = (balance + monthly_payment)/(1+interest_rate/12);
        interest = balance * (interest_rate/12);
        principal = monthly_payment - interest;
        PrintMonthlyData(month, principal, interest, balance);
    }

    return 0;
}