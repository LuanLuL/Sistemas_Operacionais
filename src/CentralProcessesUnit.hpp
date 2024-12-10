#ifndef CENTRAL_PROCESSES_UNIT_HPP
#define CENTRAL_PROCESSES_UNIT_HPP

#include <queue>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class CentralProcessesUnit {
    private:
        queue<vector<string>> processQueue;
    public:
        void addProcess(const vector<string>& process);
        vector<string> getNextProcess();
        bool hasProcesses();
};

#endif
