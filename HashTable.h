#ifndef TABLASHASH_HASHTABLE_H
#define TABLASHASH_HASHTABLE_H

#include <vector>
#include <iostream>
#include <list>
#include <cmath>
#include <functional>

enum CollisionHandling {
    SEPARATE_CHAINING,
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING,
    ROBIN_HOOD,
    COALESCED
};

class HashTable {
public:
    explicit HashTable(int size, CollisionHandling method)
            : tableSize(size), itemCount(0), collisionMethod(method) {
        table.resize(tableSize, -1);
        occupied.resize(tableSize, false);
        if (method == SEPARATE_CHAINING) {
            chains.resize(tableSize);
        }
    }

    int hashFunction(int value) const {
        return value % tableSize;
    }

    int secondHashFunction(int value) const {
        return 7 - (value % 7); // Second hash function for double hashing
    }

    void insertItem(int value) {
        if (itemCount / static_cast<double>(tableSize) >= 0.7) {
            rehash();
        }
        switch (collisionMethod) {
            case SEPARATE_CHAINING:
                insertSeparateChaining(value);
                break;
            case LINEAR_PROBING:
                insertLinearProbing(value);
                break;
            case QUADRATIC_PROBING:
                insertQuadraticProbing(value);
                break;
            case DOUBLE_HASHING:
                insertDoubleHashing(value);
                break;
            case ROBIN_HOOD:
                insertRobinHood(value);
                break;
            case COALESCED:
                insertCoalesced(value);
                break;
        }
    }

    void deleteItem(int value) {
        // Implement deletion for different strategies
    }

    int get(int value) {
        // Implement get for different strategies
    }

    void displayHash() const {
        for (int i = 0; i < tableSize; i++) {
            if (occupied[i]) {
                std::cout << i << " --> " << table[i] << std::endl;
            }
            if (collisionMethod == SEPARATE_CHAINING) {
                for (const int& val : chains[i]) {
                    std::cout << i << " --> " << val << std::endl;
                }
            }
        }
    }

private:
    int tableSize;
    int itemCount;
    CollisionHandling collisionMethod;
    std::vector<int> table;
    std::vector<bool> occupied;
    std::vector<std::list<int>> chains; // For separate chaining
    std::vector<int> next; // For coalesced chaining

    void rehash() {
        // Rehash logic remains similar, but accommodate chosen collision method
    }

    // Insert functions for each collision handling method
    void insertSeparateChaining(int value) {
        int index = hashFunction(value);
        chains[index].push_back(value);
        itemCount++;
    }

    void insertLinearProbing(int value) {
        int index = hashFunction(value);
        while (occupied[index]) {
            index = (index + 1) % tableSize;
        }
        table[index] = value;
        occupied[index] = true;
        itemCount++;
    }

    void insertQuadraticProbing(int value) {
        int index = hashFunction(value);
        int i = 0;
        while (occupied[(index + i * i) % tableSize]) {
            i++;
        }
        table[(index + i * i) % tableSize] = value;
        occupied[(index + i * i) % tableSize] = true;
        itemCount++;
    }

    void insertDoubleHashing(int value) {
        int index = hashFunction(value);
        int stepSize = secondHashFunction(value);
        while (occupied[index]) {
            index = (index + stepSize) % tableSize;
        }
        table[index] = value;
        occupied[index] = true;
        itemCount++;
    }

    void insertRobinHood(int value) {
        int index = hashFunction(value);
        int dist = 0;
        while (occupied[index]) {
            int currDist = (index - hashFunction(table[index]) + tableSize) % tableSize;
            if (currDist < dist) {
                std::swap(table[index], value);
                dist = currDist;
            }
            index = (index + 1) % tableSize;
            dist++;
        }
        table[index] = value;
        occupied[index] = true;
        itemCount++;
    }


    void insertCoalesced(int value) {
        int index = hashFunction(value);
        int lastIndex = -1;
        while (occupied[index]) {
            lastIndex = index;
            index = (index + 1) % tableSize;
        }
        table[index] = value;
        occupied[index] = true;
        itemCount++;
        if (lastIndex != -1) {
            next[lastIndex] = index;
        }
    }

};

#endif // TABLASHASH_HASHTABLE_H
