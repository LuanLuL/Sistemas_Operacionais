#include "InputsOutputs.hpp"

InputsOutputs::InputsOutputs() {
    table["Mouse"] = make_pair(0, false);
    table["Teclado"] = make_pair(0, false);
    table["Monitor"] = make_pair(0, false);
    table["Impressora"] = make_pair(0, false);
    table["Fone de Ouvido"] = make_pair(0, false);
}

void InputsOutputs::add(string key, pair<int, bool> occupied) {
    if (table.find(key) != table.end()) {
        throw runtime_error("InputsOutputs::add(Input/Output has already been added");
    }
    table[key] = occupied;
}

void InputsOutputs::setOcccupied(string key, pair<int, bool> occupied) {
    if (table.find(key) == table.end()) {
        throw runtime_error("InputsOutputs::setOccupied(Input/Output does not exist)");
    }
    table[key] = occupied;
}

void InputsOutputs::setUnoccupied(string key) {
    if (table.find(key) == table.end()) {
        throw runtime_error("InputsOutputs::setOccupied(Input/Output does not exist)");
    }
    table[key] = make_pair(0, false);
}

bool InputsOutputs::isOccupied(string key) const {
    auto it = table.find(key);
    if (it == table.end()) {
        throw runtime_error("InputsOutputs::isOccupied(Input/Output does not exist)");
    }
    return it->second.second;
}

void InputsOutputs::remove(string key) {
    if (table.erase(key) == 0) {
        throw runtime_error("InputsOutputs::remove(Input/Output does not exist)");
    }
}

bool InputsOutputs::exists(string key) const {
    if(table.find(key) == table.end()){
        throw runtime_error("InputsOutputs::remove(Input/Output does not exist)");
    };
    return true;
}