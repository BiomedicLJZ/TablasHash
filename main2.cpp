#include "HashTable.h"
#include <iostream>

int main() {
    HashTable table(5, SEPARATE_CHAINING); // Decrease the initial size of the hash table
    table.insertItem(1);
    table.insertItem(2);
    table.insertItem(3);
    table.insertItem(4);
    table.insertItem(5);
    table.insertItem(15);
    table.insertItem(25);
    table.insertItem(35);
    table.insertItem(45);
    table.insertItem(54);
    table.insertItem(22);
    table.displayHash();
    return 0;
}
