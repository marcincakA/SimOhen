#include <iostream>
#include "Class/Simulacia.h"

int main() {
    Simulacia simulacia(2, 2);
    simulacia.init(50, 40, 5, 5);
    simulacia.print();
    for (int i = 0; i < 2; ++i) {
        simulacia.setFlame(0, i);
    }
    simulacia.print();
    for(int i = 0; i < 15; ++i) {
        simulacia.step();
        simulacia.print();
    }
}
