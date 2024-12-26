#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <vector>
#include <string>
#include <sstream>

#include "RegistrarsBank.hpp"
#include "RamMemory.hpp"

using namespace std;

class ControlUnit {
    private:
        RegistrarsBank bankOfRegistrars;
        vector<string> splitLineOfCodeBySpace(string input);
    public:
        ControlUnit(int registrarsAmount);
        RegistrarsBank* getBankOfRegistrars();
        void executeProcess(const vector<string>&  instructions);
        void ULA(MemoryPage *processBlock, RamMemory *ram);
};

#endif
