#include <string>
#include <iostream>
#include <list>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "ResultType.h"
using namespace std;


// Trim functions (in place and copying)

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
bool not_letter_or_number(char ch){
    return (!isalpha(ch) && !isdigit(ch));
}
ResultType bloco_instrucoes(string text_original, int l, int c){
    return ResultType(true, text_original, l, c);
}
ResultType bloco_funcoes(string text_original, int l, int c){
    return ResultType(true, text_original, l, c);
}
ResultType bloco_declaracoes(string text_original, int l, int c){
    return ResultType(true, text_original, l, c);
}

ResultType programa(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();

    if(text.length() > 10 && text.substr(0,10) == "Declare{};" && !isalpha(text[7])){
        result = bloco_declaracoes(text.substr(10), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
        text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        if(text.length() >= 12 && text.substr(0,12) == "Functions{};"){
            result = bloco_funcoes(text.substr(12), line, column);
            if(!result.getAccept()){
                return result;
            }
            trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
            if(text.length() > 6 && text.substr(0,6) == "Main{}"){
                result = bloco_instrucoes(text.substr(6), line, column);
                if(!result.getAccept()){
                    return result;
                }   
                trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
                if(text.substr(0,1) == ";"){
                    trimmed.setAccept(true); trimmed.setText(text.substr(1));
                    return result;
                }
            }else{
                result = trim(result.getText(), result.getLine(), result.getColumn()); result.setAccept(false);
                return result;
            }
        }else{
            result = trim(result.getText(), result.getLine(), result.getColumn()); result.setAccept(false);
            return result;
        }
    }else{
        result = trim(result.getText(), result.getLine(), result.getColumn()); result.setAccept(false);
        return result;
    }
}

bool recognizes(string text){
    ResultType result = programa(text, 1, 0);
    return result.getAccept();  
}

int main(int argc, char const *argv[]){
    /* Implement recognize */
    ResultType result = programa("Declare{};\n Functions{};\nMain};", 1, 0);
    cout << result.getAccept() << endl;
    cout << result.getText() << endl;
    cout << result.getLine() << endl;
    cout << result.getColumn() << endl;
    return 0;
}