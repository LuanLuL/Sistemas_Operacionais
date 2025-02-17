#include "Mmu.hpp"

string Mmu::intToBinary(int value) {
    return bitset<32>(value).to_string();
}

int Mmu::binaryToInt(string binaryStr) {
    return stoi(binaryStr, nullptr, 2);
}

void Mmu::addAddress(int value) {
    addressQueue.push(intToBinary(value));
}

int Mmu::getNextAddress(){
    if (addressQueue.empty()){
        throw runtime_error("Mmu::getNextAddress(There\'s no address in the queue):");
    }
    string binaryStr = addressQueue.front();
    addressQueue.pop();
    return binaryToInt(binaryStr);
}

bool Mmu::hasAddresses() const {
    return !addressQueue.empty();
}
