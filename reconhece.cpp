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


ResultType programa(string text_original, int l, int c);
ResultType lista_instrucoes(string text_original, int l, int c);
ResultType lista_funcoes(string text_original, int l, int c);
ResultType lista_declaracoes(string text_original, int l, int c);
ResultType bloco(string text_original, int l, int c);
ResultType instrucao_iden(string text_original, int l, int c);
ResultType se_senao(string text_original, int l, int c);
ResultType tipo(string text_original, int l, int c);
ResultType parametros(string text_original, int l, int c);
ResultType lista_parametros(string text_original, int l, int c);
ResultType atribuicao(string text_original, int l, int c);
ResultType atribuicao_vetor(string text_original, int l, int c);
ResultType atribuicao_valores(string text_original, int l, int c);
ResultType atribuicao_valores_branch(string text_original, int l, int c);
ResultType booleano(string text_original, int l, int c);
ResultType expressao(string text_original, int l, int c);
ResultType expressao_termo_logico(string text_original, int l, int c);
ResultType termo_logico(string text_original, int l, int c);
ResultType fim_termo_logico(string text_original, int l, int c);
ResultType termo_logico_texto(string text_original, int l, int c);
ResultType termo_logico_mat(string text_original, int l, int c);
ResultType texto(string text_original, int l, int c);
ResultType chama_vetor(string text_original, int l, int c);
ResultType barra_texto(string text_original, int l, int c);
ResultType matematica(string text_original, int l, int c);
ResultType fim_mat(string text_original, int l, int c);
ResultType prioridade_baixa(string text_original, int l, int c);
ResultType prioridade_iden(string text_original, int l, int c);
ResultType prioridade_baixa_alta(string text_original, int l, int c);
ResultType lista_identificadores(string text_original, int l, int c);
ResultType lista_identificadores_mult(string text_original, int l, int c);
ResultType identificador_expressao(string text_original, int l, int c);
ResultType identificador_expressao_vetor(string text_original, int l, int c);
ResultType identificador(string text_original, int l, int c);
ResultType barra_id(string text_original, int l, int c);
ResultType numero_branch(string text_original, int l, int c);
ResultType lista_digitos(string text_original, int l, int c);
ResultType barra_lista_digitos(string text_original, int l, int c);
ResultType argumentos(string text_original, int l, int c);
ResultType args_texto(string text_original, int l, int c);
ResultType mid_args(string text_original, int l, int c);
ResultType fim_args(string text_original, int l, int c);
ResultType matematica_prioridade_baixa(string text_original, int l, int c);

ResultType programa(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected Declare{"); 
    if(text.length() > 8 && text.substr(0, 8) == "Declare{"){
        result = lista_declaracoes(text.substr(8), line, column + 8);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected };"); 
    if(text.length() > 2 && text.substr(0, 2) == "};"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected Functions{"); 
    if(text.length() > 10 && text.substr(0, 10) == "Functions{"){
        result = lista_funcoes(text.substr(10), line, column + 10);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected };"); 
    if(text.length() > 2 && text.substr(0, 2) == "};"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected Main{"); 
    if(text.length() > 5 && text.substr(0, 5) == "Main{"){
        result = lista_instrucoes(text.substr(5), line, column + 5);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected };"); 
    if(text.length() >= 2 && text.substr(0, 2) == "};"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType lista_instrucoes(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected return"); 
    if(text.length() > 6 && text.substr(0, 6) == "return"){
          result.setText(text.substr(6));
          result.setColumn(6+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador_expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_instrucoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected if("); 
    if(text.length() > 3 && text.substr(0, 3) == "if("){
        result = expressao(text.substr(3), line, column + 3);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = se_senao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_instrucoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected while("); 
    if(text.length() > 6 && text.substr(0, 6) == "while("){
        result = expressao(text.substr(6), line, column + 6);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_instrucoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected for("); 
    if(text.length() > 4 && text.substr(0, 4) == "for("){
        result = atribuicao(text.substr(4), line, column + 4);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_instrucoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = instrucao_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_instrucoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType lista_funcoes(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected Float"); 
    if(text.length() > 5 && text.substr(0, 5) == "Float"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Int"); 
    if(text.length() > 3 && text.substr(0, 3) == "Int"){
          result.setText(text.substr(3));
          result.setColumn(3+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected String"); 
    if(text.length() > 6 && text.substr(0, 6) == "String"){
          result.setText(text.substr(6));
          result.setColumn(6+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Char"); 
    if(text.length() > 4 && text.substr(0, 4) == "Char"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Boolean"); 
    if(text.length() > 7 && text.substr(0, 7) == "Boolean"){
          result.setText(text.substr(7));
          result.setColumn(7+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected List<"); 
    if(text.length() > 5 && text.substr(0, 5) == "List<"){
        result = tipo(text.substr(5), line, column + 5);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = parametros(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = bloco(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
        result = lista_funcoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType lista_declaracoes(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected Float"); 
    if(text.length() > 5 && text.substr(0, 5) == "Float"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Int"); 
    if(text.length() > 3 && text.substr(0, 3) == "Int"){
          result.setText(text.substr(3));
          result.setColumn(3+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected String"); 
    if(text.length() > 6 && text.substr(0, 6) == "String"){
          result.setText(text.substr(6));
          result.setColumn(6+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Char"); 
    if(text.length() > 4 && text.substr(0, 4) == "Char"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected Boolean"); 
    if(text.length() > 7 && text.substr(0, 7) == "Boolean"){
          result.setText(text.substr(7));
          result.setColumn(7+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected List<"); 
    if(text.length() > 5 && text.substr(0, 5) == "List<"){
        result = tipo(text.substr(5), line, column + 5);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() > 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_declaracoes(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType bloco(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected {"); 
    if(text.length() > 1 && text.substr(0, 1) == "{"){
        result = lista_instrucoes(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected }"); 
    if(text.length() >= 1 && text.substr(0, 1) == "}"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType instrucao_iden(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = argumentos(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() >= 1 && text.substr(0, 1) == ")"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ["); 
    if(text.length() > 1 && text.substr(0, 1) == "["){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ]"); 
    if(text.length() > 1 && text.substr(0, 1) == "]"){
        result = chama_vetor(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao_valores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ="); 
    if(text.length() > 1 && text.substr(0, 1) == "="){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao_valores_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
return result; 
}
}
ResultType se_senao(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected else"); 
    if(text.length() > 4 && text.substr(0, 4) == "else"){
        result = bloco(text.substr(4), line, column + 4);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType tipo(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected Float"); 
    if(text.length() >= 5 && text.substr(0, 5) == "Float"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected Int"); 
    if(text.length() >= 3 && text.substr(0, 3) == "Int"){
          result.setText(text.substr(3));
          result.setColumn(3+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected String"); 
    if(text.length() >= 6 && text.substr(0, 6) == "String"){
          result.setText(text.substr(6));
          result.setColumn(6+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected Char"); 
    if(text.length() >= 4 && text.substr(0, 4) == "Char"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected Boolean"); 
    if(text.length() >= 7 && text.substr(0, 7) == "Boolean"){
          result.setText(text.substr(7));
          result.setColumn(7+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected List<"); 
    if(text.length() > 5 && text.substr(0, 5) == "List<"){
        result = tipo(text.substr(5), line, column + 5);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected >"); 
    if(text.length() >= 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType parametros(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected Float"); 
    if(text.length() > 5 && text.substr(0, 5) == "Float"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected Int"); 
    if(text.length() > 3 && text.substr(0, 3) == "Int"){
          result.setText(text.substr(3));
          result.setColumn(3+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected String"); 
    if(text.length() > 6 && text.substr(0, 6) == "String"){
          result.setText(text.substr(6));
          result.setColumn(6+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected Char"); 
    if(text.length() > 4 && text.substr(0, 4) == "Char"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected Boolean"); 
    if(text.length() > 7 && text.substr(0, 7) == "Boolean"){
          result.setText(text.substr(7));
          result.setColumn(7+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected List<"); 
    if(text.length() > 5 && text.substr(0, 5) == "List<"){
        result = tipo(text.substr(5), line, column + 5);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType lista_parametros(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ,"); 
    if(text.length() > 1 && text.substr(0, 1) == ","){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = parametros(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType atribuicao(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao_vetor(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao_valores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ;"); 
    if(text.length() >= 1 && text.substr(0, 1) == ";"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType atribuicao_vetor(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ["); 
    if(text.length() > 1 && text.substr(0, 1) == "["){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ]"); 
    if(text.length() > 1 && text.substr(0, 1) == "]"){
        result = chama_vetor(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType atribuicao_valores(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ="); 
    if(text.length() > 1 && text.substr(0, 1) == "="){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = atribuicao_valores_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType atribuicao_valores_branch(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = fim_mat(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
        result = texto(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() >= 1 && text.substr(0, 1) == "\""){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected True"); 
    if(text.length() >= 4 && text.substr(0, 4) == "True"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected False"); 
    if(text.length() >= 5 && text.substr(0, 5) == "False"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
          return result;
return result; 
}
}
ResultType booleano(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected True"); 
    if(text.length() >= 4 && text.substr(0, 4) == "True"){
          result.setText(text.substr(4));
          result.setColumn(4+result.getColumn());
          return result;
return result; 
}
  result.setError("Expected False"); 
    if(text.length() >= 5 && text.substr(0, 5) == "False"){
          result.setText(text.substr(5));
          result.setColumn(5+result.getColumn());
          return result;
return result; 
}
}
ResultType expressao(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
        result = texto(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_texto(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = mid_args(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType expressao_termo_logico(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected &&"); 
    if(text.length() > 2 && text.substr(0, 2) == "&&"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected ||"); 
    if(text.length() > 2 && text.substr(0, 2) == "||"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType termo_logico(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
        result = texto(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_texto(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = fim_termo_logico(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType fim_termo_logico(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ∗"); 
    if(text.length() > 1 && text.substr(0, 1) == "∗"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected /"); 
    if(text.length() > 1 && text.substr(0, 1) == "/"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected %"); 
    if(text.length() > 1 && text.substr(0, 1) == "%"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected +"); 
    if(text.length() > 1 && text.substr(0, 1) == "+"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected -"); 
    if(text.length() > 1 && text.substr(0, 1) == "-"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected =="); 
    if(text.length() > 2 && text.substr(0, 2) == "=="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected !="); 
    if(text.length() > 2 && text.substr(0, 2) == "!="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >="); 
    if(text.length() > 2 && text.substr(0, 2) == ">="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <="); 
    if(text.length() > 2 && text.substr(0, 2) == "<="){
        result =  termo_logico(text.substr(2), line, column + 2);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <"); 
    if(text.length() > 1 && text.substr(0, 1) == "<"){
        result =  termo_logico(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType termo_logico_texto(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected =="); 
    if(text.length() > 2 && text.substr(0, 2) == "=="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType termo_logico_mat(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected =="); 
    if(text.length() > 2 && text.substr(0, 2) == "=="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected !="); 
    if(text.length() > 2 && text.substr(0, 2) == "!="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >="); 
    if(text.length() > 2 && text.substr(0, 2) == ">="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <="); 
    if(text.length() > 2 && text.substr(0, 2) == "<="){
        result =  termo_logico(text.substr(2), line, column + 2);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <"); 
    if(text.length() > 1 && text.substr(0, 1) == "<"){
        result =  termo_logico(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType texto(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Invalid character"); 
    if(text.length() > 1 && istext(text[0])){
        result = barra_texto(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType chama_vetor(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ["); 
    if(text.length() > 1 && text.substr(0, 1) == "["){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ]"); 
    if(text.length() > 1 && text.substr(0, 1) == "]"){
        result = chama_vetor(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType barra_texto(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Invalid character"); 
    if(text.length() > 1 && istext(text[0])){
        result = barra_texto(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType matematica(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = fim_mat(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType fim_mat(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ∗"); 
    if(text.length() > 1 && text.substr(0, 1) == "∗"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected /"); 
    if(text.length() > 1 && text.substr(0, 1) == "/"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected %"); 
    if(text.length() > 1 && text.substr(0, 1) == "%"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected +"); 
    if(text.length() > 1 && text.substr(0, 1) == "+"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected -"); 
    if(text.length() > 1 && text.substr(0, 1) == "-"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType prioridade_baixa(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = prioridade_baixa_alta(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa_alta(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa_alta(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa_alta(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType prioridade_iden(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ["); 
    if(text.length() > 1 && text.substr(0, 1) == "["){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ]"); 
    if(text.length() > 1 && text.substr(0, 1) == "]"){
        result = chama_vetor(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = argumentos(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() >= 1 && text.substr(0, 1) == ")"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
          return result;
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType prioridade_baixa_alta(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ∗"); 
    if(text.length() > 1 && text.substr(0, 1) == "∗"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected /"); 
    if(text.length() > 1 && text.substr(0, 1) == "/"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected %"); 
    if(text.length() > 1 && text.substr(0, 1) == "%"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType lista_identificadores(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores_mult(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType lista_identificadores_mult(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ,"); 
    if(text.length() > 1 && text.substr(0, 1) == ","){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = lista_identificadores(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType identificador_expressao(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = identificador_expressao_vetor(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType identificador_expressao_vetor(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ["); 
    if(text.length() > 1 && text.substr(0, 1) == "["){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected ]"); 
    if(text.length() > 1 && text.substr(0, 1) == "]"){
        result = chama_vetor(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
}
ResultType identificador(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType barra_id(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType numero_branch(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ."); 
    if(text.length() > 1 && text.substr(0, 1) == "."){
        result = lista_digitos(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType lista_digitos(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType barra_lista_digitos(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType argumentos(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
        result = texto(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected \""); 
    if(text.length() > 1 && text.substr(0, 1) == "\""){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = args_texto(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected ("); 
    if(text.length() > 1 && text.substr(0, 1) == "("){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
  result.setError("Expected )"); 
    if(text.length() > 1 && text.substr(0, 1) == ")"){
        result = mid_args(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
}else{
result.setAccept(false);
}
return result; 
}
  result.setError("Expected !"); 
    if(text.length() > 1 && text.substr(0, 1) == "!"){
        result = expressao(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected letter"); 
    if(text.length() > 1 && isalpha(text[0])){
        result = barra_id(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_iden(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected number"); 
    if(text.length() > 1 && isdigit(text[0])){
        result = barra_lista_digitos(text.substr(1), line, column);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = numero_branch(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = mid_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = fim_args(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType args_texto(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected =="); 
    if(text.length() > 2 && text.substr(0, 2) == "=="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected &&"); 
    if(text.length() > 2 && text.substr(0, 2) == "&&"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected ||"); 
    if(text.length() > 2 && text.substr(0, 2) == "||"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType mid_args(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ∗"); 
    if(text.length() > 1 && text.substr(0, 1) == "∗"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected /"); 
    if(text.length() > 1 && text.substr(0, 1) == "/"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected %"); 
    if(text.length() > 1 && text.substr(0, 1) == "%"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica_prioridade_baixa(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected +"); 
    if(text.length() > 1 && text.substr(0, 1) == "+"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected -"); 
    if(text.length() > 1 && text.substr(0, 1) == "-"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico_mat(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected =="); 
    if(text.length() > 2 && text.substr(0, 2) == "=="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected !="); 
    if(text.length() > 2 && text.substr(0, 2) == "!="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >="); 
    if(text.length() > 2 && text.substr(0, 2) == ">="){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <="); 
    if(text.length() > 2 && text.substr(0, 2) == "<="){
        result =  termo_logico(text.substr(2), line, column + 2);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected >"); 
    if(text.length() > 1 && text.substr(0, 1) == ">"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected <"); 
    if(text.length() > 1 && text.substr(0, 1) == "<"){
        result =  termo_logico(text.substr(1), line, column + 1);
        if(!result.getAccept()){
            return result;
        }
        trimmed = trim(result.getText(), result.getLine(), result.getColumn());
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao_termo_logico(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected &&"); 
    if(text.length() > 2 && text.substr(0, 2) == "&&"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
          result.setText(text.substr(0));
          result.setColumn(0+result.getColumn());
  result.setError("Expected ||"); 
    if(text.length() > 2 && text.substr(0, 2) == "||"){
          result.setText(text.substr(2));
          result.setColumn(2+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = expressao(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType fim_args(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected ,"); 
    if(text.length() > 1 && text.substr(0, 1) == ","){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = argumentos(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}
ResultType matematica_prioridade_baixa(string text_original, int l, int c){
    ResultType trimmed = trim(text_original, l, c);
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();
  result.setError("Expected +"); 
    if(text.length() > 1 && text.substr(0, 1) == "+"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
  result.setError("Expected -"); 
    if(text.length() > 1 && text.substr(0, 1) == "-"){
          result.setText(text.substr(1));
          result.setColumn(1+result.getColumn());
trimmed = trim(result.getText(), result.getLine(), result.getColumn());
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();
        result = matematica(text.substr(0), line, column + 0);
        if(!result.getAccept()){
            return result;
        }
return result; 
}
}

ResultType recognizes(string text){
    text = rtrim_copy(text);
    ResultType result = programa(text, 0, 1);
    if(result.getText().empty() && result.getAccept()){
        cout << "Foi Reconhecido" << endl;
    }else{
        cout << result.getError() << endl;
        cout << "Existe um erro na linha " << result.getLine() << " e coluna " << result.getColumn() << endl; 
    }
    return result;  
}

int main(int argc, char const *argv[]){
    /* Implement recognize */

    string file = "programa";
    //cin >> file;

    ifstream t(file + ".txt");
    string text;

    t.seekg(0, ios::end);   
    text.reserve(t.tellg());
    t.seekg(0, ios::beg);

    text.assign((istreambuf_iterator<char>(t)),
                istreambuf_iterator<char>());
    //Print program before recognition
    //cout << text <<  endl << endl;

    ResultType result = recognizes(text);

    return 0;
}