#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>

using namespace std;

class Cache {
    private:
        vector<string> cache; // Armazena as instruções
        int capacity; // Capacidade máxima da cache
        bool isSimilar(const string& a, const string& b);
        int findReplaceIndex();
    public:
        Cache(int capacity);
        void save(const string& instruction);
        bool containsSimilar(const string& instruction);
        void displayCache();

};

#endif