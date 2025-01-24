#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <vector>
#include <string>
#include <sstream>

#include "RegistrarsBank.hpp"
#include "RamMemory.hpp"
#include "Cache.hpp"

using namespace std;

class ControlUnit {
    private:
        RegistrarsBank bankOfRegistrars;
        vector<string> splitLineOfCodeBySpace(string input);
    public:
        ControlUnit(int registrarsAmount);
        RegistrarsBank* getBankOfRegistrars();
        void ULA(MemoryPage *processBlock, RamMemory *ram, Cache *cache);
        void cacheHIT(MemoryPage *processBlock, Cache *cache, int address);
};

#endif
