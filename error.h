//
// Created by ALEXA on 04.02.2020.
//

#ifndef LEX_ANALISE_ERROR_H
#define LEX_ANALISE_ERROR_H

#include "token.h"
#include "TID.h"
#include <iostream>
using namespace std;

enum error_type {
    none,
    double_format_error,
    float_format_error,
    char_format_error,
    do_while_format_error,
    broken_bracket_balance,
    call_error,
    index_error,
    while_format_error,
    for_format_error,
    semicolon_error,
    get_format_error,
    out_format_error,
    if_format_error,
    case_format_error,
    list_format_error,
    expression_format_error,
    unexpected_token_error,
    syntax_error,
    header_format_error,
    unexpected_end_of_file,
    header_list_format_error,
    redeclaration,
    undeclared_var,
    wrong_expression_type,
    incompatible_types
};

class error {
public :
    error_type type;
    int line;
    token word;
    error(error_type t, int lin, token tok): type(t), line(lin), word(tok) {};
    error() {
        type = none;
        line = -1;
        token a;
        word = a;
    }
    void fill (token t) {
        word = t;
    }
    void fill (error_type t) {
        type = t;
    }
    void fill (int i) {
        line = i;
    }
    void fill () {
        type = none;
        line = -1;
        token a;
        word = a;
    }
    void fill (error_type t, int lin, token tok) {
        type = t;
        line = lin;
        word = tok;
    }
};

struct bool_error {
    error error;
    bool val;
};

string error_to_string (error a) {
    switch (a.type) {
        case double_format_error:
            return "double_format_error";
        case float_format_error:
            return "float_format_error";
        case char_format_error:
            return "char_format_error";
        case do_while_format_error:
            return "do_while_format_error";
        case broken_bracket_balance:
            return "broken_bracket_balance";
        case call_error:
            return "call_error";
        case index_error:
            return "index_error";
        case while_format_error:
            return "while_format_error";
        case for_format_error:
            return "for_format_error";
        case semicolon_error:
            return "semicolon_error";
        case get_format_error:
            return "get_format_error";
        case out_format_error:
            return "out_format_error";
        case if_format_error:
            return "if_format_error";
        case case_format_error:
            return "case_format_error";
        case list_format_error:
            return "list_format_error";
        case expression_format_error:
            return "expression_format_error";
        case unexpected_token_error:
            return "unexpected_token_error";
        case syntax_error:
            return "syntax_error";
        case none:
            return "none";
        case header_format_error:
            return "header_format_error";
        case unexpected_end_of_file:
            return "unexpected_end_of_file";
        case header_list_format_error:
            return "header_list_format_error";
        case redeclaration :
            return "redeclaration_in_identificator";
        case undeclared_var :
            return "usage_of_redeclared_identificator";
        case wrong_expression_type :
            return "wrong_type_of_expression";
        case incompatible_types:
            return "incompatible_types";
        default: return "smth_whent_wrong";
    }

}

#endif //LEX_ANALISE_ERROR_H
