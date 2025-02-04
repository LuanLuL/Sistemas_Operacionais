#include "RamMemory.hpp"

RamMemory::RamMemory(int size) {
    this->size = size;
    if (this->size <= 0) {
        throw invalid_argument("RamMemory::RamMemory(The memory's size must be bigger than 1)");
    }
    this->memoryCells.resize(this->size, createMemoryCell(0, 0, 0, 0));
}

RamMemory::~RamMemory() {
    this->memoryCells.clear();
    while (!this->processQueue.empty()) {
        this->processQueue.pop();
    }
}

vector<MemoryCell> RamMemory::getMemoryCells() {
    return this->memoryCells;
}

MemoryCell RamMemory::createMemoryCell(int newIdProcess, int newRegistrarAddress, int newRegistrarValue, int newRamAddress) {
    MemoryCell newMemoryCell;
    newMemoryCell.idProcess = newIdProcess;
    newMemoryCell.ramAddress = newRamAddress;
    newMemoryCell.registrarAddress = newRegistrarAddress;
    newMemoryCell.registrarValue = newRegistrarValue;
    return newMemoryCell;
}

void RamMemory::write(MemoryCell newMemoryCell, int kindOfWrite) { // A variável "(this->memoryCells[newMemoryCell.ramAddress].idProcess == 0)" implica que a celula de memória está vazia
    if (newMemoryCell.ramAddress < -1 || newMemoryCell.ramAddress >= this->size) { // O valor "newRamAddress == -1" implica que será armazenado em um local desconhecido na memória Ram
        throw out_of_range("RamMemory::write(Address is out of range)");
    }
    switch (kindOfWrite){
        case 0: // executa a operação de STORE
            if(this->memoryCells[newMemoryCell.ramAddress].idProcess == 0 || this->memoryCells[newMemoryCell.ramAddress].idProcess == newMemoryCell.idProcess){ // so adiciona no endereço se ele não estiver ocupado por outro processo qualquer
                this->memoryCells[newMemoryCell.ramAddress] = newMemoryCell;
            }
            break;
        case 1: // limpa o espaço da memória, pois o processo que o ocupava foi finalizado
            this->memoryCells[newMemoryCell.ramAddress] = newMemoryCell;
            break;
        case 2: {  // swap de registrador sujo vindo de um processo não finalizado para a memória
            bool spaceFound = false;
            for(int i = 0; i < this->size; i++){ // armazena no primeiro espaço livre da memória
                if(this->memoryCells[i].idProcess == 0){
                    newMemoryCell.ramAddress = i;  // atualiza o endereço da Ram para o primeiro local vázio encontrado 
                    this->memoryCells[i] = newMemoryCell;
                    spaceFound = true;
                    break;
                }
            }
            if(!spaceFound){ // se nao encontrar um espaço de memória livre implica em tela azul, porque a memória está superlotada
                throw runtime_error("RamMemory::write(MEMORY_MANAGEMENT -> RAM memory's already been totaly occupied )");
            } 
            break;
        }
        default:
                throw invalid_argument("RamMemory::write(Invalid kind of write tried to run)");
            break;
    }
    // caso contrario fica esperando algum processo liberar o espaço de memória para armazenar
}

MemoryCell RamMemory::read(int address) {
    if (address < 0 || address >= size) {
        throw out_of_range("RamMemory::read(Address is out of range)");
    }
    return this->memoryCells[address];
}

void RamMemory::clear(int address) {
    if (address < 0 || address >= size) {
        throw out_of_range("RamMemory::clear(Address is out of range)");
    }
    this->memoryCells[address] = createMemoryCell(0, 0, 0, 0);
}

int RamMemory::getSize() {
    return this->size;
}

void RamMemory::addProcess(MemoryPage process) {
    this->processQueue.push(process);
}

MemoryPage RamMemory::getNextProcess() {
    if (this->processQueue.empty()) {
        throw runtime_error("RamMemory::getNextProcess(No processes in the queue)");
    }
    MemoryPage nextProcess = this->processQueue.front();
    this->processQueue.pop();
    return nextProcess;
}

MemoryPage RamMemory::getProcessWithLeastClocks() {
    if (this->processQueue.empty()) {
        throw runtime_error("RamMemory::getProcessWithLeastClocks(No processes in the queue)");
    }
    queue<MemoryPage> tempQueue;
    MemoryPage minProcess = this->processQueue.front();
    this->processQueue.pop();
    tempQueue.push(minProcess);
    while (!this->processQueue.empty()) {
        MemoryPage current = this->processQueue.front();
        this->processQueue.pop();
        if (current.numberClocksEstimated < minProcess.numberClocksEstimated) {
            minProcess = current;
        }
        tempQueue.push(current);
    }
    queue<MemoryPage> newQueue;
    bool removed = false;
    while (!tempQueue.empty()) {
        MemoryPage current = tempQueue.front();
        tempQueue.pop();
        if (!removed && current.id == minProcess.id && current.numberClocksEstimated == minProcess.numberClocksEstimated) {
            removed = true;
            continue;
        }
        newQueue.push(current);
    }
    this->processQueue = newQueue;
    return minProcess;
}

MemoryPage RamMemory::getProcessByPriority() {
    if (this->processQueue.empty()) {
        throw runtime_error("RamMemory::getProcessByPriority(No processes in the queue)");
    }
    queue<MemoryPage> tempQueue;
    MemoryPage maxProcessPriority = this->processQueue.front();
    this->processQueue.pop();
    tempQueue.push(maxProcessPriority);
    while (!this->processQueue.empty()) {
        MemoryPage current = this->processQueue.front();
        this->processQueue.pop();
        if (current.priority > maxProcessPriority.priority) {
            maxProcessPriority = current;
        }
        tempQueue.push(current);
    }
    queue<MemoryPage> newQueue;
    bool removed = false;
    while (!tempQueue.empty()) {
        MemoryPage current = tempQueue.front();
        tempQueue.pop();
        if (!removed && current.id == maxProcessPriority.id && 
            current.priority == maxProcessPriority.priority) {
            removed = true;
            continue;
        }
        newQueue.push(current);
    }
    this->processQueue = newQueue;
    return maxProcessPriority;
}

MemoryPage RamMemory::getProcessBySimilarity(int lastProcessRuned) {
    if (this->processQueue.empty()) {
        throw runtime_error("RamMemory::getProcessBySimilarity (No processes in the queue)");
    }
    if (lastProcessRuned < 0 || processQueue.size() == 1) {  // Se não houver apenas um processo na fila, retorne o primeiro da fila
        MemoryPage selectedProcess = processQueue.front();   // Se não houver um último processo executado, retorne o primeiro da fila
        processQueue.pop();  
        return selectedProcess;
    }
    queue<MemoryPage> tempQueue = processQueue;
    vector<MemoryPage> processList;
    while (!tempQueue.empty()) {
        processList.push_back(tempQueue.front());
        tempQueue.pop();
    }
    double maxSimilarity = -1.0;
    MemoryPage mostSimilarProcess;
    int mostSimilarIndex = -1;  // Índice do processo mais semelhante
    for (int i = 0; i < this->similarityMatrix.size(); i++) {  // Encontrar o processo mais semelhante
        if (i != lastProcessRuned && isProcessesin(i+1)) {
            double similarity = similarityMatrix[lastProcessRuned][i];
            if (similarity > maxSimilarity) {
                maxSimilarity = similarity;
                mostSimilarIndex = i;
            }
        }
    }
    for (int i = 0; i < processList.size(); i++) {
        MemoryPage item = processList[i];
        if (item.id == mostSimilarIndex+1) {
            mostSimilarProcess = item;
        }
    }
    queue<MemoryPage> updatedQueue; // Remover o processo mais similar da fila
    for (int i = 0; i < processList.size(); i++) {
        MemoryPage item = processList[i];
        if (item.id != mostSimilarProcess.id) {
            updatedQueue.push(processList[i]);
        }
    }
    processQueue = updatedQueue;
    return mostSimilarProcess;
}


bool RamMemory::hasProcesses() {
    return !this->processQueue.empty();
}

bool RamMemory::isProcessesin(int id) {
    queue<MemoryPage> tempQueue = this->processQueue; 
    while (!tempQueue.empty()) {
        if (tempQueue.front().id == id) {
            return true; 
        }
        tempQueue.pop(); 
    }
    return false; 
}



int RamMemory::getNumberOfProcesses() {
    return this->processQueue.size();
}

double RamMemory::calculateSimilarityPercentage(vector<string> a, vector<string> b) {
    if (a.empty() || b.empty()) return 0.0;
    unordered_set<string> setA(a.begin(), a.end());
    int commonElements = 0;
    for (string process : b) {
        if (setA.count(process) > 0) {
            commonElements++;
        }
    }
    int maxSize = max(a.size(), b.size());
    return (static_cast<double>(commonElements) / maxSize) * 100.0; // Calculo da porcentagem de similaridade entre dois vectores
}

void RamMemory::precomputeSimilarities() {
    vector<MemoryPage> processList;
    queue<MemoryPage> tempQueue = processQueue; // Copia os processos para um vetor temporário
    while (!tempQueue.empty()) {
        processList.push_back(tempQueue.front());
        tempQueue.pop();
    }
    int n = processList.size();
    this->similarityMatrix.resize(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double similarity = calculateSimilarityPercentage(processList[i].process, processList[j].process);
            this->similarityMatrix[i][j] = similarity;
            this->similarityMatrix[j][i] = similarity; // Matriz simétrica
        }
    }
}

void RamMemory::finishProcess(int processFinished) {
    for (int i = 0; i < this->similarityMatrix.size(); ++i) { // Zera a similaridade do processFinished com todos os outros
        if (i != processFinished) {
            this->similarityMatrix[processFinished][i] = 0;
            this->similarityMatrix[i][processFinished] = 0; // Como a matriz é simétrica, zera também a posição [i][x]
        }
    }
}
    
