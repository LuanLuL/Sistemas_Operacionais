#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

typedef struct CacheCell {
    string instruction;
    int result;
    int amountTimesUsed = 0;
} CacheCell;


class Cache {
    private:
        vector<CacheCell> cache; // Armazena as instruções
        int capacity; // Capacidade máxima da cache
        int findReplaceIndex();
    public:
        Cache(int capacity);
        void save(string instruction, int result);
        void displayCache();
        int* isSimilar(string comand);
};

#endif