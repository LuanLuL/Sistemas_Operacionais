#include "RamMemory.hpp"

RamMemory::RamMemory(int size) {
    if(size <= 0){
        throw invalid_argument("RamMemory::RamMemory(The memory's size must be bigger than 1)");
    }
    this->size = size;
    this->memoryCells = new int[size];
    for (int i = 0; i < this->size; ++i) {
        this->memoryCells[i] = 0;
    }
}

RamMemory::~RamMemory() {
    delete[] this->memoryCells;
    this->memoryProcesses.clear();
}

void RamMemory::write(int value, int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RamMemory::write(Address is out of range)");
    }
    this->memoryCells[address] = value;
}

int RamMemory::read(int address) {
    if (address < 0 || address >= size) {
        throw out_of_range("RamMemory::read(Address is out of range)");
    }
    return this->memoryCells[address];
}

string RamMemory::search(int address) {
    if (address < 0 || address >= this->memoryProcesses.size()) {
        throw out_of_range("RamMemory::search(Address is out of range)");
    }
    return memoryProcesses[address];
}

int RamMemory::getSize() {
    return this->size;
}