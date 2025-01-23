#include "Cache.hpp"

Cache::Cache(int capacity) {
    this->capacity = capacity;
}

int* Cache::isSimilar(string comand) {
    for(int i=0; i < this->cache.size(); i++){
        if(this->cache[i].instruction == comand){ // Simplesmente compara se existe alguma instrução similar
            this->cache[i].amountTimesUsed++;
            return &this->cache[i].result;
        }
    }
    return NULL;
}

int Cache::findReplaceIndex() {
    int theLeastUsed = INT_MAX;
    for(int i=0;i<this->cache.size();i++){
        if(cache[i].amountTimesUsed < theLeastUsed){
            theLeastUsed = i;
        }
    }
    return theLeastUsed;
}

void Cache::save(string instructionCode, int resultCode) {
    CacheCell newCell; // cria nova célula no padrão da Mémoria Cache
    newCell.instruction = instructionCode;
    newCell.result = resultCode;
    if(this->cache.size() < this->capacity){ // se a cache não estiver cheia
        this->cache.push_back(newCell); // salva normalmente
        return;
    } // caso contrario subescreve
    cache[findReplaceIndex()] = newCell;
}

void Cache::displayCache() {
    if(this->cache.size() == 0){
        cout << "\n\nMemória cache esta vazia!\n\n";
        return;
    }
    cout << "\n\nConteúdo da memória cache:\n\n";
    for (CacheCell item : cache) {
        cout << "["<< item.instruction << ", " << item.result <<  ", " << item.amountTimesUsed << "]\n";
    }
    cout << endl;
}