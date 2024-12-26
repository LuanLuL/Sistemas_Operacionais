#ifndef REGISTRARS_BANK_HPP
#define REGISTRARS_BANK_HPP

#include <iostream>
#include <vector>

using namespace std;

typedef struct Registrar {
    int value;
    bool dirty;
} Registrar;

class RegistrarsBank {
    private:
        vector<Registrar> registrars;
        int size;
        int Pc = 0;

 public:
    RegistrarsBank(int size);
    ~RegistrarsBank();
    Registrar createRegistrar(int value, bool dirty);
    vector<Registrar> getRegistrars();
    int getSize();
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