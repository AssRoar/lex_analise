//
// Created by Alex on 29.01.2020.
//

#ifndef LEX_ANALISE_LEX_H
#define LEX_ANALISE_LEX_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "token.h"
#include "variables.h"
#include <fstream>
using namespace std;

bool make_single (char c) {
    if (c == ' ') return 1;
    string s1 = "";
    s1 += c;
    if (r_punctiation.find(c) != r_punctiation.end()) {
        token a(punctuation, s1);
        main_vector.push_back(a);
        return 1;
    }
    return 0;
}

int is_delimiter (char c) {
    if (c == ' ') return 1;
    string s1 = "";
    s1 += c;
    if (r_punctiation.find(c) != r_punctiation.end()) return 1;
    if (operators_single.find(s1) != operators_single.end() ) return 2;
    if (s1 == "!") return 3;
    return 0;
}

bool is_number (string s) {
    for (char i: s) if (numbers.find(i) == numbers.end()) return false;
    return true;
}

bool is_word (string s) {
    for (char i: s) if (numbers.find(i) == numbers.end() and letters.find(i) == letters.end() and i != '_') return false;
    return true;
}

bool long_analysis (string s) {
    if (operators_single.find(s) != operators_single.end()) {
        token a(operation, s);
        main_vector.push_back(a);
        return true;
    }
    if (r_words.find(s) != r_words.end()) {
        token a(res_word, s);
        main_vector.push_back(a);
        return true;
    }
    if (operators_many.find(s) != operators_many.end()) {
        token a(operation, s);
        main_vector.push_back(a);
        return true;
    }
    if (constants.find(s) != constants.end()) {
        token a(constant, s);
        main_vector.push_back(a);
        return true;
    }
    if (is_number(s)) {
        token a(number, s);
        main_vector.push_back(a);
        return true;
    }
    if (is_word(s)) {
        token a(word, s);
        main_vector.push_back(a);
        return true;
    }
    token a(other, s);
    main_vector.push_back(a);
}

void in_lex_analysis (string s) {
    string s1;
    char c;
    while (not s.empty()) {
        if (s[0] == '\"') {
            int d = 1;
            while (s[d] != '\"' and d < s.size()) {
                s1 += s[d];
                d++;
            }
            if (d < s.size())  {
                token a (string_constant, s1);
                main_vector.push_back(a);
                s.erase(s.begin(), s.begin() + d + 1);
                s1 = "";
                continue;
            }
            else {
                token a (other, "\"");
                main_vector.push_back(a);
                s.erase(s.begin());
                continue;
            }
        }
        if (s.empty()) continue;
        if (s[0] == '\'') {
            int d = 1;
            while (s[d] != '\'' and d < s.size()) {
                s1 += s[d];
                d++;
            }
            if (d < s.size())  {
                token a (char_constant, s1);
                main_vector.push_back(a);
                s.erase(s.begin(), s.begin() + d + 1);
                s1 = "";
                continue;
            }
            else {
                token a (other, "\'");
                main_vector.push_back(a);
                s.erase(s.begin());
                continue;
            }
        }
        if (s.empty()) continue;
        int k = is_delimiter(s[0]);
        if (k == 1) { //this is a delimiter
            if (s[0] == '(' and s.size() >= 3 and s[1] == '-' and s[2] == ')') {
                k = 2;
            }
            else {
                make_single(s[0]);
                s.erase(s.begin());
                continue;
            }
        }
        if (s.empty()) continue;
        if (k == 2) {                  //this is an operation
            if (s.size() == 1) {       //is a single and a LAST  operation
                s1 = s[0];
                token a(operation, s1);
                main_vector.push_back(a);
                s.erase(s.begin());
                continue;
            }
            s1 = s[0];
            s1 += s[1];
            if (s1 == "//") return;
            if (s.size() > 2) {
                s1 += s[2];
                if (s1 == "^_^" or s1 == ">_<") {
                    token a(res_word, s1);
                    main_vector.push_back(a);
                    s.erase(s.begin());
                    s.erase(s.begin());
                    s.erase(s.begin());
                    continue;
                }
                else if (s1 == "(-)") {
                    token a (operation, s1);
                    main_vector.push_back(a);
                    s.erase(s.begin());
                    s.erase(s.begin());
                    s.erase(s.begin());
                    continue;
                }
            }
            s1 = s[0];
            s1 += s[1];
            if (operators_many.find(s1) == operators_many.end()) { //single in context
                s1 = s[0];
                token a(operation, s1);
                main_vector.push_back(a);
                s.erase(s.begin());
                s1 = "";
                continue;
            }
            token a(operation, s1);                     //not single
            main_vector.push_back(a);
            s.erase(s.begin());
            s.erase(s.begin());
            continue;
        }
        else if (k == 3) {
            if (s.length() == 1) {
                token a (other, "!");
                main_vector.push_back(a);
                s.clear();
                s1.clear();
                continue;
            }
            else {
                if (s[1] == '=') {
                    s1 = "!=";
                    token a (operation, s1);
                    main_vector.push_back(a);
                    s.erase(s.begin());
                    s.erase(s.begin());
                    continue;
                }
                else {
                    token a (other, "!");
                    main_vector.push_back(a);
                    s.erase(s.begin());
                    s1.clear();
                    continue;
                }
            }
        }
        c = s[0];
        s1 = "";
        if (s.empty()) continue;
        while (not (s.empty() or is_delimiter(c))) {
            s.erase(s.begin());
            s1 += c;
            c = s[0];
        }
        long_analysis(s1);
        s1 = "";
    }
}

void fill_numbers () {
    for (char c = '0'; c <= '9'; ++c) numbers.insert(c);
}

void fill_letters () {
    for (char c = 'A'; c <= 'Z'; ++c) letters.insert(c);
    for (char c = 'a'; c <= 'z'; ++c) letters.insert(c);
}

void add_eoln () {
    token a (my_endl, "endl");
    main_vector.push_back(a);
}

void add_eof () {
    token a (my_eof, "eof");
    main_vector.push_back(a);
}

void lex_analysis () {
    string s, s1;
    fill_numbers();
    fill_letters();
    while(getline(file, s)) {
        s1 = "";
        for (char c: s) if (c != '\r') s1 += c;
        in_lex_analysis(s1);
        add_eoln();
    }
    add_eof();
}



#endif //LEX_ANALISE_LEX_H
