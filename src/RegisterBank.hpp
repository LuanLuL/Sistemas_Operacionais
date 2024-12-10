#ifndef REGISTER_BANK_HPP
#define REGISTER_BANK_HPP

#include <iostream>
#include <vector>

using namespace std;

typedef struct Register {
    int value;
    bool dirty;
} Register;

class RegisterBank {
    private:
        vector<Register> registers;
        int size;
        int Pc = 0;

 public:
    RegisterBank(int size);
    ~RegisterBank();
    Register createRegister(int value, bool dirty);
    int getValue(int address);
    void setValue(int address, int value);
    bool getStatus(int address);
    void setDirty(int address);
    void setClean(int address);
    int getPc();
    void setPc(int numberOfProgram);
    void print();
};

#endif