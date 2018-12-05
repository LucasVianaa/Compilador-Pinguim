#include <string>
#include <iostream>
#include <list>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <fstream>
#include <streambuf>
#include <map>

#include "ResultType.h"

using namespace std;

list<>
map<String, list<Variable>> SCOPES;
map<String, list<Variable>> CALLS;
list<String> OPERATIONS;

class Variable{
private:
    string name, type;
    int line, int column;
public:
    Variable(string n, string t, int l, int c){
        name = n;
        type = t;
        line = l;
        column = c;
    }

    Variable(){

    }
    int getLine(){ return line; }
    int getColumn(){ return column; }
    string getName(){ return name; }
    string getType(){ return type; }
    void setLine(int l){ line = l; }
    void setColumn(int c){ column = c; }
    void setName(string n){ name = n; }
    void setType(string t){ type = t; }    
};


static inline void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
}
static inline string rtrim_copy(string s) {
    rtrim(s);
    return s;
}
ResultType trim(string text, int line, int column){
    int i = 0;
    int l = line;
    int c = column;
    int length = text.length();
    while(i < length && (text[i] == ' ' || text[i] == '\n' || text[i] == '\t')){
        c++;
        if (text[i] == '\n')        {
            l++;
            c = 0;
        }
        i++;
    }
    int j = length - 1;
    if(i <= j){
        return ResultType(true, text.substr(i), l, c);
    }
    return ResultType(true, "", l, c);
}
bool istext(char ch){
    //[A-z`SP`0-9?!+-/*=:[]]
    return (isalpha(ch) || isdigit(ch) || ch == ' ' || ch == '?' ||
            ch == '!' ||ch == '+' || ch == '-' || ch == '/' || ch == '*' || 
            ch == '=' || ch == ':' || ch == '[' || ch == ']' || ch == '.' || ch == '<' ||
            ch == '>');
}

void build_scopes(string text_original, int l, int c){
    ResultType result = trim(text_original, l, c);
    result.setText(result.getText().substr(8));
    result = trim(result.getText(), result.getLine(), result.getColumn());
    list<Variable> global;
    ::SCOPES.insert(pair<string, list<Variable>>("global", global));

}


void analyze(String text){
    ResultType result;
    text = rtrim_copy(text);
    
    //Finds Variables in each scope
    build_scopes(text);
    //Finds Variables and Functions when called
    find_calls(text);
    //Finds Atribuitions, Mathematical and Boolean operations
    find_operations(text);

    //Verifies if multiple variables or functions exist with the same name
    result = verify_duplicates();
    print_errors(result);
    
    //Verifies if variables are used on a possible scope
    result = verify_scopes();
    print_errors(result);

    //Verifies type in atribuitions and arguments of function calls
    result = verify_assignments();
    print_errors(result);

}

void print_errors(ResultType result){

}
