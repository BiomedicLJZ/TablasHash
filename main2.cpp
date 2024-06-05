#include "HashTable.h"
#include <iostream>

int main() {
    HashTable table(10,QUADRATIC_PROBING);
    table.insertItem(89);
    table.insertItem(18);
    table.insertItem(49);
    table.insertItem(58);
    table.insertItem(69);
    table.displayHash();
    return 0;
}
