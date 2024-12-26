#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

#include "RamMemory.hpp"
#include "CentralProcessesUnit.hpp"

#define AMOUNT_PROCESSES 2
#define AMOUNT_REGISTERS_ADDRESS 32
#define AMOUNT_MEMORY_ADDRESS 64

using namespace std;

// void readDisc(RamMemory &ram) {
//     for (int i = 1; i <= AMOUNT_PROCESSES; ++i) {
//         string filename = "./data/codigo" + to_string(i) + ".txt";
//         ifstream file(filename);
//         if (file.is_open()) {
//             vector<string> fileLines;
//             string line;
//             while (getline(file, line)) {
//                 fileLines.push_back(line);
//             }
//             MemoryPage page;
//             page.id = i;
//             page.processCount = 1;
//             page.process = fileLines;
//             ram.addProcess(page);
//             file.close();
//         } else {
//             cerr << "Erro ao abrir o arquivo: " << filename << endl;
//         }
//     }
// }

void readManualInput(RamMemory &ram) {
    // Process 1
    vector<string> process1 = {
        "LOAD 5 2",
        "LOAD 2 6",
        "ADD 2 6 3",
        "STORE 2 44",
        "SUB 3 6 12"
    };
    MemoryPage page1;
    page1.id = 1;
    page1.processCount = 1;
    page1.process = process1;
    ram.addProcess(page1);

    // Process 2
    vector<string> process2 = {
        "LOAD 31 8",
        "LOAD 22 13",
        "SUB 8 13 3",
        "STORE 8 25"
    };
    MemoryPage page2;
    page2.id = 2;
    page2.processCount = 1;
    page2.process = process2;
    ram.addProcess(page2);
}

int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(AMOUNT_MEMORY_ADDRESS); // inicia memória RAM
    readManualInput(ram); // carrega processos do disco para a memória RAM
    CentralProcessesUnit cpu(AMOUNT_REGISTERS_ADDRESS); // inicia a CPU
    cout << "\nIniciando execução: \n\n";
    while(ram.hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
        MemoryPage currentProcess = ram.getNextProcess();
        if(cpu.execute(&currentProcess, &ram) == -1){
            ram.addProcess(currentProcess);
        }
    }
    return 0;
}