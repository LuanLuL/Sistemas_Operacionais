#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <vector>
#include <string>

#include "RegistrarsBank.hpp"
#include "RamMemory.hpp"

using namespace std;

class ControlUnit {
    private:
        RegistrarsBank* bankOfRegistrars;
    public:
        ControlUnit(RegistrarsBank* regBank);
        void executeProcess(const vector<string>&  instructions);
        void ULA(string comand, int firstRegistrar, int secondRegistrar, int addressOfResult);
};

#endif
