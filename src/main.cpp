// "./dataset/codigo.txt"

#include <iostream>
#include <locale>

#include "RamMemory.hpp"

using namespace std;

int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RamMemory ram(32);
    cout << "\n"<< ram.read(2) <<"\n\n";
    return 0;
}