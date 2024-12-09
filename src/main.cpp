// "./dataset/codigo.txt"

#include <iostream>
#include <locale>

#include "RamMemory.hpp"
#include "RegisterBank.hpp"

using namespace std;

int main() {
    setlocale(LC_CTYPE, "Portuguese");
    RegisterBank registers(32);
    cout << "\n";
    registers.print();
    cout << "\n\n";
    return 0;
}