#include "ResultType.h"
#include <string>
using namespace std;

ResultType::ResultType(bool a, string t, int l, int c){
    accept = a;
    text = t;
    line = l;
    column = c;
}
ResultType::ResultType(bool a, string t, int l, int c, string err){
    accept = a;
    text = t;
    line = l;
    column = c;
    error = err;
}
ResultType::ResultType(){
    
}
int ResultType::getLine(){
    return line;
}
int ResultType::getColumn(){
    return column;
}
bool ResultType::getAccept(){
    return accept;
}
string ResultType::getText(){
    return text;
}

void ResultType::setAccept(bool a){
    accept = a;
}
void ResultType::setText(string t){
    text = t;
}
void ResultType::setLine(int l){
    line = l;
}
void ResultType::setColumn(int c){
    column = c;
}