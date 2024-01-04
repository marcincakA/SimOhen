#include <iostream>
#include "Class/Simulacia.h"

int main() {
    Simulacia simulacia(5, 5);
    simulacia.init(50, 30, 10, 10);
    simulacia.print();
    for (int i = 0; i < 5; ++i) {
        simulacia.setFlame(0, i);
    }
    simulacia.print();
    for(int i = 0; i < 15; ++i) {
        simulacia.step();
        simulacia.print();
    }
}
