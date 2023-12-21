#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <vector>

#include "Block.h"

//#define DEBUG

// CLASSE BLOCKMANAGER
// Utile per la gestione dei nodi dell'albero sintattico -> semantico
class BlockManager {
public:
    // Costruttore di default
    BlockManager() = default;

    // Distruttore manuale che de-alloca iterativamente ciascun blocco
    ~BlockManager() {
        auto i = allocated.begin();
        for (; i != allocated.end(); i++) {
            delete(*i);
        }
    }

    // Costruttore di copia e Operatore di assegnamento sono inaccessibili
    // per evitare problemi e conflitti con la memoria
    BlockManager(const BlockManager& other) = delete;
    BlockManager& operator= (const BlockManager& other) = delete;

    // Metodo make Block che esegue la push nello stack
    Block* makeBlock(Block b) {
        #ifdef DEBUG
        std::cout << "Creo Block" << std::endl;
        #endif // DEBUG

        Block* bl = new Block(b);
        allocated.push_back(bl);
        return bl;
    }

private:
    std::vector<Block*> allocated;
};

#endif