#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <vector>
#include <string>
#include <sstream>

#include "RegistrarsBank.hpp"
#include "RamMemory.hpp"

using namespace std;

class ControlUnit {
    private:
        int id;
        RegistrarsBank* bankOfRegistrars;
        int clocks;
        vector<string> splitLineOfCodeBySpace(string input);
    public:
        ControlUnit(int id, RegistrarsBank* regBank);
        int getId();
        int getClocks();
        void executeProcess(const vector<string>&  instructions);
        void ULA(vector<string> code);
};

#endif
