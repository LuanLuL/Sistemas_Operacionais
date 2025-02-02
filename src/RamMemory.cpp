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

MemoryPage RamMemory::getProcessBySimilarity(MemoryPage lastProcessRuned) {
    if (this->processQueue.empty()){
        throw runtime_error("RamMemory::getProcessBySimilarity(No processes in the queue)");
    }
    if (lastProcessRuned.process.empty()){ // Se não houver um último processo executado, retorne o primeiro da fila
        MemoryPage selectedProcess = processQueue.front();
        processQueue.pop();
        return selectedProcess;
    }
    int queueSize = processQueue.size();
    if(queueSize == 1){ // Se não houver apenas um processo na fila de espera, retorne o primeiro da fila
        MemoryPage selectedProcess = processQueue.front(); 
        processQueue.pop();
        return selectedProcess;
    }
    double maxSimilarity = -1.0;
    MemoryPage bestMatch;
    bool foundMatch = false;
    
    for (int i = 0; i < queueSize; ++i){
        MemoryPage currentProcess = processQueue.front(); // Percorre a fila para encontrar o processo com maior percentual de similaridade
        processQueue.pop();
        double similarityScore = calculateSimilarityPercentage(lastProcessRuned.process, currentProcess.process);
        if (similarityScore > maxSimilarity && (lastProcessRuned.id != currentProcess.id)){
            maxSimilarity = similarityScore;
            bestMatch = currentProcess;
            foundMatch = true;
        } 
        processQueue.push(currentProcess); // Retorna o processo para a fila de processos em espera
    }
    if (!foundMatch){
        throw runtime_error("RamMemory::getProcessBySimilarity (No process found with similarity)");
    }
    int currentQueueSize = processQueue.size();
    bool removed = false;
    queue<MemoryPage> newQueue;
    for (int i = 0; i < currentQueueSize; ++i) { // remove bestMatch da fila de processos em espera
        MemoryPage currentProcess = processQueue.front();
        processQueue.pop();
        if (!removed && currentProcess.id == bestMatch.id) {
            removed = true;
            continue;
        }
        newQueue.push(currentProcess);
    }
    this->processQueue = newQueue;
    return bestMatch;
}

bool RamMemory::hasProcesses() {
    return !this->processQueue.empty();
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