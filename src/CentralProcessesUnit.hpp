#ifndef CENTRAL_PROCESSES_UNIT_HPP
#define CENTRAL_PROCESSES_UNIT_HPP

#include <queue>
#include <vector>
#include <string>
#include <stdexcept>

#include "ControlUnit.hpp"

using namespace std;

class CentralProcessesUnit {
    private:
        queue<ControlUnit> processQueue;
    public:
        void addProcess(ControlUnit process);
        ControlUnit getNextProcess();
        bool hasProcesses();
};

#endif
