#include <iostream>
#include "Class/Simulacia.h"

int main() {
    Simulacia* simulacia = new Simulacia(10, 10);
    simulacia->init(25, 25, 25, 25);
    simulacia->print();
}
