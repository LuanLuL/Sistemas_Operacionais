#ifndef RAM_MEMORY_HPP
#define RAM_MEMORY_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class RamMemory {
    private: 
        int* memoryCells;
        vector<vector<string>> memoryProcesses;
        int size;
    public:
        RamMemory(int size, vector<vector<string>> initialProcesses);
        ~RamMemory();
        void write(int value, int address);
        int read(int address);
        vector<string> search(int addressProcess);
        int getSize();
};

#endif
