#ifndef LEX_ANALISE_SYNTAX_H
#define LEX_ANALISE_SYNTAX_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include "token.h"
#include "lex.h"
#include <fstream>
#include "variables.h"
#include "error.h"
#include "TID.h"
#include <stack>


bool_error is_block (name_type s);
bool_error is_pseudo_variable ();
bool_error is_op(name_type s);
bool_error is_variable_declaration();
int is_an_array ();
token qur;
int next_index_in_main_vector;
int qur_line;
bool new_level_created;

bool get_qur();

bool is_bin(token a) {
    return unary_operators.find(a.value) == unary_operators.end();
}

bool is_ident(){
    return letters.find(qur.value[0]) != letters.end();
}
void words_to_ident () {
    next_index_in_main_vector = 0;
    qur_line = 1;
    while (get_qur()) {
        if (qur.type == word and is_ident()) main_vector[next_index_in_main_vector -1].type = identificator;
    }
}

void operators_to_typed () {
    next_index_in_main_vector = 0;
    qur_line = 1;
    while (get_qur()) {
        if (qur.type == operation) {
            if (arithmetic_operators.find(qur.value) != arithmetic_operators.end())
                main_vector[next_index_in_main_vector - 1].type = arithmetic_operator;
            if (logic_operators.find(qur.value) != logic_operators.end())
                main_vector[next_index_in_main_vector - 1].type = logic_operator;
            if (comparsion_operators.find(qur.value) != comparsion_operators.end())
                main_vector[next_index_in_main_vector - 1].type = comparsion_operator;
            if (assignment_operators.find(qur.value) != assignment_operators.end())
                main_vector[next_index_in_main_vector - 1].type = assignment_operator;
        }
    }
}

void res_words_to_operations () {
    next_index_in_main_vector = 0;
    qur_line = 1;
    while (get_qur()) {
        if (qur.type == res_word and operators_many.find(qur.value) != operators_many.end())
            main_vector[next_index_in_main_vector -1].type = operation;
    }
}

void from_save_qur (int save_index, token save_qur, int save_line) {
    next_index_in_main_vector = save_index;
    qur_line = save_line;
    qur = save_qur;
}

std::map<std::string, int> priority;
std::stack<token> signs;
std::vector<token> ans;

bool afterOpeningBracket = false;
int afterOpBracket = 0;
bool mustBeObject = false;
bool mustBeVar = false;
bool afterVariable = false;

int inFunction = 0;

struct bool_error_expr {
    bool_error bool_error;
    name_type type;
    int fields_amount = 0;
};

struct variable {
    name_type type;
    int fields_amount;
};

pair <bool_error, variable> idendef_analise () {
    pair <bool_error, variable> ans;
    token save_qur = qur;
    if (qur.type != identificator) {
        ans.first.error.fill(unexpected_token_error, qur_line, qur);
        ans.first.val = false;
    }
    name_type nt = is_declared(qur.value);
    if (nt == default_type) {
        ans.first.error.fill(undeclared_var, qur_line, qur);
        ans.first.val = false;
    }
    get_qur();
    int amount = 0;
    if (is_mas(nt))
        if (qur.value == "[") { //is_massive
            int amount = is_an_array();
            if (amount == 0) {
                ans.first.error.fill(unexpected_token_error, qur_line, qur);
                ans.first.val = false;
                return ans;
            }
            else {
                ans.first.val = true;
                ans.second.type = nt_to_mas(nt);
                ans.second.fields_amount = amount;
                return ans;
            }
        }
    //is_variable
    ans.first.val = true;
    ans.second.type = mas_to_nt(nt);
    ans.second.fields_amount = 0;
    return ans;
}

struct tfp{
    token_type type;
    error error;
    int int_value=0;
    double double_value=0;
    string string_value=0;
    char char_value=0;
    float float_value=0;
    bool bool_value=0;
};

tfp str_to_num(token t){
    tfp ans;
    if (t.type == double_type || t.type==float_type){
        for (auto i:t.value) if (i=='e' || i=='E') {ans.type == float_type; ans.float_value=stof(t.value); return ans;}
        ans.type=double_type;
        ans.double_value=stof(t.value);
        return ans;
    }
    else if (t.type == int_type){
        for (auto i:t.value) {
            ans.int_value=ans.int_value*10+(i-'0');
        }
        return ans;
    }
    else if (t.type == char_constant){
        ans.char_value=t.value[0];
        return ans;
    }
    else if (t.type == string_constant){
        ans.string_value=t.value;
        return ans;
    }
//    else if (t.type== bool) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!BOOL
}

bool_error_expr polys(std::vector<token> ans){
    stack<token> result;
    bool_error_expr error;
    int k=0, FL=0;
    for (auto i:ans){
        if (i.type == double_type ||
            i.type == float_type ||
            i.type == int_type ||
            i.type == string_constant ||
            i.type == char_constant ||
            i.type == constant) {
            result.push(i);
            k++;
        }
    }
    for (int i=0; i<k;i++){
        auto bp=result.top();
        result.pop();
        if (bp.type == double_type){
            if (FL<3) FL=3;
        }
        else if (bp.type == float_type) if (FL<4) FL=4;
        else if ((bp.type == string_constant || bp.type == char_constant) && FL!=0 && FL<5)  {error.bool_error.error.type=expression_format_error; error.bool_error.val=false; return error;}//ОШИБКА
        else if (bp.type == int_type) if (FL<2) FL=2;
        else if (bp.type == constant) if (FL<1) FL=1;
        else if (bp.type == string_constant) if (FL==5 || FL==0) FL=6;
        else if (bp.type == char_constant) if (FL==6 || FL==0) FL=5;


        }
    switch (FL){
        case 1: {error.type=bool_t; return error;}
        case 2: {error.type=int_t; return error;}
        case 3: {error.type=double_t; return error;}
        case 4: {error.type=float_t; return error;}
        case 5: {error.type=char_t; return error;}
        case 6: {error.type=string_t; return error;}
    }
    }



//        if (i.type==arithmetic_operator){
//            if(i.value=="+"){
////                if(result.empty())
////                {
//////                    error.error.type=expression_format_error;
//////                    error.bool_error=false;
////                    // ОШИКБКА
////                    return error;
////                }
//                token a = result.top();
//                result.pop();
////                if(result.empty())
////                {
//////                    error.error.type=expression_format_error;
//////                    error.bool_error=false;
////                    // ОШИКБКА
////                    return error;
////                }
//                token b = result.top();
//                result.pop();
////                if (a.type){}
//            }
//        }


bool_error_expr is_expression(){
    int FL=0;
    priority["."] = 2;
    priority["++"] = priority["!"] = 3;
    priority["*"] = priority["/"] = priority["mod"] = priority["div"] = 5;
    priority["+"] = priority["-"] = 6;
    priority["<"] = priority[">"] =
    priority["<="] =priority[">="] = 8;
    priority["=="] = priority["!="] = 9;
    priority["and"] = 13;
    priority["or"] = 14;
    priority[""] = 4;
    priority["="] =priority["+="] =priority["-="]
            =priority["*="] = priority["/="] =priority["%="] =15;
    bool_error a;
    bool_error_expr error;
    a.val = false;
    a.error.fill();
    while ((qur.value != ")" || afterOpBracket > 0)
           && qur.value != ";" &&
           (qur.value != "," || afterOpBracket > 0)) {
        if (qur.type == int_type || qur.type == double_type ||
            qur.type == string_constant || qur.type == constant || qur.type == float_type || qur.type == char_constant) {
            ans.push_back(qur);

            if (qur.type == double_type) if (FL<3) FL=3;
            else if (qur.type == float_type) if (FL<4) FL=4;
            else if ((qur.type == string_constant || qur.type == char_constant) && FL!=0 && FL<5)  {error.bool_error.error.type=expression_format_error; error.bool_error.val=false; return error;}//ОШИБКА
            else if (qur.type == int_type) if (FL<2) FL=2;
            else if (qur.type == constant) if (FL<1) FL=1;
            else if (qur.type == string_constant) if (FL==5 || FL==0) FL=6;
            else if (qur.type == char_constant) if (FL==6 || FL==0) FL=5;

            get_qur();
            afterVariable = true;
            afterOpeningBracket = false;
        } else if (qur.type == identificator) {
            if (is_declared(qur.value) == default_type) {
                a.val = false; a.error.fill(undeclared_var, qur_line, qur);
                return error;
            }
            afterOpeningBracket = false;
            token q = qur;

            if (qur.type == double_type) if (FL<3) FL=3;
            else if (qur.type == float_type) if (FL<4) FL=4;
            else if ((qur.type == string_constant || qur.type == char_constant) && FL!=0 && FL<5)  {error.bool_error.error.type=expression_format_error; error.bool_error.val=false; return error;}//ОШИБКА
            else if (qur.type == int_type) if (FL<2) FL=2;
            else if (qur.type == constant) if (FL<1) FL=1;
            else if (qur.type == string_constant) if (FL==5 || FL==0) FL=6;
            else if (qur.type == char_constant) if (FL==6 || FL==0) FL=5;

            get_qur();
            if (qur.value == "(") {
                q.isFunction = true;
                ++inFunction;
                ++afterOpBracket;

                signs.push(q);
                signs.push(qur);
                get_qur();
            } else {
                afterVariable = true;
                q.isSimpleVariable = true;
                ans.push_back(q);
            }
        } else if (qur.value == ",") {
            if (!inFunction){
                a.error.fill();
                error.bool_error=a;
                return error;
            }
            while (!signs.empty() && (signs.top().type != punctuation && signs.top().value!= "(")) {
                ans.push_back(signs.top());
                signs.pop();
            }
            if (signs.empty()){
                a.error.fill();
                error.bool_error=a;
                return error;
            }
            get_qur();
        } else if ((qur.type == arithmetic_operator && (qur.value=="++" || qur.value=="--")) ||
                   (qur.type == logic_operator && qur.value == "!")) {
            if(afterVariable){
                a.error.fill();
                error.bool_error=a;
                return error;
            }
            signs.push(qur);
            get_qur();
        } else if (qur.type == arithmetic_operator ||
                   qur.type == comparsion_operator ||
                   qur.type == assignment_operator ||
                   qur.type == logic_operator) {
            afterVariable = false;
            if (qur.value == "=" || qur.value == "") {
                while(!signs.empty() && (signs.top().type != punctuation && signs.top().value!= "(") && ((signs.top().type == arithmetic_operator) ||
                                                                                                         (signs.top().type == logic_operator
                                                                                                          && signs.top().value == "!") ||
                                                                                                         priority[signs.top().value] < priority[qur.value])) {
                    ans.push_back(signs.top());
                    signs.pop();
                }
            } else {
                while(!signs.empty() && (signs.top().type != punctuation && signs.top().value!= "(") && ((signs.top().type == arithmetic_operator) ||
                                                                                                         (signs.top().type == logic_operator
                                                                                                          && signs.top().value == "!") ||
                priority[signs.top().value] <= priority[qur.value])) {
                    ans.push_back(signs.top());
                    signs.pop();
                }
            }

            signs.push(qur);
            get_qur();
        } else if (qur.type == punctuation && qur.value=="(") {
            ++afterOpBracket;
            afterOpeningBracket = true;
            signs.push(qur);
            get_qur();
        } else if (qur.type == punctuation && qur.value==")") {
            --afterOpBracket;
            if (afterOpeningBracket){
                a.error.fill(broken_bracket_balance);
                error.bool_error=a;
                return error;
            }
            while(!signs.empty() && (signs.top().type != punctuation && signs.top().value!= "(")) {
                ans.push_back(signs.top());
                signs.pop();
            }
            if (signs.empty()){
                a.error.fill();
                error.bool_error=a;
                return error;
            }
            else {
                signs.pop();
                if (!signs.empty() && signs.top().type == identificator) {
                    ans.push_back(signs.top());
                    signs.pop();
                    --inFunction;
                }
            }
            get_qur();
        } else{
            a.error.fill();
            error.bool_error=a;
            return error;
        }

    }
    while (!signs.empty()) {
        if (signs.top().type == punctuation && signs.top().value=="("){
            a.error.fill();
            error.bool_error=a;
            return error;
        }

        ans.push_back(signs.top());
        signs.pop();
    }
    a.val=true;
    error.bool_error=a;
    error.type=bool_t;
    return error;
}

bool_error is_for_nl(){
    bool_error a;
    a.val=false;
    a.error.fill();
    if(qur.value==")"){
        a.val=true;
        return a;
    }
    if (qur.value!=",") {
        a.error.fill(list_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.type!=identificator){
        a.error.fill(list_format_error, qur_line, qur);
        return a;
    }
    else if (qur.type==identificator){
        if (is_declared(qur.value) == default_type) {
            a.error.fill(undeclared_var, qur_line, qur);
            return a;
        }
        if (!get_qur()){
            a.error.fill(list_format_error, qur_line, qur);
            return a;
        }
        return is_for_nl();
    }
}
bool_error is_names_list(){
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.type!=identificator){
        a.error.fill(list_format_error, qur_line, qur);
        return a;
    }
    if (is_declared(qur.value) == default_type) {
        a.error.fill(undeclared_var, qur_line, qur);
        return a;
    }
    get_qur();
    if (!is_for_nl().val){
        a.error.fill(list_format_error, qur_line, qur);
        return a;
    }
    a.val=true;
    return a;
}
bool_error is_list(){
    bool_error a;
    a.val=false;
    a.error.fill();
    bool_error_expr b_e_x = is_expression();
    a = b_e_x.bool_error;
    if (a.error.type != none) return a;
    if (qur.value!=","){
        a.val=true;
        return a;
    }
    else {get_qur(); return is_list();}
}
bool_error is_header_list () {
    bool_error a;
    a.error.fill();
    a.val = false;
    while (types.find(qur.value) != types.end()) {
        name_type name_t = str_to_name_t(qur.value);
        get_qur();
        if (qur.type != identificator) {
            a.error.fill(header_list_format_error, qur_line, qur);
            return a;
        }
        a = push_tid(qur.value, name_t, qur_line);
        if (!a.val) return a;
        get_qur();
        if (qur.value == ")") break;
        else if (qur.value != ",") {
            a.error.fill(header_list_format_error, qur_line, qur);
            return a;
        }
        get_qur();
    }
    if (qur.value != ")" ) {
        a.error.fill(header_list_format_error, qur_line, qur);
        return a;
    }
    a.val = true;
    return a;
}

bool_error is_op_do_while(name_type s) {
    bool_error a;
    a.val = false;
    a.error.fill();
    if (qur.type != res_word or qur.value != "do"){ //do
        return a;
    }
    get_qur();
    if (qur.value == "{") { //do {}
        a = is_block(s);
        if (a.error.type != none) return a;
        else if (!a.val) {
            a.error.fill(do_while_format_error, qur_line, qur);
            return a;
        }
    }
    else {               //do expr;
        add_level();
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        a = is_op(s);
        if (!a.val) {
            from_save_qur(s_i, s_q, s_l);
            bool_error_expr b_e_x = is_expression();
            a = b_e_x.bool_error;
            if (a.error.type != none) return a;
        }
        if (qur.value != ";") {
            a.error.fill(semicolon_error, qur_line, qur);
            return a;
        }
        remove_level();
    }
    // do smth while
    get_qur();
    if (qur.type != res_word and qur.value != "while") {
        a.error.fill(do_while_format_error, qur_line, qur);
        return a;
    }
    // while (
    get_qur();
    if (qur.value!="(") {
        a.error.fill(do_while_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    // ...while ( expr
    bool_error_expr b_e_x = is_expression();
    a = b_e_x.bool_error;
    if (a.error.type != none) return a;
    if (b_e_x.type != bool_t) {
        a.error.fill(wrong_expression_type, qur_line, qur);
        return a;
    }
    if (qur.value != ")") { //...while ( expr )
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    a.val = true;
    get_qur();
    return a;
}
bool_error is_op_while(name_type s){
    bool_error a;
    a.val = false;
    a.error.fill();
    if (qur.type != res_word or qur.value != "while") {
        return a;
    }
    // while (
    get_qur();
    if (qur.value!="(") {
        a.error.fill(while_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    // while ( expr
    bool_error_expr b_e_x = is_expression();
    a = b_e_x.bool_error;
    if (a.error.type != none) return a;
    if (b_e_x.type != bool_t) {
        a.error.fill(wrong_expression_type, qur_line, qur);
        return a;
    }
    if (qur.value != ")") { //while ( expr )
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    //while ( expr ) {} or expr
    get_qur();
    if (qur.value == "{") { //while ( expr ) {}
        a = is_block(s);
        if (a.error.type != none) return a;
        else if (!a.val) {
            a.error.fill(while_format_error, qur_line, qur);
            return a;
        }
        a.val = true;
        return a;
    }
    else {               //while ( expr ) expr;
        add_level();
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        a = is_op(s);
        if (!a.val) {
            from_save_qur(s_i, s_q, s_l);
            bool_error_expr b_e_x = is_expression();
            a = b_e_x.bool_error;
            if (a.error.type != none) return a;
        }
        if (qur.value != ";") {
            a.error.fill(semicolon_error, qur_line, qur);
            return a;
        }
        remove_level();
    }
    a.val = true;
    get_qur();
    return a;
}
bool_error is_op_for(name_type s) {
    add_level();
    new_level_created = true;
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.type != res_word or qur.value != "for") return a; //for
    get_qur();
    if (qur.value != "("){ //for (
        a.error.fill(for_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    int bb = 0;
    if (qur.type == res_word and types.find(qur.value) != types.end()) {
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        bb = 1;
        a = is_variable_declaration();
        if (a.val) {
            if (qur.value != ";") {
                a.error.fill(semicolon_error, qur_line, qur);
                return a;
            }
            get_qur();
        }
        else {
            bb = 0;
            from_save_qur(s_i, s_q, s_l);
        }
    }
    for (int i = bb; i < 3; ++i) { //for (e;e;e
        bool_error_expr b_e_x = is_expression();
        a = b_e_x.bool_error;
        if (a.error.type != none) return a;
        if (i != 2 and qur.value != ";") {
            a.error.fill(semicolon_error, qur_line, qur);
            return a;
        }
        else get_qur();
    }
    if (qur.value != ")") { //for (e;e;e)
        a.error.fill(for_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value == "{") { //for (e;e;e) {}
        a = is_block(s);
        if (a.error.type != none) return a;
        else if (!a.val) {
            a.error.fill(for_format_error, qur_line, qur);
            return a;
        }
    }
    else {               //for (e;e;e) expr;
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        a = is_op(s);
        if (!a.val) {
            from_save_qur(s_i, s_q, s_l);
            bool_error_expr b_e_x = is_expression();
            a = b_e_x.bool_error;
            if (a.error.type != none) return a;
        }
        if (qur.value != ";") {
            a.error.fill(semicolon_error, qur_line, qur);
            return a;
        }
    }
    remove_level();
    a.val = true;
    get_qur();
    return a;
}

bool_error is_op_get() {
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.type!=res_word or qur.value!="get") return a; //get
    get_qur();
    if (qur.value!="("){        //get (
        a.error.fill(get_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    a = is_names_list();
    if (a.error.type != none) return a;
    else if (!a.val) {
        a.error.fill(get_format_error, qur_line, qur);
        return a;
    }
    if (qur.value != ")"){ //get (list)
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    a.val = true;
    get_qur();
    return a;
}
bool_error is_op_out(){
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.type!=res_word or qur.value!="out") return a; //out
    get_qur();
    if (qur.value!="("){ //out (
        a.error.fill(out_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    a = is_list();
    if (a.error.type != none) return a;
    else if (!a.val){
        a.error.fill(out_format_error, qur_line, qur);
        return a;
    }
    if (qur.value!=")"){ //out (smth)
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    a.val = true;
    get_qur();
    return a;
}

bool_error is_op_if(name_type s) {
    bool_error a;
    a.val=false;
    a.error.fill();
    if(qur.value != "if") return a;
    get_qur();
    if(qur.value!="("){ //if (
        a.error.fill(if_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    //if (exp
    bool_error_expr b_e_x = is_expression();
    a = b_e_x.bool_error;
    if (b_e_x.type != bool_t) {
        a.error.fill(wrong_expression_type, qur_line, qur);
        return a;
    }
    if (a.error.type != none) return a;
    if(qur.value!=")") { //if (exp)
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    get_qur();
    if(qur.value != "then"){ //if (exp) then
        a.error.fill(if_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value == "{") { //if (exp) then {}
        a = is_block(s);
        if (a.error.type != none) return a;
    }
    else {               //if (exp) then expr;
        add_level();
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        a = is_op(s);
        if (!a.val) {
            from_save_qur(s_i, s_q, s_l);
            bool_error_expr b_e_x = is_expression();
            a = b_e_x.bool_error;
            if (a.error.type != none) return a;
        }
        if (qur.value != ";") {
            a.error.fill(semicolon_error, qur_line, qur);
            return a;
        }
        remove_level();
    }
    a.val=true;
    get_qur();
    return a;
}
bool_error is_pseudo_variable () { // only indef[a][3]
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.type != identificator) return a;
    get_qur();
    while (qur.value == "[") {
        get_qur();
        if (qur.type != identificator and qur.type != int_type) {
            a.error.fill(index_error, qur_line, qur);
            return a;
        }
        get_qur();
        if (qur.value != "]") {
            a.error.fill(broken_bracket_balance, qur_line, qur);
            return a;
        }
        get_qur();
    }

    a.val = true;
    return a;
}
bool_error_expr is_range () {
    bool_error_expr a;
    a.bool_error.val=false;
    a.bool_error.error.fill();
    bool_error_expr b_e_x = is_expression();
    a = b_e_x;
    if (a.bool_error.error.type != none) {
        a.bool_error.val=false; return a;
    }
    for (int i = 0; i < 3; ++i) {
        if (qur.value != ".") {
            a.bool_error.val=false; return a;
        }
        get_qur();
    }
    a = is_expression();
    if (a.bool_error.error.type != none) {
        a.bool_error.val=false; return a;
    }
    if (a.type != b_e_x.type) {
        a.bool_error.error.fill(incompatible_types, qur_line, qur);
        return a;
    }
    a.bool_error.val=true;
    return a;
}
bool_error is_values(name_type n) {
    bool_error a;
    a.val=false;
    a.error.fill();
    bool_error_expr b_e_x;
    int s_i = next_index_in_main_vector;
    auto s_q = qur;
    int s_l = qur_line;
    b_e_x = is_range();
    a = b_e_x.bool_error;
    if (!a.val and a.error.type != none) return a;
    else if (!a.val) {
        from_save_qur(s_i, s_q, s_l);
        bool_error_expr b_e_x = is_expression();
        a = b_e_x.bool_error;
        if (!a.val) {
            return a;
        }
    }
    if (b_e_x.type != n) {
        a.error.fill(incompatible_types, qur_line, qur);
        return a;
    }
    if (qur.value!=","){
        a.val=true;
        return a;
    }
    else {
        get_qur();
        return is_values(n);
    }
}
bool_error is_op_case(name_type s) {
    bool_error a;
    a.val=false;
    a.error.fill();
    if (qur.value!="case") return a;
    get_qur();
    if (qur.type != identificator) {
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    name_type nt = is_declared(qur.value);
    if (nt == default_type) {
        a.error.fill(undeclared_var, qur_line, qur);
        return a;
    }
    a = is_pseudo_variable(); //case var
    if (a.error.type != none) return a;
    else if (!a.val)  {
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    if (qur.value!="of"){ //case var of
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if(qur.value != ":"){//case var of:
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    a = is_values(nt);
    if (!a.val) return a;
    while (a.val) {
        if (qur.value != ":") {
            a.error.fill(case_format_error, qur_line, qur);
            return a;
        }
        get_qur();
        a = is_block(s);
        if (a.error.type != none) return a;
        if (!a.val) {
            a.error.fill(case_format_error, qur_line, qur);
        }
        if (qur.value =="else") break;
        a = is_values(nt);
    }
    if (qur.value!="else"){
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != ":") {
        a.error.fill(case_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    a = is_block(s);
    if (a.error.type != none) return a;
    if (!a.val) {
        a.error.fill(case_format_error, qur_line, qur);
    }
    if (!a.val) return a;
    a.val = true;
    return a;
}

bool_error is_op_break() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (qur.value != "break") return a;
    get_qur();
    a.val = true;
    return a;
}
bool_error is_op_continue() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (qur.value != "continue") return a;
    a.val = true;
    get_qur();
    return a;
}
bool_error is_op_return(name_type s) {
    bool_error a;
    a.error.fill();
    a.val = false;
    if(qur.value!="return") return a;
    get_qur();
    if (qur.value == ";") {
        a.val=true;
        return a;
    }
    bool_error_expr b_e_x = is_expression();
    if (a.error.type != none) return a;
    a = b_e_x.bool_error;
    if (b_e_x.type != s) {
        a.error.fill(wrong_expression_type, qur_line, qur);
        return a;
    }
    a.val=true;
    return a;
}

bool_error is_function_call() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if(qur.type!=identificator) return a;
    get_qur();
    if (qur.value!="("){
        a.error.fill(call_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != ")") {
        a = is_names_list();
        if (a.error.type != none) return a;
        else if (!a.val) {
            a.error.fill(call_error, qur_line, qur);
            return a;
        }
    }
    if (qur.value!=")"){
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    a.val=true;
    get_qur();
    return a;
}

int is_an_array () {
    int s_i = next_index_in_main_vector;
    auto s_q = qur;
    int s_l = qur_line;
    int amount = 0;
    while (qur.value == "[") {
        amount++;
        get_qur();
        bool min_fl = 0;
        if (qur.type == operation and qur.value == "-") {
            min_fl = true;
            get_qur();
        }
        if (qur.type != int_type) { //first number
            from_save_qur(s_i, s_q, s_l);
            return 0;
        }
        get_qur();
        if (qur.type != punctuation and qur.value != "." and min_fl) { //min is ok only in [a...b] costruction
            from_save_qur(s_i, s_q, s_l);
            return 0;
        }
        if (qur.type == punctuation and qur.value == ".") { //its [a...b] costruction
            for (int i = 0; i < 2; ++i) {
                if (!get_qur()) return 0;
                if (qur.type != punctuation and qur.value != ".") {
                    from_save_qur(s_i, s_q, s_l);
                    return 0;
                }
            }
            get_qur();
            if (qur.type == operation and qur.value == "-") {
                if (min_fl) {
                    if (!get_qur()) {
                        from_save_qur(s_i, s_q, s_l);
                        return 0;
                    }
                }
                else {
                    from_save_qur(s_i, s_q, s_l);
                    return 0;
                }
            }
            if (qur.type != int_type) { //second number
                from_save_qur(s_i, s_q, s_l);
                return 0;
            }
        }
        //its ] after [a or [a..b construction
        if (qur.type != punctuation and qur.value != "]") {
            from_save_qur(s_i, s_q, s_l);
            return 0;
        }
        get_qur();
    }
    return amount;
}
bool_error is_variable_declaration() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (qur.type != res_word and types.find(qur.value) == types.end()) return a;
    name_type name_t = str_to_name_t(qur.value);
    get_qur();
    if (qur.type != identificator) return a;
    token ququr = qur;
    get_qur();
    if (qur.value == "[") { //this is an array
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        int aans = is_an_array();
        if (aans == 0) {
            from_save_qur(s_i, s_q, s_l);
            a = push_tid(ququr.value, name_t, qur_line);
            if (!a.val) return a;
        }
        else {
            a = push_tid_m(ququr.value, nt_to_mas(name_t), qur_line, aans);
            if (!a.val) return a;
        }
    }
    else {
        a = push_tid(ququr.value, name_t, qur_line);
        if (!a.val) return a;
    }
    if (qur.value == "=") {
        get_qur();
        bool_error_expr b_e_x = is_expression();
        a = b_e_x.bool_error;
        if (a.error.type != none) return a;
        tid qur_tid = get_qur_tid();
        if (qur_tid.type != b_e_x.type) {
            a.error.fill(incompatible_types, qur_line, qur);
            return a;
        }
    }
    a.val = true;
    return a;
}
bool_error is_op(name_type s){
    bool_error a;
    a=is_function_call();
    if (error_to_string(a.error)!="none") return a;
    a=is_op_break();
    if (error_to_string(a.error)!="none") return a;
    a=is_op_case(s);
    if (error_to_string(a.error)!="none") return a;
    a=is_op_continue();
    if (error_to_string(a.error)!="none") return a;
    a=is_op_do_while(s);
    if (error_to_string(a.error)!="none") return a;
    a=is_op_for(s);
    if (error_to_string(a.error)!="none") return a;
    a=is_op_get();
    if (error_to_string(a.error)!="none") return a;
    a=is_op_if(s);
    if (error_to_string(a.error)!="none") return a;
    a=is_op_out();
    if (error_to_string(a.error)!="none") return a;
    a=is_op_return(s);
    if (error_to_string(a.error)!="none") return a;
    a=is_op_while(s);
    if (error_to_string(a.error)!="none") return a;
    a.val=false;
    a.error.fill();
    return a;
}

bool_error is_function_header() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (types.find(qur.value) == types.end()) return a;
    get_qur();
    if (qur.type != identificator) {
        a.error.fill(header_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != "(") {
        a.error.fill(header_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != ")" ){
        a = is_header_list();
        if (a.error.type != none) return a;
    }
    if (qur.value != ")") {
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    get_qur();
    a.val = true;
    return a;
}
bool_error is_pocedure_header () {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (qur.value != ">_<") return a;
    get_qur();
    if (qur.type != identificator) {
        a.error.fill(header_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != "(") {
        a.error.fill(header_format_error, qur_line, qur);
        return a;
    }
    get_qur();
    if (qur.value != ")") {
        a = is_header_list();
        if (a.error.type != none) return a;
    }
    if (qur.value != ")") {
        a.error.fill(broken_bracket_balance, qur_line, qur);
        return a;
    }
    get_qur();
    a.val = true;
    return a;
}
bool_error is_main_header() {
    bool_error a;
    a.error.fill();
    a.val = false;
    if (qur.value != "^_^") return a;
    a.val = true;
    get_qur();
    return a;
}

bool_error is_double(){
    bool_error a;
    a.val = false;
    a.error.fill(); //default
    if (qur.type != number) return a;
    if (qur.value.size() > 19) return a; //max size of double 19
    a.error.word.value = qur.value;
    if (!get_qur()) return a;
    if (qur.type != punctuation) return a;
    if (qur.value != ".") return a;
    a.error.word.value += qur.value;
    if (!get_qur()) {
        a.error.fill(double_format_error, qur_line, qur);
        return a;
    }
    if (qur.type != number) {
        a.error.fill(double_format_error, qur_line, qur);
        return a;
    }
    a.error.word.value += qur.value;
    a.val = true;
    return a;
}
bool_error is_float() {
    bool_error a;
    a.val = false;
    a.error.fill(); //default;
    if (qur.value[0] == '0' or qur.value.size() != 1) return a;
    a.error.word.value = qur.value;
    if (!get_qur()) return a;
    if (qur.type != punctuation) return a;
    if (qur.value != ".") return a;
    a.error.word.value += qur.value;
    if (!get_qur()) {
        a.error.fill(float_format_error, qur_line, qur);
        return a;
    }
    if (qur.type != word) return a;
    if (qur.value[qur.value.size() - 1] != 'e') return a;
    for (int i = 0; i < qur.value.size() - 1; ++i) {
        if (numbers.find(qur.value[i]) == numbers.end()) return a;
    }
    a.error.word.value += qur.value;
    if (!get_qur()) return a;
    if (qur.type != operation) {
        a.error.fill(float_format_error, qur_line, qur);
        return a;
    }
    if (not ((qur.value[0] == '+' or qur.value[0] == '-') and qur.value.size() == 1)) {
        a.error.fill(float_format_error, qur_line, qur);
        return a;
    }
    a.error.word.value += qur.value;
    if (!get_qur() or qur.type != number or qur.value[0] == '0') {
        a.error.fill(float_format_error, qur_line, qur);
        return a;
    }
    a.error.word.value += qur.value;
    a.val = true;
    return a;
}

error check_char_constants () {
    error a;
    next_index_in_main_vector = 0;
    qur_line = 1;
    while (get_qur()) {
        if (qur.type == char_constant and qur.value.size() > 1) {
            a.fill(char_format_error, qur_line, qur);
            return a;
        }
    }
    return a;
}
error numbers_to_types() {
    next_index_in_main_vector = 0;
    qur_line = 1;
    bool_error a;
    int i = 0, ii = 1;
    while (get_qur()) {
        if (qur.type == number) {
            i = next_index_in_main_vector - 1; //index of current token which is "number"
            ii = qur_line;
            auto save_qur = qur;
            a = is_float();
            if (a.error.type != none) return a.error;
            if (a.val) {
                main_vector[i].value = a.error.word.value;
                main_vector[i].type = float_type;
                main_vector.erase(main_vector.begin() + i + 1, main_vector.begin() + i + 5);
                next_index_in_main_vector -= 4;
            }
            else {
                next_index_in_main_vector = i + 1;
                qur = save_qur;
                qur_line = ii;
                a = is_double();
                if (a.error.type != none) return a.error;
                if (a.val) {
                    main_vector[i].value = a.error.word.value;
                    main_vector[i].type = double_type;
                    main_vector.erase(main_vector.begin() + i + 1, main_vector.begin() + i + 3);
                    next_index_in_main_vector -= 2;
                }
                else {
                    next_index_in_main_vector = i + 1;
                    qur = save_qur;
                    qur_line = ii;
                    main_vector[i].type = int_type;
                }
            }
        }
    }
    return a.error;
}

error pre_sintax () {
    error a;
    a = check_char_constants();
    if (a.type != none) return a;
    a = numbers_to_types();
    if (a.type != none) return a;
    words_to_ident();
    res_words_to_operations();
    operators_to_typed ();
    a.fill();
    return a;
}

bool get_qur() {
    if (next_index_in_main_vector >= main_vector.size()) return false;
    qur = main_vector[next_index_in_main_vector];
    if (qur.type == my_eof) return false;
    if (qur.type == my_endl) {
        next_index_in_main_vector++;
        qur_line++;
        return(get_qur());
    }
    next_index_in_main_vector++;
    return true;
}

bool_error is_block (name_type s) { // !!!!!   includes its '{'           !!!
    if (! new_level_created) add_level();
    bool_error a;
    bool_error_expr b_e_x;
    a.val = false;
    a.error.fill(); //default
    //string s is the type of the function this block belongs to
    if (qur.value != "{") return a;
    //else
    get_qur();
    while (qur.value != "}" and qur.type != my_eof) {
        //save qur not to spoil anything with analyzes;
        int s_i = next_index_in_main_vector;
        auto s_q = qur;
        int s_l = qur_line;
        // cycles
        a = is_op_do_while(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        a = is_op_while(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else continue; //while checks ; after it's expression
        a = is_op_for(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else continue; //for checks ; after it's expression
        //choise
        a = is_op_case(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else continue; //there is no ; after case construction
        a = is_op_if(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else continue; //if checks ; after it's expression
        //goto
        a = is_op_break();
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        a = is_op_continue();
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        //call
        a = is_function_call();
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        //new
        a = is_variable_declaration(); //or an array
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        //other
        a = is_op_get();
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        a = is_op_out();
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        a = is_op_return(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else goto here;
        a = is_block(s);
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        else continue;
        b_e_x = is_expression();
        a = b_e_x.bool_error;
        if (not a.val or a.error.type != none) {
            if (a.error.type == none) from_save_qur(s_i, s_q, s_l);
            else {
                a.val = false;
                return a;
            }
        }
        if (b_e_x.type != s) {
            a.error.fill(wrong_expression_type, qur_line, qur);
            return a;
        }
//        else goto here;
        here:
        //if it's not an operator;
        if(not a.val or a.error.type != none) break;
        else {
            if (qur.value != ";") {
                a.error.fill(syntax_error, qur_line, qur);   //here we need the ;
                a.val = false;
                break;
            }
            get_qur();
            if (qur.value == "}") break;
        }
    }
    if (qur.value == "}") {
        remove_level();
        new_level_created = false;
        get_qur();
        a.val = true;
        return a;
    }
    else {
        a.val = false;
        if (qur.type == my_eof) a.error.fill (broken_bracket_balance, qur_line, qur);
        else a.error.fill (unexpected_token_error, qur_line, qur);
        return a;
    }
}


error syntax_analise() {
    new_level_created = false;
    error a;
    bool_error b;
    a = pre_sintax ();
    if (a.type != none) return a;
    next_index_in_main_vector = 0;
    qur_line = 1;
    get_qur();
    while (qur.type != my_eof)
    {
        //header
        if (qur.value == "^_^") { //main
            add_level();
            new_level_created = true;
            b = is_main_header();
            if (not b.val) { //a error in main found
                a = b.error;
                return a;
            }
        }
        else if (qur.value == ">_<") { //procedure
            add_level();
            new_level_created = true;
            b = is_pocedure_header();
            if (not b.val) { //a error in procedure found
                a = b.error;
                return a;
            }
        }
        else if (types.find(qur.value) != types.end()) { //function
            add_level();
            new_level_created = true;
            b = is_function_header();
            if (not b.val) { //a error in function found
                a = b.error;
                return a;
            }
        }
        else {
            a.fill(unexpected_token_error, qur_line, qur);
            return a;
        }
        //function or prototype
        if (qur.value == ";") {
            //semantic thing mb
            get_qur();
        }
        else
        if (qur.value == "{") { // block analise
            b = is_block(str_to_name_t(qur.value));
            if (! b.val) {
                a = b.error;
                return a;
            }
        }
        else {
            if (qur.type == my_eof) a.fill(unexpected_end_of_file, qur_line, qur);
            else a.fill(unexpected_token_error, qur_line, qur);
            return a;
        }
    }
    return a;
}



#endif //LEX_ANALISE_SYNTAX_H