#include "CentralProcessesUnit.hpp"

CentralProcessesUnit::CentralProcessesUnit(int registrarsAmount) : controlUnit(registrarsAmount), clocks(0), busy(false) {}

int CentralProcessesUnit::getClocks() {
    return this->clocks;
}

void CentralProcessesUnit::setClocks(int newClocks) {
    this->clocks = newClocks;
}

bool CentralProcessesUnit::getBusy() {
    return this->busy;
}

void CentralProcessesUnit::setBusy(bool newBusy) {
    this->busy = newBusy;
}

int CentralProcessesUnit::execute(MemoryPage *processPage, RamMemory *ram, Cache *cache, bool isPreempitivo) {
    int timeQuantum = 0;
    this->swapProgram(processPage, ram, 2);
    while(this->controlUnit.getBankOfRegistrars()->getPc() <= processPage->process.size()){
        if(timeQuantum == TIME_QUANTUM && isPreempitivo){
            this->swapProgram(processPage, ram, 1);
            return -1;
        }
        int* isPorcessSimilar = cache->isSimilar(processPage->process[this->controlUnit.getBankOfRegistrars()->getPc()-1]);
        if(isPorcessSimilar != NULL){
            this->controlUnit.cacheHIT(processPage, cache, (*isPorcessSimilar));
        } else{
            controlUnit.ULA(processPage, ram, cache);
            processPage->numberClocksEstimated--; // dimnui um de clock estimado a cada execução
        }
        this->clocks++;
        timeQuantum++;
        
    }
    this->swapProgram(processPage, ram, 0);
    return 0;
}

void CentralProcessesUnit::swapProgram(MemoryPage *processPage, RamMemory *ram, int typeOfSwap) {
    RegistrarsBank *oldBank  = this->controlUnit.getBankOfRegistrars();
    vector<MemoryCell> pastMemoryBank = ram->getMemoryCells();
    switch (typeOfSwap){
        case 0: //processo acabou
            for(int i = 0; i < oldBank->getSize(); i++){ //limpa todos os registradores relacioandos com o processo antigo
                if((*oldBank).getRegistrars()[i].dirty){ // sujo implica que estava sendo usado no processo antigo
                    oldBank->setClean(i); // indica que está liberado e não precisa fazer swap deste registrador
                }
            }
            for(int i = 0; i < ram->getSize(); i++){ //limpa todos as celulas de memoria relacioandas com o processo antigo
                if(pastMemoryBank[i].idProcess == processPage->id){
                    ram->write(ram->createMemoryCell(0, 0, 0, i), 1) ;
                }
            }
            this->busy = false; // desocupa a cpu
            break;
        case 1: // processo não acabou mas precisa trocar porque estorou o tempo de quantum que ele tem para executar
            processPage->processCount = oldBank->getPc();
            for(int i = 0; i < oldBank->getSize(); i++){ //salva todos os registradores relacioandos com o processo antigo na memória Ram
                Registrar pastRegistrar = (*oldBank).getRegistrars()[i];
                if(pastRegistrar.dirty || pastRegistrar.value != 0){ // sujo implica que estava sendo usado no processo antigo
                    ram->write(ram->createMemoryCell(processPage->id, i, pastRegistrar.value, -1), 2);
                    oldBank->setClean(i); // feito o swap o resgistrador tem o status de limpo
                }
            }
            this->busy = false; // desocupa a cpu
            break;
        case 2: // novo processo entrando para a execução
            this->busy = true; // ocupa a cpu
            this->controlUnit.getBankOfRegistrars()->setPc(processPage->processCount); //atualiza o Pc de acordo com o novo processo
            for(int i = 0; i < ram->getSize(); i++){ //busca os valores do novo processo na ram e passa para os registradores
                if(pastMemoryBank[i].idProcess == processPage->id){
                    oldBank->setValue(pastMemoryBank[i].registrarAddress, pastMemoryBank[i].registrarValue);
                    ram->clear(i);
                }
            }
            break;
        default:
            throw invalid_argument("CentralProcessesUnit::swapProgram(Invalid type of swap tried to run)");
            break;
    }
}