#ifndef INPUTSOUTPUTS_HPP
#define INPUTSOUTPUTS_HPP

#include <unordered_map>
#include <string>
#include <stdexcept>

using namespace std;

class InputsOutputs {
private:
    unordered_map<string, pair<int, bool>> table;

public:
    InputsOutputs();
    void add(string key, pair<int, bool> occupied);
    void setOcccupied(string key, pair<int, bool> occupied);
    void setUnoccupied(string key);
    bool isOccupied(string key) const;
    void remove(string key);
    bool exists(string key) const;
};

#endif

