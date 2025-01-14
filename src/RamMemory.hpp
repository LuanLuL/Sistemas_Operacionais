#ifndef RAM_MEMORY_HPP
#define RAM_MEMORY_HPP

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stdexcept>
#include <random>

using namespace std;

typedef struct MemoryPage {
    int id;
    int processCount;
    int numberClocksEstimated;
    int priority;
    vector<string> process;
    string inputOutput;
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
        queue<MemoryPage> processQueue;
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
        bool hasProcesses();
        int getNumberOfProcesses();
};

#endif
