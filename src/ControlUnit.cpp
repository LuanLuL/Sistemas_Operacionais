#include "ControlUnit.hpp"

ControlUnit::ControlUnit(RegisterBank* regBank, RamMemory* ram) {
    this->bankOfRegister = regBank;
    this->ram = ram;
}

void ControlUnit::executeProcess(const vector<string>&  instructions) {
    for (string lineCode : instructions) {
        cout << lineCode << endl;
    }
}

void ControlUnit::ULA(string comand, int firstRegister, int secondRegister, int addressOfResult) {
    if(this->bankOfRegister->getStatus(firstRegister)){
        //verifica se esta sujo
    } else if(this->bankOfRegister->getStatus(secondRegister)){
        //verifica se esta sujo
    }
    
    if (comand == "ADD") {
    // Adicionar ação para o caso ADD
    } else if (comand == "SUB") {
        // Adicionar ação para o caso SUB
    } else if (comand == "MUL") {
        // Adicionar ação para o caso MUL
    } else if (comand == "DIV") {
        // Adicionar ação para o caso DIV
    } else {
        // Adicionar ação para o caso default
    }
}
