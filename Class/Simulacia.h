//
// Created by marci on 1/3/2024.
//

#ifndef SIMOHENKLIENT_SIMULACIA_H
#define SIMOHENKLIENT_SIMULACIA_H
#include "Biotop.h"
#include <random>
#include <iostream>


enum VietorEnum {
    SEVER = 0,
    VYCHOD = 1,
    JUH = 2,
    ZAPAD = 3,
    BEZVETRIE = 4
};

class Simulacia {
private:
    int sizeX, sizeY;
    Biotop** biotop;
    VietorEnum vietor;
    unsigned int pocetSimulacii = 0;
    unsigned int zaciatokVetra = 0;
public:
    Simulacia(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
        biotop = new Biotop*[sizeX];
        for(int i = 0; i < sizeX; i++) {
            biotop[i] = new Biotop[sizeY];
        }
        this->vietor = BEZVETRIE;
    };
    ~Simulacia() {
        // Deallocate memory for the 2D array
        for (int i = 0; i < sizeX; ++i) {
            delete[] biotop[i];
        }
        delete[] biotop;
    }

    bool positionIsValid(int x, int y) {
        return x >= 0 && x < sizeX && y >= 0 && y < sizeY;
    }

    void init(int percentoLes, int percentoLuka, int percentoSkala, int percentoVoda) {
        if (percentoVoda + percentoSkala + percentoLuka + percentoLes != 100) {
            std::cout << "Zla percentualna hodnota";
            return;
        }
        if (percentoVoda < 0 || percentoSkala < 0 || percentoLuka < 0 || percentoLes < 0) {
            std::cout << "Zla percentualna hodnota";
            return;
        }

        for(int i = 0; i < sizeX; i++) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 100);
            int cislo = dis(gen);
            int cislo2 = dis(gen);
            //mapa init random hodnoty
            for(int j = 0; j < sizeY; j++) {
                biotop[i][j].setPosX(i);
                biotop[i][j].setPosY(j);
                biotop[i][j].setZhorena(false);
                if (dis(gen) <= percentoLes) {
                    biotop[i][j].setStav(LES);
                } else if (dis(gen) <= percentoLuka) {
                    biotop[i][j].setStav(LUKA);
                } else if (dis(gen) <= percentoSkala) {
                    biotop[i][j].setStav(SKALA);
                } else {
                    biotop[i][j].setStav(VODA);
                }
            }

            //vietor init
            if (cislo2 <= 90) {
                this->vietor = BEZVETRIE;
            } else {
                int smerVetra = dis(gen);
                if (smerVetra <= 25) {
                    this->vietor = SEVER;
                } else if (smerVetra <= 50) {
                    this->vietor = VYCHOD;
                } else if (smerVetra <= 75) {
                    this->vietor = JUH;
                } else {
                    this->vietor = ZAPAD;
                }
                this->zaciatokVetra = 1;
            }
        }
    }

    //zatial takto neskor treba producent a konzument
    void print() {
        std::cout << "Smer vetra: " << this->vietor << std::endl;
        for(int i = 0; i < sizeX; i++) {
            for(int j = 0; j < sizeY; j++) {
                std::cout << biotop[i][j].getStav() << " ";
            }
            std::cout << std::endl;
        }
    }

};


#endif //SIMOHENKLIENT_SIMULACIA_H
