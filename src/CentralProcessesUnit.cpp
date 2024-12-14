#include "CentralProcessesUnit.hpp"

void CentralProcessesUnit::addProcess(ControlUnit process) {
    this->processQueue.push(process);
}

ControlUnit CentralProcessesUnit::getNextProcess() {
    if (this->processQueue.empty()) {
        throw runtime_error("CentralProcessesUnit::getNextProcess(No processes in the queue)");
    }
    ControlUnit nextProcess = this->processQueue.front();
    this->processQueue.pop();
    return nextProcess;
}

bool CentralProcessesUnit::hasProcesses() {
    return !this->processQueue.empty();
}
