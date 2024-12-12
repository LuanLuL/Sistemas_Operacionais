#include "ControlUnit.hpp"

ControlUnit::ControlUnit(RegistrarsBank* regBank) {
    this->bankOfRegistrars = regBank;
}

void ControlUnit::executeProcess(const vector<string>&  instructions) {
    for (string lineCode : instructions) {
        cout << lineCode << endl;
    }
}

void ControlUnit::ULA(string comand, int firstRegistrar, int secondRegistrar, int addressOfResult) {
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
