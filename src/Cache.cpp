#include "Cache.hpp"

Cache::Cache(int capacity) : capacity(capacity) {
    cache.resize(capacity, "");
}

bool Cache::isSimilar(const string& a, const string& b) {
    // Simplesmente compara o tamanho das strings ou similaridade por hash
    if (a.empty() || b.empty()) return false;
    return a.substr(0, 3) == b.substr(0, 3); // Exemplo simples de similaridade
}

int Cache::findReplaceIndex() {
    random_device seeder;
    const auto seed(seeder.entropy() ? seeder() : time(nullptr));
    mt19937 randomEngine{static_cast<mt19937::result_type>(seed)};
    uniform_int_distribution<int> uniformDistribution{0, this->capacity-1};
    auto generator{bind(uniformDistribution, randomEngine)};
    vector<int> values(1);
    generate(begin(values), end(values), generator);
    cout << "\n\n" << values[0] << " " << values[1] << "\n\n";
    return values[0];
}

void Cache::save(const string& instruction) {
    for (size_t i = 0; i < cache.size(); ++i) {
        if (cache[i].empty()) { // Se há espaço vazio na cache
            cache[i] = instruction; // Salva instrução
            return;
        }
    }
    
    cache[findReplaceIndex()] = instruction; // caso não tenha espaços vazios -> sobrescreve
}

bool Cache::containsSimilar(const string& instruction) {
    for (const auto& item : cache) {
        if (isSimilar(instruction, item)) {
            return true;
        }
    }
    return false;
}

void Cache::displayCache() {
    cout << "Conteúdo da cache:\n";
    for (const auto& item : cache) {
        cout << (item.empty() ? "[Vazio]" : item) << "\n";
    }
}