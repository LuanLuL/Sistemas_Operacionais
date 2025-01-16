#ifndef CENTRAL_PROCESSES_UNIT_HPP
#define CENTRAL_PROCESSES_UNIT_HPP

#include <queue>
#include <vector>
#include <string>
#include <stdexcept>

#include "ControlUnit.hpp"
#include "RegistrarsBank.hpp"
#include "RamMemory.hpp"

#define TIME_QUANTUM 35 // TIME_QUANTUM = 35 CLOCKS

using namespace std;

class CentralProcessesUnit {
    private:
        bool busy;
        int clocks;
        ControlUnit controlUnit;
    public:
        CentralProcessesUnit(int registrarsAmount);
        int getClocks();
        void setClocks(int newClocks);
        bool getBusy();
        void setBusy(bool newBusy);
        int execute(MemoryPage *processPage, RamMemory *ram, bool isPreempitivo);
        void swapProgram(MemoryPage *processPage, RamMemory *ram, int typeOfSwap);
};

#endif
