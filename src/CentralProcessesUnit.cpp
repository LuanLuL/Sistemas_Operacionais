#include "CentralProcessesUnit.hpp"

void CentralProcessesUnit::addProcess(const vector<string>& process) {
    this->processQueue.push(process);
}

vector<string> CentralProcessesUnit::getNextProcess() {
    if (this->processQueue.empty()) {
        throw runtime_error("CentralProcessesUnit::getNextProcess(No processes in the queue)");
    }
    vector<string> nextProcess = this->processQueue.front();
    this->processQueue.pop();
    return nextProcess;
}

bool CentralProcessesUnit::hasProcesses() {
    return !this->processQueue.empty();
}
