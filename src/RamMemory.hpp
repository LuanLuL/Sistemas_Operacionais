#ifndef RAM_MEMORY_HPP
#define RAM_MEMORY_HPP

#include "Mmu.hpp"

#include <vector>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef struct MemoryPage {
    int id;
    int processCount;
    int numberClocksEstimated;
    int priority;
    vector<string> process;
    string inputOutput;
    int allocationBegins;
    int allocationEnds;
} MemoryPage;

typedef struct MemoryCell {
    int idProcess;
    int registrarAddress;
    int registrarValue;
    int ramAddress;
} MemoryCell;

class RamMemory {
    private: 
        vector<MemoryCell> memoryCells;
        unordered_map<int, MemoryPage> listOfProcess;
        Mmu virtualAddresses;
        int size;
    public:
        RamMemory(int size);
        ~RamMemory();
        vector<MemoryCell> getMemoryCells();
        MemoryCell createMemoryCell(int newIdProcess, int newRegistrarAddress, int newRegistrarValue, int newRamAddress);
        void write(MemoryCell newMemoryCell, int kindOfWrite);
        MemoryCell read(int address);
        void clear(int address);
        int getSize();
        void addProcess(MemoryPage process);
        MemoryPage getNextProcess();
        MemoryPage getProcessWithLeastClocks();
        MemoryPage getProcessByPriority();
        MemoryPage getProcessBySimilarity(int lastProcessRuned);
        bool hasProcesses();
        int getNumberOfProcesses();
};

#endif
