#include "HashTable.h"
#include <iostream>


int main(int argc, char **argv) {
    auto *tabla = new HashTable(10, SEPARATE_CHAINING);
    tabla->insertItem(1);
    tabla->insertItem(2);
    tabla->insertItem(3);
    tabla->insertItem(4);
    tabla->insertItem(5);
    tabla->insertItem(15);
    tabla->insertItem(25);
    tabla->insertItem(35);
    tabla->insertItem(45);
    tabla->insertItem(54);
    tabla->insertItem(22);
    tabla->displayHash();
    return 0;
}
