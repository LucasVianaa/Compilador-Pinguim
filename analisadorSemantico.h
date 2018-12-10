#ifndef SEMANTICO_H
#define SEMANTICO_H
#include "ResultType.h"
class Variable;
void print_errors(ResultType);
void build_scopes(string);
void find_calls(string);
void find_operations(string);

ResultType verify_assignments();
ResultType verify_duplicates();
ResultType verify_scopes();
#endif
