//
// Created by Alex on 30.04.2020.
//

#ifndef LEX_ANALISE_TIF_H
#define LEX_ANALISE_TIF_H

#include <iostream>
#include <vector>
#include "error.h"
using namespace std;

class tif {
public:
    string name;
    name_type type;
    bool has_block;
    vector <name_type> FP;
    tif (): name (""), type(default_type) {}
    tif(string s, name_type t, bool b): name(s), type(t), has_block(b) {}
};

vector <tif> tif_tree;
tif *qur_tif;

void push_tif (string name, name_type type, bool b,  int line) {
    tif my_tif(name, type, b);
    tif_tree.push_back(my_tif);
}

bool is_declared_prototipe (string name) {
    for (int i = 0; i < tif_tree.size(); ++i) {
        if (tif_tree[i].name == name && !tif_tree[i].has_block) return true;
    }
    return false;
}

bool is_declared_function (string name) {
    for (int i = 0; i < tif_tree.size(); ++i) {
        if (tif_tree[i].name == name && tif_tree[i].has_block) return true;
    }
    return false;
}

bool has_a_prototipe (string name) {
    for (int i = 0; i < tif_tree.size(); ++i) {
        //if (tif_tree[i].name == name && !tif_tree[i].has_block && ) return true;
    }
    return false;
}




#endif //LEX_ANALISE_TIF_H
