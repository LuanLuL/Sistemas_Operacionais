#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <vector>

#include "RamMemory.hpp"
#include "CentralProcessesUnit.hpp"

#define AMOUNT_PROCESSES 1
#define AMOUNT_REGISTERS_ADDRESS 32
#define AMOUNT_MEMORY_ADDRESS AMOUNT_PROCESSES*AMOUNT_REGISTERS_ADDRESS

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
    RamMemory ram(AMOUNT_MEMORY_ADDRESS, readDisc());
    CentralProcessesUnit cpu;
    for(int i = 0; i < AMOUNT_PROCESSES; i++){
        RegistrarsBank* bank = new RegistrarsBank(AMOUNT_REGISTERS_ADDRESS);
        ControlUnit process(i, bank);
        cpu.addProcess(process);
    }
    while (cpu.hasProcesses()){
        ControlUnit currentProcess =  cpu.getNextProcess();
        currentProcess.ULA(ram.search(currentProcess.getId()));
    }
    

    return 0;
}