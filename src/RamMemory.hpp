#ifndef RAM_MEMORY_HPP
#define RAM_MEMORY_HPP

#include <iostream>
#include <vector>
#include <string>

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
