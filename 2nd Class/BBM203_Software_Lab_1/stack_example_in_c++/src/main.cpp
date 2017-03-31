#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "denemeeodev2.h"
using namespace std;

// ApnStack Class member functions
template <class Apn>
ApnStack<Apn>::ApnStack(){ top = NULL; }

template <class Apn>
void ApnStack<Apn>::push(Apn param){

    apnNumber *newNode;
    newNode = new apnNumber;
    // first node
    if( top == NULL ){

        newNode->number = param.getApnNumber();
        newNode->prev = NULL;
        newNode->next = NULL;
        top = newNode;

    }

    else{

        newNode->number = param.getApnNumber();
        newNode->prev = top;
        newNode->next = NULL;
        top->next = newNode;
        top = newNode;
    }
}// push number to stack

//pop
template <class Apn>
long long ApnStack<Apn>::pop(){

    long long returnNumber;

    if(top == NULL){ return -1;}        //empty stack

    else{

        returnNumber = top->number;
        top = top->prev;
        return returnNumber;
    }

}//end pop
// end of ApnStack class member functions

// ADD
template <class Apn>
long long ApnStack<Apn>::addApnNumbers(long long param1,long long param2){

    long long result;
    result = param1 + param2;

    return result;
}


// SUBTRACTÝON
template <class Apn>
long long ApnStack<Apn>::subApnNumbers(long long param1,long long param2){

    long long result;
    result = param1 - param2;

    return result;
}


// MULTIPLY
template <class Apn>
long long ApnStack<Apn>::multiplyApnNumbers(long long param1,long long param2){

    long long result;
    result = param1 * param2;

    return result;
}

// DIVIDE
template <class Apn>
long long ApnStack<Apn>::divideApnNumbers(long long param1,long long param2){


    long long result;
    result = param1 / param2;

    return result;
}


// POWER
template <class Apn>
long long ApnStack<Apn>::powerApnNumbers(long long param1,long long param2){



    long long result = param1;

    for ( int i=1; i<param2; i++){

        result *= param1 ;
    }

    return result;
}

// end apnstack class


// Operators class membet functions
Operators::Operators(){ top = NULL; }

void Operators::pushOperator(string param){

    operatorsStruct *newNode;
    newNode = new operatorsStruct;
    // first node
    if( top == NULL ){

        newNode->processOperator = param;
        newNode->prev = NULL;
        newNode->next = NULL;
        top = newNode;

    }

    else{

        newNode->processOperator = param;
        newNode->prev = top;
        newNode->next = NULL;
        top->next = newNode;
        top = newNode;
    }
}// push operator to stack

// pop
string Operators::popOperator(){

    string returnOperator;

    if(top == NULL){ return "e";}        //empty stack

    else{

        returnOperator = top->processOperator;
        top = top->prev;
        return returnOperator;
    }

}//end pop
// end of Operators class member functions


// Apn class member functions
Apn::Apn(){ }
void Apn::setApnNumber(long long numberParam){

    number = numberParam;
}

long long Apn::getApnNumber(){

    return number;
}// end of Apn class member functions



int main( int argc, char* arg[] )
{
    Apn A;
    Operators B;
    ApnStack<Apn> X;

    long long resultOfProcesses = 0;
    string line,processOperator;
    ifstream inputFile(arg[1]);
    ofstream outputFile(arg[2]);

    if (inputFile.is_open()){

            while ( getline (inputFile,line) ){

                istringstream iss(line);

                string splitVariable;
                iss >> splitVariable;

                // reading operators and numbers
                while (iss){

                    if(splitVariable == "+" || splitVariable == "-" || splitVariable == "*" || splitVariable == "/" || splitVariable == "^"){

                        B.pushOperator(splitVariable);
                    }
                    else {

                        long long result = 0;
                        long long power = 1;
                        size_t negative = splitVariable.find("-");//control for negative

                        if(negative == NULL){

                            for(int i=splitVariable.length()-1; i>0; i--){

                                long long digit = atol((splitVariable.substr(i,1)).c_str());
                                result += digit * power;
                                power *= 10;
                            }
                                result = 0 - result;
                        }

                        else{

                            for(int i=splitVariable.length()-1; i>=0; i--){

                                long long digit = atol((splitVariable.substr(i,1)).c_str());
                                result += digit * power;
                                power *= 10;
                            }

                        }

                        A.setApnNumber(result);
                        X.push(A);
                    }
                    iss >> splitVariable;
                }

                processOperator = B.popOperator();
                while ( processOperator != "e"){

                    if ( processOperator == "+" ){

                        resultOfProcesses = X.addApnNumbers(X.pop(),X.pop());
                        A.setApnNumber(resultOfProcesses);
                        X.push(A);
                    }

                    else if ( processOperator == "*" ){

                        resultOfProcesses = X.multiplyApnNumbers(X.pop(),X.pop());
                        A.setApnNumber(resultOfProcesses);
                        X.push(A);
                    }

                    else if ( processOperator == "/" ){

                        long long division,divisionBy;
                        divisionBy = X.pop();
                        division = X.pop();

                        if( divisionBy == 0){

                            resultOfProcesses = -1;
                            outputFile << "NaN" << endl;
                            break;
                        }

                        resultOfProcesses = X.divideApnNumbers(division,divisionBy);
                        A.setApnNumber(resultOfProcesses);
                        X.push(A);
                    }

                    else if ( processOperator == "^" ){

                        long long exp = X.pop();
                        long long floor = X.pop();

                        if( exp < 0){

                            resultOfProcesses = 0;
                            A.setApnNumber(resultOfProcesses);
                            X.push(A);
                        }

                        else{

                            resultOfProcesses = X.powerApnNumbers(X.pop(),X.pop());
                            A.setApnNumber(resultOfProcesses);
                            X.push(A);
                        }
                    }

                    else if ( processOperator == "-" ){

                        resultOfProcesses = X.subApnNumbers(X.pop(),X.pop());
                        A.setApnNumber(resultOfProcesses);
                        X.push(A);
                    }

                    processOperator = B.popOperator();
                }

                if ( resultOfProcesses != -1)
                    outputFile << resultOfProcesses << endl;

            }

            outputFile.close();
            inputFile.close();
    }

    else cout << "Unable to open file";


    return 0;
}
