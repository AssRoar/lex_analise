#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "token.h"
#include "lex.h"
#include "syntax.h"
#include "variables.h"
#include "error.h"

void cout_vector() {
    for (auto f: main_vector)
        cout << type_to_string(f.type)
             << "          '" << f.value << "'" << endl;
}

void manual_mode() {
    string s;
    fill_numbers();
    fill_letters();
    getline(cin, s);
    in_lex_analysis(s);
    error a = syntax_analise();
    cout_vector();
    cout << endl << error_to_string(a) << " " << a.line << " " << a.word.value;
}

void file_mode() {
    file.open("code.txt"); // in cmake-build-debug
    lex_analysis();
    file.close();
    error a = syntax_analise();
    cout_vector();
    cout << endl << error_to_string(a) << " " << a.line << " " << a.word.value;
}

int main() {
    file_mode(); // "code.txt" in cmake-build-debug
//    manual_mode();
    return 0;
}
