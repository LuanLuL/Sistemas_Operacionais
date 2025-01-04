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

bool RamMemory::hasProcesses() {
    return !this->processQueue.empty();
}

int RamMemory::getNumberOfProcesses() {
    return this->processQueue.size();
}
