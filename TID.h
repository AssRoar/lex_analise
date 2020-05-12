//
// Created by Alex on 27.04.2020.
//

#ifndef LEX_ANALISE_TID_H
#define LEX_ANALISE_TID_H

#include <iostream>
#include <vector>
#include "error.h"
#include <set>

using namespace std;

enum name_type {
    float_t,
    bool_t,
    string_t,
    int_t,
    double_t,
    char_t,
    float_t_m,
    bool_t_m,
    string_t_m,
    int_t_m,
    double_t_m,
    char_t_m,
    default_type
};

class tid {
public:
    string name;
    name_type type;
    int fields_amount;
    tid(string name_m, name_type type_m): name(name_m), type(type_m) {};
    tid(string name_m, name_type type_m, int am): name(name_m), type(type_m), fields_amount(am) {};
    tid (): name (""), type(default_type) {};
};

vector <vector<tid> > tid_tree;
pair <int, int> qur_tid_i;

tid get_qur_tid () {
    return tid_tree[tid_tree.size() - 1][tid_tree[tid_tree.size() - 1].size() - 1];
}

//tid get_tid(string s) {
//    tid *ans;
//    for (int i = 0; i < tid_tree.size(); i++) {
//        qur_tid = &tid_tree[i];
//        while (qur_tid->next != nullptr) if(qur_tid->name == s) ans = qur_tid;
//    }
//    get_qur_tid ();
//    return ans;
//}

void add_level () {
    vector <tid> lav;
    tid_tree.push_back(lav);
}

void remove_level () {
    tid_tree.pop_back();
}

bool_error push_tid (string name, name_type type, int line) {
    token aa(identificator, name);
    bool_error a;
    a.error.fill(redeclaration, line, aa);
    a.val = false;
    int i = tid_tree.size() - 1;
    int ii = tid_tree[i].size() - 1;
    for (int j = 0; j < ii; j++) {
        if (tid_tree[i][j].name == name) return a;
    }
    a.error.fill(none);
    tid t (name, type);
    tid_tree[i].push_back(t);
    a.val = true;
    return a;
}

name_type nt_to_mas (name_type nt) {
    if (nt == float_t or nt == float_t_m) return float_t_m;
    else if (nt == bool_t or nt == bool_t_m) return bool_t_m;
    else if (nt == string_t or nt == string_t_m) return string_t_m;
    else if (nt == int_t or nt == int_t_m) return int_t_m;
    else if (nt == double_t or nt == double_t_m) return double_t_m;
    else if (nt == char_t or nt == char_t_m) return char_t_m;
}

name_type mas_to_nt (name_type nt) {
    if (nt == float_t or nt == float_t_m) return float_t;
    else if (nt == bool_t or nt == bool_t_m) return bool_t;
    else if (nt == string_t or nt == string_t_m) return string_t;
    else if (nt == int_t or nt == int_t_m) return int_t;
    else if (nt == double_t or nt == double_t_m) return double_t;
    else if (nt == char_t or nt == char_t_m) return char_t;
}

bool is_mas (name_type nt) {
    return nt == float_t_m or nt == bool_t_m or nt == string_t_m or nt == int_t_m or nt == double_t_m or nt == char_t_m;
}

bool_error push_tid_m (string name, name_type type, int line, int num) {
    token aa(identificator, name);
    bool_error a;
    a.error.fill(redeclaration, line, aa);
    a.val = false;
    int i = tid_tree.size() - 1;
    for (int j = 0; j < tid_tree[i].size(); j++) {
        if (tid_tree[i][j].name == name) return a;
    }
    a.error.fill(none);
    tid t (name, nt_to_mas(type), num);
    tid_tree[i].push_back(t);
    a.val = true;
    return a;
}

name_type is_declared (string name) { //returns default_type if not found
    for (int i = tid_tree.size() - 1; i >= 0; i--) {
        for (int j = 0; j < tid_tree[i].size(); j++) {
            if (tid_tree[i][j].name == name) return tid_tree[i][j].type;
        }
    }
    return default_type;
}

name_type str_to_name_t (string s) {
    if (s == "float") return float_t;
    else if (s == "bool") return bool_t;
    else if (s == "string") return string_t;
    else if (s == "int") return int_t;
    else if (s == "double") return double_t;
    else if (s == "char") return char_t;
    else return default_type;
}

#endif //LEX_ANALISE_TID_H
