#include "RegistrarsBank.hpp"

RegistrarsBank::RegistrarsBank(int size) {
    this->size = size;
    if(this->size <= 0){
        throw invalid_argument("RegistrarsBank::RegistrarsBank(The registrar bank's size must be bigger than 1)");
    }
    this->registrars.resize(this->size, createRegistrar(0, false));
}

RegistrarsBank::~RegistrarsBank() {
    this->registrars.clear();
}

Registrar RegistrarsBank::createRegistrar(int value, bool dirty) {
    Registrar reg;
    reg.value = value;
    reg.dirty = dirty;
    return reg;
}

vector<Registrar> RegistrarsBank::getRegistrars() {
    return this->registrars;
}

int RegistrarsBank::getSize() {
    return this->size;
}

int RegistrarsBank::getValue(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegistrarsBank::getValue(Address is out of range)");
    }
  return this->registrars[address].value;
}

void RegistrarsBank::setValue(int address, int value) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegistrarsBank::setValue(Address is out of range)");
    }
    this->registrars[address] = createRegistrar(value, false);
}

bool RegistrarsBank::getStatus(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegistrarsBank::setValue(Address is out of range)");
    }
    return this->registrars[address].dirty;
}

void RegistrarsBank::setDirty(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegistrarsBank::setDirty(Address is out of range)");
    }
    this->registrars[address].dirty = true;
}

void RegistrarsBank::setClean(int address) {
    if (address < 0 || address >= this->size) {
        throw out_of_range("RegistrarsBank::setClean(Address is out of range)");
    }
    this->registrars[address].dirty = false;
}

int RegistrarsBank::getPc() {
    return this->Pc;
}

void RegistrarsBank::setPc(int numberOfProgram) {
    if(numberOfProgram <= 0){
        throw invalid_argument("RegistrarsBank::RegistrarsBank(The registra bank's size must be bigger than 1)");
    }
    this->Pc = numberOfProgram;
}

void RegistrarsBank::print() {
    cout<< this->registrars.size() << endl;
    cout<< this->size << endl;
  for (int i = 0; i < this->registrars.size(); i++) {
    cout << "0x" << i << " " << getValue(i) << endl;
  }
}