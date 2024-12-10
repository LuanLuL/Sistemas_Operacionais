#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <vector>
#include <string>

#include "RegisterBank.hpp"
#include "RamMemory.hpp"

using namespace std;

class ControlUnit {
    private:
        RegisterBank* bankOfRegister;
        RamMemory* ram;
    public:
        ControlUnit(RegisterBank* regBank, RamMemory* ram);
        void executeProcess(const vector<string>&  instructions);
        void ULA(string comand, int firstRegister, int secondRegister, int addressOfResult);
};

#endif
