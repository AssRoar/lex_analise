//
// Created by Alex on 22.01.2020.
//

#ifndef LEX_ANALISE_TOKEN_H
#define LEX_ANALISE_TOKEN_H
#include <string>

enum token_type {
    word,
    res_word,
    constant,
    operation,
    punctuation,
    number,
    other,
    my_endl,
    my_eof,
    default_t,
    string_constant,
    char_constant,
    int_type,
    float_type,
    double_type,
    identificator,
    arithmetic_operator,
    logic_operator,
    comparsion_operator,
    assignment_operator
};

class token {
public:
    token_type type;
    bool isFunction = false;
    bool isSimpleVariable = false;
    std::string value;
    token(token_type t, std::string val): type(t), value(val) {};
    token(): type(default_t), value("") {};
};

std::string type_to_string (token_type t) {
    if (t == word) return "word";
    else if (t == res_word) return "res_word";
    else if (t == constant) return "constant";
    else if (t == operation) return "operation";
    else if (t == punctuation) return "punctuation";
    else if (t == number) return "number";
    else if (t == my_endl) return "endl";
    else if (t == my_eof) return "eof";
    else if (t == default_t) return "uninitialised token";
    else if (t == string_constant) return "string";
    else if (t == char_constant) return "char";
    else if (t == int_type) return "int";
    else if (t == float_type) return "float";
    else if (t == double_type) return "double";
    else if (t == identificator) return "name";
    else if (t == arithmetic_operator) return "arithmetic_operator";
    else if (t == logic_operator) return "logic_operator";
    else if (t == comparsion_operator) return "comparsion_operator";
    else if (t == assignment_operator) return "assignment_operator";
    else return "another type (look at token.h)";
}

#endif //LEX_ANALISE_TOKEN_H
