#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <vector>

#include "RamMemory.hpp"
#include "RegistrarsBank.hpp"

#define AMOUNT_PROCESSES 2

using namespace std;

vector<vector<string>> readDisc() {
    vector<vector<string>> filesContent(AMOUNT_PROCESSES);
    for (int i = 1; i <= AMOUNT_PROCESSES; ++i) {
        string filename = "./data/codigo" + to_string(i) + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                filesContent[i - 1].push_back(line);
            }
            file.close();
        } else {
            cerr << "Erro ao abrir o arquivo: " << filename << endl;
        }
    }
    return filesContent;
}


int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(AMOUNT_PROCESSES*32, readDisc());
    
    // RegistrarsBank registrars(32);
    // cout << "\n";
    // registrars.print();
    // cout << "\n\n";
    return 0;
}