#include <iostream>
#include "Class/Simulacia.h"

int main() {
    Simulacia* simulacia = new Simulacia(10, 10);
    simulacia->init(25, 25, 25, 25);
    simulacia->print();
    for (int i = 0; i < 10; ++i) {
        simulacia->setFlame(0, i);
    }

    simulacia->print();
}
