#ifndef DENEMEEODEV2_H_INCLUDED
#define DENEMEEODEV2_H_INCLUDED
using namespace std;
// apn number's class
class Apn {

private:
    long long number;
public:
    Apn();
    void setApnNumber(long long);
    long long getApnNumber();
};


//Operators' class
class Operators{

private:
    struct operatorsStruct{
        string processOperator;  // operator value
        operatorsStruct *prev;
        operatorsStruct *next;  // next stack element
    };

    operatorsStruct *top;   // top of stack

public:
    Operators();
    void pushOperator(string);
    string popOperator();

};


template <class Apn>
class ApnStack {

private:
    struct apnNumber{
        long long number;  // number value
        apnNumber *prev;
        apnNumber *next;  // next stack element
    };
    apnNumber *top;

public:

    ApnStack();
    void push(Apn);  // push element
    long long pop();   // pop element
    long long addApnNumbers(long long,long long);
    long long subApnNumbers(long long,long long);
    long long multiplyApnNumbers(long long,long long);
    long long divideApnNumbers(long long,long long);
    long long powerApnNumbers(long long,long long);

};
#endif // DENEMEEODEV2_H_INCLUDED


