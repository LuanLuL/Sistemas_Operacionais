#ifndef RAM_MEMORY_HPP
#define RAM_MEMORY_HPP

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stdexcept>

using namespace std;

typedef struct MemoryPage {
    int id;
    int processCount;
    vector<string> process;
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
        int getSize();
        void addProcess(MemoryPage process);
        MemoryPage getNextProcess();
        bool hasProcesses();
        int getNumberOfProcesses();
};

#endif
