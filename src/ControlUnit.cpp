#include "ControlUnit.hpp"

ControlUnit::ControlUnit(int registrarsAmount) 
    : bankOfRegistrars(registrarsAmount) {}

RegistrarsBank* ControlUnit::getBankOfRegistrars() {
    return &this->bankOfRegistrars;
}

void ControlUnit::ULA(MemoryPage *processBlock, RamMemory *ram) {
   vector<string> comands = splitLineOfCodeBySpace(processBlock->process[this->bankOfRegistrars.getPc()-1]);
    if(comands[0] == "ADD-VALUE"){
        this->bankOfRegistrars.setValue(stoi(comands[2]), (this->bankOfRegistrars.getValue(stoi(comands[2])) + stoi(comands[1])));
        this->bankOfRegistrars.setDirty(stoi(comands[2]));
    }
    else if(comands[0] == "SUB-VALUE"){
        this->bankOfRegistrars.setValue(stoi(comands[2]), (this->bankOfRegistrars.getValue(stoi(comands[2])) - stoi(comands[1])));
        this->bankOfRegistrars.setDirty(stoi(comands[2]));
    }
    else if (comands[0] == "LOAD") {
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
    } else if(comands[0] == "IF"){
        if(comands[1] == "<"){
            if(this->bankOfRegistrars.getValue(stoi(comands[2])) < this->bankOfRegistrars.getValue(stoi(comands[3]))){
                this->bankOfRegistrars.setValue(stoi(comands[4]), 6666); // 6666 implica em verdadeiro 
                this->bankOfRegistrars.setDirty(stoi(comands[4]));
            } else {
                this->bankOfRegistrars.setValue(stoi(comands[4]), -6666); // -6666 implica em falso
                this->bankOfRegistrars.setDirty(stoi(comands[4]));
            }
        } else if(comands[1] == ">"){
            if(this->bankOfRegistrars.getValue(stoi(comands[2])) > this->bankOfRegistrars.getValue(stoi(comands[3]))){
                this->bankOfRegistrars.setValue(stoi(comands[4]), 6666); // 6666 implica em verdadeiro
                this->bankOfRegistrars.setDirty(stoi(comands[4]));
            }
            else{
                this->bankOfRegistrars.setValue(stoi(comands[4]), -6666); // -6666 implica em falso
                this->bankOfRegistrars.setDirty(stoi(comands[4]));
            }
        }
    } else if(comands[0] == "FOR"){
        int amountTimes = stoi(comands[2]);
        if(comands[1] == "SUB"){
            for(int i = 0; i < amountTimes; i++){
                processBlock->process.insert((processBlock->process.begin() + this->bankOfRegistrars.getPc()), "SUB-VALUE " + comands[3] + " " + comands[4]);
            }
        } else if(comands[1] == "ADD"){
            for(int i = 0; i < amountTimes; i++){
                processBlock->process.insert((processBlock->process.begin() + this->bankOfRegistrars.getPc()), "ADD-VALUE " + comands[3] + " " + comands[4]);
            }
        }
    } else {
        throw invalid_argument(" ControlUnit::ULA(Invalid Comand input)");  
    }

    // LOAD value register_result
    // STORE register_source register_destiny
    // ADD register_one register_two register_result
    // SUB register_one register_two register_result
    // IF operation(< ou >) register_one register_two register_result
    // FOR operation(SUB ou ADD) amount_times value register_result

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
