#include "ControlUnit.hpp"

ControlUnit::ControlUnit(int registrarsAmount) 
    : bankOfRegistrars(registrarsAmount) {}

RegistrarsBank* ControlUnit::getBankOfRegistrars() {
    return &this->bankOfRegistrars;
}

void ControlUnit::executeProcess(const vector<string>&  instructions) {
    for (string lineCode : instructions) {
        cout << lineCode << endl;
    }
}

void ControlUnit::ULA(MemoryPage *processBlock, RamMemory *ram) {
    cout << "Código " << processBlock->id << ": " << processBlock->process[this->bankOfRegistrars.getPc()-1] << endl;
    vector<string> comands = splitLineOfCodeBySpace(processBlock->process[this->bankOfRegistrars.getPc()-1]);
    if (comands[0] == "LOAD") {
        this->bankOfRegistrars.setValue(stoi(comands[2]), stoi(comands[1]));
        this->bankOfRegistrars.setDirty(stoi(comands[2]));
    }
    else if(comands[0] == "STORE"){
        MemoryCell newMemoryCell;
        newMemoryCell.idProcess = processBlock->id;
        newMemoryCell.ramAddress = stoi(comands[2]);
        newMemoryCell.registrarAddress = stoi(comands[1]);
        newMemoryCell.registrarValue = this->bankOfRegistrars.getRegistrars()[stoi(comands[1])].value;
        ram->write(newMemoryCell, 0);
        this->bankOfRegistrars.setClean(stoi(comands[1]));
    }
    else if (comands[0] == "ADD"){
        this->bankOfRegistrars.setValue(stoi(comands[3]), this->bankOfRegistrars.getValue(stoi(comands[1])) + this->bankOfRegistrars.getValue(stoi(comands[2])));
        this->bankOfRegistrars.setDirty(stoi(comands[3]));
    } else if (comands[0] == "SUB"){
        this->bankOfRegistrars.setValue(stoi(comands[3]), this->bankOfRegistrars.getValue(stoi(comands[1])) - this->bankOfRegistrars.getValue(stoi(comands[2])));
        this->bankOfRegistrars.setDirty(stoi(comands[3]));
    } else {
        throw invalid_argument(" ControlUnit::ULA(Invalid Comand input)");  
    }

    // LOAD value register_result
    // STORE register_source register_destiny
    // ADD register_one register_two register_result
    // SUB register_one register_two register_result

    this->bankOfRegistrars.setPc(this->bankOfRegistrars.getPc() + 1);
    
}

vector<string> ControlUnit::splitLineOfCodeBySpace(string input) {
    vector<string> words;
    istringstream stream(input);
    string word;
    while (stream >> word) {
        words.push_back(word);
    }
    return words;
}
