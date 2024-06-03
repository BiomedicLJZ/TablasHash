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
        } else if (method == COALESCED) {
            next.resize(tableSize, -1);
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
        int index = hashFunction(value);
        switch (collisionMethod) {
            case SEPARATE_CHAINING:
                chains[index].remove(value);
                itemCount--;
                break;
            case LINEAR_PROBING:
            case QUADRATIC_PROBING:
            case DOUBLE_HASHING:
            case ROBIN_HOOD:
            case COALESCED:
                while (occupied[index]) {
                    if (table[index] == value) {
                        table[index] = -1;
                        occupied[index] = false;
                        itemCount--;
                        return;
                    }
                    index = nextIndex(index, value);
                }
                break;
        }
    }

    int get(int value) {
        int index = hashFunction(value);
        switch (collisionMethod) {
            case SEPARATE_CHAINING:
                for (const int& val : chains[index]) {
                    if (val == value) {
                        return val;
                    }
                }
                break;
            case LINEAR_PROBING:
            case QUADRATIC_PROBING:
            case DOUBLE_HASHING:
            case ROBIN_HOOD:
            case COALESCED:
                while (occupied[index]) {
                    if (table[index] == value) {
                        return table[index];
                    }
                    index = nextIndex(index, value);
                }
                break;
        }
        return -1; // Return -1 if value is not found
    }

    void displayHash() const {
        for (int i = 0; i < tableSize; i++) {
            std::cout << i << " --> ";
            if (collisionMethod == SEPARATE_CHAINING) {
                if (!chains[i].empty()) {
                    for (const auto& val : chains[i]) {
                        std::cout << val << " ";
                    }
                }
            } else {
                if (occupied[i]) {
                    std::cout << table[i];
                }
            }
            std::cout << std::endl;
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
        std::vector<int> oldTable = table;
        std::vector<bool> oldOccupied = occupied;
        std::vector<std::list<int>> oldChains = chains; // Store the old chains
        tableSize *= 2; // Double the table size
        table = std::vector<int>(tableSize, -1);
        occupied = std::vector<bool>(tableSize, false);
        if (collisionMethod == SEPARATE_CHAINING) {
            chains = std::vector<std::list<int>>(tableSize);
        } else if (collisionMethod == COALESCED) {
            next = std::vector<int>(tableSize, -1);
        }
        itemCount = 0; // Reset itemCount
        for (int i = 0; i < oldTable.size(); i++) {
            if (oldOccupied[i]) {
                insertItem(oldTable[i]);
            }
        }
        // Rehash the elements from the old chains
        if (collisionMethod == SEPARATE_CHAINING) {
            for (int i = 0; i < oldChains.size(); i++) {
                for (const auto& value : oldChains[i]) {
                    insertItem(value);
                }
            }
        }
    }

    int nextIndex(int currentIndex, int value) const {
        switch (collisionMethod) {
            case LINEAR_PROBING:
                return (currentIndex + 1) % tableSize;
            case QUADRATIC_PROBING: {
                static int i = 1;
                int index = (currentIndex + i * i) % tableSize;
                i++;
                return index;
            }
            case DOUBLE_HASHING:
                return (currentIndex + secondHashFunction(value)) % tableSize;
            case ROBIN_HOOD:
                return (currentIndex + 1) % tableSize;
            case COALESCED:
                return next[currentIndex];
            default:
                return -1;
        }
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
