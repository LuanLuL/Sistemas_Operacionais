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
#include "Cache.hpp"
#include "CentralProcessesUnit.hpp"
#include "InputsOutputs.hpp"

#define AMOUNT_PROCESSES 6 // NÃO RECOMENDADO AUMENTAR O NÚMERO DE PROCESSOS PARA MAIS DE QUATRO !!!
#define AMOUNT_REGISTERS_ADDRESS 1032
#define AMOUNT_MEMORY_ADDRESS 1032
#define AMOUNT_CASHE_ADDRESS 32

using namespace std;
using std::thread;
using std::atomic;

atomic<int> processesExecuted(0);
mutex mtx;
bool preemptivo = false;

void readDisc(RamMemory &ram);
void firstComeFirstService(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory);
void shortestJobFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory);
void highestPriorityFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory);
void greatestSimilarityFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory);
void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram, InputsOutputs *io, Cache *cacheMemory);

int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(AMOUNT_MEMORY_ADDRESS); // inicia memória RAM
    Cache cache(AMOUNT_CASHE_ADDRESS);
    readDisc(ram); // carrega processos do disco para a memória RAM
    CentralProcessesUnit cpu(AMOUNT_REGISTERS_ADDRESS); // inicia a CPU
    InputsOutputs inOut;
    int option = 0;
    string option2 = "";
    cout << "\nEscolha algum método de escalonamento: \n\n";
    cout << "1 - First Job First Service (FIFO)\n";
    cout << "2 - Shortest Job First (SJF)\n";
    cout << "3 - Highest Priority First (HPF)\n";
    cout << "4 - Greatest Similarity First (GSF)\n";
    cout << "5 - Sair\n";
    cout << "\nDigite sua opção: ";
    cin >> option;
    if(option != 5){ // questiona se irá considerar preempçao ou não
        bool respondeuCerto = true;
        while(respondeuCerto){
            cout << "\nGostaria de executar de forma preemptiva (sim / nao)? ";
            cin >> option2;
            if(option2 == "S" || option2 == "s" || option2 == "Sim" || option2 == "sim" || option2 == "SIM"){
                preemptivo = true;
                respondeuCerto = false;
            } else if(option2 == "N" || option2 == "n" || option2 == "Nao" || option2 == "nao" || option2 == "NAO" || option2 == "Não" || option2 == "não" || option2 == "NÃO"){
                preemptivo = false;
                respondeuCerto = false;
            } else {
                cout << "\nOpção inválida. Por favor, tente novamente.\n";
            }
        }
    }
    switch (option) {
        case 1: {
            cout << "\n-------------------------------------------------------------------------------\n";
            cout << "\tIniciando execução por First Job First Service (FIFO): ";
            cout << "\n-------------------------------------------------------------------------------";
            auto inicio = std::chrono::high_resolution_clock::now();
            thread monitorThread(firstComeFirstService, &cpu, &ram, &inOut, &cache);
            if (monitorThread.joinable()) {
                monitorThread.join();
            }
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio); // calcula o tempo de duração do programa em milessegundos
            cout << "\n\nTempo de execução por First Job First Service (FIFO)" << (preemptivo ? " preemptivo: " : " não preemptvo: ") << duracao.count() << " ms\n\n";
            break;
        }
        case 2: {
            cout << "\n-------------------------------------------------------------------------------\n";
            cout << "\tIniciando execução por Shortest Job First (SJF): ";
            cout << "\n-------------------------------------------------------------------------------";
            auto inicio = std::chrono::high_resolution_clock::now();
            thread monitorThread(shortestJobFirst, &cpu, &ram, &inOut, &cache);
            if (monitorThread.joinable()) {
                monitorThread.join();
            }
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio); // calcula o tempo de duração do programa em milessegundos
            cout << "\n\nTempo de execução por Shortest Job First (SJF)" << (preemptivo ? " preemptivo: " : " não preemptvo: ") << duracao.count() << " ms\n\n";
            break;
        }
        case 3: {
            cout << "\n-------------------------------------------------------------------------------\n";
            cout << "\tIniciando execução por Highest Priority First (HPF): ";
            cout << "\n-------------------------------------------------------------------------------";
            auto inicio = std::chrono::high_resolution_clock::now();
            thread monitorThread(highestPriorityFirst, &cpu, &ram, &inOut, &cache);
            if (monitorThread.joinable()) {
                monitorThread.join();
            }
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio); // calcula o tempo de duração do programa em milessegundos
            cout << "\n\nTempo de execução por Highest Priority First (HPF)" << (preemptivo ? " preemptivo: " : " não preemptvo: ") << duracao.count() << " ms\n\n";
            break;
        }
        case 4: {
            cout << "\n-------------------------------------------------------------------------------\n";
            cout << "\tIniciando execução por Greatest Similarity First (GSF): ";
            cout << "\n-------------------------------------------------------------------------------";
            auto inicio = std::chrono::high_resolution_clock::now();
            thread monitorThread(greatestSimilarityFirst, &cpu, &ram, &inOut, &cache);
            if (monitorThread.joinable()) {
                monitorThread.join();
            }
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio); // calcula o tempo de duração do programa em milessegundos
            cout << "\n\nTempo de execução por Greatest Similarity First (GSF)" << (preemptivo ? " preemptivo: " : " não preemptvo: ") << duracao.count() << " ms\n\n";
            break;
        }
        case 5 :
            cout << "\nSaindo... \n\n";
            break;
        default:
            cout << "\nOpção inválida. Por favor, tente novamente.\n\n";
    }
    return 0;
}

void readDisc(RamMemory &ram) {
    for (int i = 1; i <= AMOUNT_PROCESSES; ++i) {
        string filename = "./data/codigo" + to_string(i) + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            MemoryPage page;
            vector<string> fileLines;
            int numberClocksEstimated = 0;
            string line;
            if (getline(file, line)) { // Ler a primeira linha
                istringstream iss(line);
                string inputOutputStr, priorityStr;
                if (!getline(iss, inputOutputStr, ';') || !getline(iss, priorityStr, ';')) { // separa linha pelo ';'
                    cerr << "Erro ao processar a primeira linha do arquivo: " << filename << endl;
                    continue; // Pula para o próximo arquivo
                }
                page.inputOutput = inputOutputStr; // armazena variável de controle no processo
                page.priority = stoi(priorityStr); // armazena variável de controle no processo
            }
            while (getline(file, line)) {  // Ler as demais linhas
                fileLines.push_back(line);
                istringstream iss(line);
                vector<string> words;
                string word;
                while (iss >> word) {
                    words.push_back(word);
                }
                if (words.size() >= 3 && words[0] == "FOR") {
                    numberClocksEstimated += stoi(words[2]);
                } else {
                    numberClocksEstimated += 1;
                }
            }
            page.id = i;
            page.processCount = 1;
            page.process = fileLines;
            page.numberClocksEstimated = numberClocksEstimated;
            ram.addProcess(page);
            file.close();
        } else {
            cerr << "Erro ao abrir o arquivo: " << filename << endl;
        }
    }
    ram.precomputeSimilarities();
}

void firstComeFirstService(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory) { // monitora a CPU
    vector<thread> nucleos; // gaveta de núcleos
    while (processesExecuted < AMOUNT_PROCESSES) {
        cout << "\n\n" << (AMOUNT_PROCESSES - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getNextProcess(); // seleciona o primeiro processo
            if(inputsOutputs->isOccupied(currentProcess.inputOutput)){ // verifica a concorrência entre os Recursos do SO
                cout << "\n\tProcesso " << currentProcess.id << " não pode executar agora, pois o " << currentProcess.inputOutput << " está ocupado.";
                RAM->addProcess(currentProcess);
                break;
            }else {
                inputsOutputs->setOcccupied(currentProcess.inputOutput, make_pair(currentProcess.id, true));
                nucleos.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM, inputsOutputs, cacheMemory));
            }
        }
        for (auto& th : nucleos) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::microseconds(1000)); // observa a acada 1 milissegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------";
}

void shortestJobFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory) {
    vector<thread> nucleos; // gaveta de núcleos
    while (processesExecuted < AMOUNT_PROCESSES) {
        cout << "\n\n" << (AMOUNT_PROCESSES - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getProcessWithLeastClocks(); // seleciona o menor processo
            if(inputsOutputs->isOccupied(currentProcess.inputOutput)){ // verifica a concorrência entre os Recursos do SO
                cout << "\n\tProcesso " << currentProcess.id << " não pode executar agora, pois o " << currentProcess.inputOutput << " está ocupado.";
                RAM->addProcess(currentProcess);
                break;
            }else{
                inputsOutputs->setOcccupied(currentProcess.inputOutput, make_pair(currentProcess.id, true));
                nucleos.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM, inputsOutputs, cacheMemory));
            } 
        }
        for (auto& th : nucleos) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::microseconds(1000)); // observa a acada 1 milissegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------";
}

void highestPriorityFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory) {
    vector<thread> nucleos; // gaveta de núcleos
    while (processesExecuted < AMOUNT_PROCESSES) {
        cout << "\n\n" << (AMOUNT_PROCESSES - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getProcessByPriority(); // sorteia processo por loteria
            if(inputsOutputs->isOccupied(currentProcess.inputOutput)){ // verifica a concorrência entre os Recursos do SO
                cout << "\n\tProcesso " << currentProcess.id << " não pode executar agora, pois o " << currentProcess.inputOutput << " está ocupado.";
                RAM->addProcess(currentProcess);
                break;
            }else{
                inputsOutputs->setOcccupied(currentProcess.inputOutput, make_pair(currentProcess.id, true));
                nucleos.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM, inputsOutputs, cacheMemory));
            }
        }
        for (auto& th : nucleos) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::microseconds(1000)); // observa a acada 1 milissegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------";
}

void greatestSimilarityFirst(CentralProcessesUnit *CPU, RamMemory *RAM, InputsOutputs *inputsOutputs, Cache *cacheMemory) {
    vector<thread> nucleos; // gaveta de núcleos
    MemoryPage lastProcessRuned; 
    lastProcessRuned.id = 0;
    while (processesExecuted < AMOUNT_PROCESSES) {
        cout << "\n\n" << (AMOUNT_PROCESSES - processesExecuted) << " processos restantes\n";
        while(RAM->hasProcesses()){ // enquanto tiver processos para rodar, execute os processo
            MemoryPage currentProcess = RAM->getProcessBySimilarity((lastProcessRuned.id-1)); // busca o processo com maior similaridade
            lastProcessRuned = currentProcess; // o utlimo se torna o atual
            if(inputsOutputs->isOccupied(currentProcess.inputOutput)){ // verifica a concorrência entre os Recursos do SO
                cout << "\n\tProcesso " << currentProcess.id << " não pode executar agora, pois o " << currentProcess.inputOutput << " está ocupado.";
                RAM->addProcess(currentProcess);
                break;
            }else {
                inputsOutputs->setOcccupied(currentProcess.inputOutput, make_pair(currentProcess.id, true));
                nucleos.push_back(thread(executeProcessInThread, CPU, currentProcess, RAM, inputsOutputs, cacheMemory));
            }
        }
        for (auto& th : nucleos) {
            if (th.joinable()) {
                th.join();
            }
        }
        this_thread::sleep_for(chrono::microseconds(1000)); // observa a acada 1 milissegundo
    }
    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\tEncerrando execução de processos";
    cout << "\n-------------------------------------------------------------------------------";
}

void executeProcessInThread(CentralProcessesUnit* cpu, MemoryPage processPage, RamMemory* ram, InputsOutputs *io, Cache *cacheMemory) {
    lock_guard<mutex> lock(mtx); // Bloqueia o mutex durante a execução do processo
    cout << "\n\tIniciando execução do processo " << processPage.id << " que usa o " << processPage.inputOutput << ".";
    int result = cpu->execute(&processPage, ram, cacheMemory, preemptivo);
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
        ram->finishProcess((processPage.id-1));
    }
}