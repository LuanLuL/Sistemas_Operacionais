#ifndef RAMMEMORY_HPP
#define RAMMEMORY_HPP

#include <iostream>
#include <vector>

using namespace std;

class RamMemory {
    private: 
        int* memoryCells;
        vector<string> memoryProcesses;
        int size;
    public:
        RamMemory(int size, vector<string> initialProcesses);
        ~RamMemory();
        void write(int value, int address);
        int read(int address);
        string search(int address);
        int getSize();
};

#endif
