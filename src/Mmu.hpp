#ifndef MMU_HPP
#define MMU_HPP

#include <queue>
#include <iostream>
#include <string>
#include <bitset>
#include <stdexcept>

using namespace std;

class Mmu{
private:
    queue<string> addressQueue;
    string intToBinary(int value);
    int binaryToInt(string binaryStr);

public:
    void addAddress(int value);
    int getNextAddress();
    bool hasAddresses() const;
};

#endif
