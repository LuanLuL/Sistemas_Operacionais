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
#include "InputsOutputs.hpp"

#define AMOUNT_PROCESSES 6
#define AMOUNT_REGISTERS_ADDRESS 32
#define AMOUNT_MEMORY_ADDRESS 1024

using namespace std;
using std::thread;
using std::atomic;

atomic<int> processesExecuted(0);
mutex mtx;

void readDisc(RamMemory &ram);
void monitorProcesses(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs);
void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram, InputsOutputs *io);

int main() {
    auto inicio = std::chrono::high_resolution_clock::now();
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(AMOUNT_MEMORY_ADDRESS); // inicia memória RAM
    readDisc(ram); // carrega processos do disco para a memória RAM
    CentralProcessesUnit cpu(AMOUNT_REGISTERS_ADDRESS); // inicia a CPU
    InputsOutputs inOut;
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tIniciando execução:";
    cout << "\n-------------------------------------------------------------------------------";
    thread monitorThread(monitorProcesses, &cpu, &ram, &inOut);
    if (monitorThread.joinable()) {
        monitorThread.join();
    }

    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio); // calcula o tempo de duração do programa em milessegundos
    cout << "Tempo de execução: " << duracao.count() << " ms\n\n";
    return 0;
}

void readDisc(RamMemory &ram) {
    for (int i = 1; i <= AMOUNT_PROCESSES; ++i) {
        string filename = "./data/codigo" + to_string(i) + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            MemoryPage page;
            vector<string> fileLines;
            string line;
            getline(file, line);
            page.inputOutput = line;
            while (getline(file, line)) {
                fileLines.push_back(line);
            }
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

void monitorProcesses(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs) { // monitora a CPU
    vector<thread> threads; // gaveta de Threads
    while (processesExecuted < AMOUNT_PROCESSES) {
        cout << "\n\n" << (AMOUNT_PROCESSES - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getNextProcess();
            if(inputsOutputs->isOccupied(currentProcess.inputOutput)){ // verifica a concorrência entre os Recursos do SO
                cout << "\n\tProcesso " << currentProcess.id << " não pode executar agora, pois o " << currentProcess.inputOutput << " está ocupado.";
                RAM->addProcess(currentProcess);
                break;
            }else{
                inputsOutputs->setOcccupied(currentProcess.inputOutput, make_pair(currentProcess.id, true));
                threads.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM, inputsOutputs));
            }
            
        }
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::microseconds(1000)); // observa a acada 1 milissegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------\n\n";

}

void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram, InputsOutputs *io) {
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex durante a execução do processo
    cout << "\n\tIniciando execução do processo " << processPage.id << " que usa o " << processPage.inputOutput << ".";
    int result = cpu->execute(&processPage, ram);
    if (result == -1) { // caso o processo não termine retorna ele para a fila
        ram->addProcess(processPage);
        cout << "\n\tAcabou o tempo quantum do Processo " << processPage.id << ", retornando para a fila.";
        cout << "\n\tLiberando o " << processPage.inputOutput << ".";
        io->setUnoccupied(processPage.inputOutput);
    } else {
        cout << "\n\tProcesso " << processPage.id << " terminou a execução.";
        cout << "\n\tLiberando o " << processPage.inputOutput << ".";
        io->setUnoccupied(processPage.inputOutput);
        processesExecuted++;
    }
}