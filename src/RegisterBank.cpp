#include "RegisterBank.hpp"

RegisterBank::RegisterBank(int size) { 
    if(size <= 0){
        throw invalid_argument("RegisterBank::RegisterBank(The register bank's size must be bigger than 1)");
    }
    this->registers.resize(size, createRegister(0, false));
}

RegisterBank::~RegisterBank() {
    this->registers.clear();
}

Register RegisterBank::createRegister(int value, bool dirty) {
    Register reg;
    reg.value = value;
    reg.dirty = dirty;
    return reg;
}

int RegisterBank::getValue(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegisterBank::getValue(Address is out of range)");
    }
  return this->registers[address].value;
}

void RegisterBank::setValue(int address, int value) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegisterBank::setValue(Address is out of range)");
    }
    this->registers[address].value = value;
}

void RegisterBank::setDirty(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegisterBank::setDirty(Address is out of range)");
    }
    this->registers[address].dirty = true;
}

void RegisterBank::setClean(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegisterBank::setClean(Address is out of range)");
    }
    this->registers[address].dirty = false;
}

int RegisterBank::getPc() {
    return this->Pc;
}

void RegisterBank::setPc(int numberOfProgram) {
    if(numberOfProgram <= 0){
        throw invalid_argument("RegisterBank::RegisterBank(The register bank's size must be bigger than 1)");
    }
    this->Pc = numberOfProgram;
}

void RegisterBank::print() {
  for (int i = 0; i < this->registers.size(); i++) {
    cout << "0x" << i << " " << this->getValue(i) << endl;
  }
}