#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include "RamMemory.hpp"
#include "CentralProcessesUnit.hpp"

#define AMOUNT_PROCESSES 3
#define AMOUNT_REGISTERS_ADDRESS 32
#define AMOUNT_MEMORY_ADDRESS 64

using namespace std;
using std::thread;
using std::atomic;

atomic<int> processesExecuted(0);
mutex mtx;

void readDisc(RamMemory &ram);
void monitorProcesses(CentralProcessesUnit *CPU, RamMemory *RAM);
void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram);

int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(AMOUNT_MEMORY_ADDRESS); // inicia memória RAM
    readDisc(ram); // carrega processos do disco para a memória RAM
    CentralProcessesUnit cpu(AMOUNT_REGISTERS_ADDRESS); // inicia a CPU
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tIniciando execução:";
    cout << "\n-------------------------------------------------------------------------------\n";
    thread monitorThread(monitorProcesses, &cpu, &ram);
    if (monitorThread.joinable()) {
        monitorThread.join();
    }
    return 0;
}

void readDisc(RamMemory &ram) {
    for (int i = 1; i <= AMOUNT_PROCESSES; ++i) {
        string filename = "./data/codigo" + to_string(i) + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            vector<string> fileLines;
            string line;
            while (getline(file, line)) {
                fileLines.push_back(line);
            }
            MemoryPage page;
            page.id = i;
            page.processCount = 1;
            page.process = fileLines;
            ram.addProcess(page);
            file.close();
        } else {
            cerr << "Erro ao abrir o arquivo: " << filename << endl;
        }
    }
}

void monitorProcesses(CentralProcessesUnit *CPU, RamMemory *RAM) { // monitora a CPU
    int shouldBeExecuted = RAM->getNumberOfProcesses();
    vector<thread> threads; // gaveta de Threads
    while (processesExecuted < shouldBeExecuted) {
        cout << "\n" << (shouldBeExecuted - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getNextProcess();
            threads.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM));
        }
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::seconds(1)); // observa a acada 1 microssegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------\n\n";

}

void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram) {
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex durante a execução do processo
    cout << "\n\tIniciando execução do processo " << processPage.id;
    int result = cpu->execute(&processPage, ram);
    if (result == -1) { // caso o processo não termine retorna ele para a fila
        ram->addProcess(processPage);
        cout << "\n\tAcabou o tempo quantum do Processo " << processPage.id << ", retornando para a fila.\n";
    } else {
        cout << "\n\tProcesso " << processPage.id << " terminou a execução.\n";
        processesExecuted++;
    }
}