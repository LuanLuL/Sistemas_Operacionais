#include "ControlUnit.hpp"

ControlUnit::ControlUnit(int id, RegistrarsBank* regBank) {
    this->id = id;
    this->clocks = 0;
    this->bankOfRegistrars = regBank;
}

int ControlUnit::getId() {
    return this->id;
}

int ControlUnit::getClocks() {
    return this->clocks;
}

void ControlUnit::executeProcess(const vector<string>&  instructions) {
    for (string lineCode : instructions) {
        cout << lineCode << endl;
    }
}

void ControlUnit::ULA(vector<string> code) {
    while(this->bankOfRegistrars->getPc() < code.size()){
        vector<string> comands = splitLineOfCodeBySpace(code[this->bankOfRegistrars->getPc()]);
        if (comands[0] == "LOAD") {
            this->bankOfRegistrars->setValue(stoi(comands[2]), stoi(comands[1]));
            this->bankOfRegistrars->setDirty(stoi(comands[1]));
        }
        else if (comands[0] == "ADD"){
            this->bankOfRegistrars->setValue(stoi(comands[3]), this->bankOfRegistrars->getValue(stoi(comands[1])) + this->bankOfRegistrars->getValue(stoi(comands[2])));
            this->bankOfRegistrars->setDirty(stoi(comands[3]));
        } else if (comands[0] == "SUB"){
            this->bankOfRegistrars->setValue(stoi(comands[3]), this->bankOfRegistrars->getValue(stoi(comands[1])) - this->bankOfRegistrars->getValue(stoi(comands[2])));
            this->bankOfRegistrars->setDirty(stoi(comands[3]));
        } else if (comands[0] == "MUL"){
            this->bankOfRegistrars->setValue(stoi(comands[3]), this->bankOfRegistrars->getValue(stoi(comands[1])) * this->bankOfRegistrars->getValue(stoi(comands[2])));
            this->bankOfRegistrars->setDirty(stoi(comands[3]));
        } else if (comands[0] == "DIV"){
            this->bankOfRegistrars->setValue(stoi(comands[3]), this->bankOfRegistrars->getValue(stoi(comands[1])) / this->bankOfRegistrars->getValue(stoi(comands[2])));
            this->bankOfRegistrars->setDirty(stoi(comands[3]));
        } else {
          throw invalid_argument(" ControlUnit::ULA(Invalid Comand input)");  
        }

        // LOAD value register_result
        // ADD register_one register_two register_result

        this->bankOfRegistrars->setPc(this->bankOfRegistrars->getPc() + 1);
        this->clocks++;
    }
    
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
