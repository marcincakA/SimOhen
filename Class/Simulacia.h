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
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    Simulacia(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
        biotop = new Biotop*[sizeX];
        for(int i = 0; i < sizeX; i++) {
            biotop[i] = new Biotop[sizeY];
        }
        this->vietor = BEZVETRIE;
        this->gen = std::mt19937(rd());
        this->dis = std::uniform_int_distribution<>(0, 100);
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

    void setFlame(int x, int y) {
        if (positionIsValid(x, y)) {
            if (!biotop[x][y].isFlamable()) {
                std::cout << "Bunka nie je horlava" << std::endl;
                return;
            }
            biotop[x][y].setStav(POZIAR);
        }
    }

    void windStep() {
        if(this->vietor != BEZVETRIE) {
            //vietor drzi svoj smer na tri tahy
            if (this->zaciatokVetra == this->pocetSimulacii - 3) {
                this->vietor = BEZVETRIE;
                return;
            }

            if(this->vietor == BEZVETRIE) {

                int cislo = dis(gen);
                if (cislo <= 90) {
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
                    this->zaciatokVetra = this->pocetSimulacii;
                }
            }


        }
    }

    void setFireArround(int x, int y) { // x je vyska y je sirka
        //metodka co zoberie stav vetra a bunku na zaklade vetra zapali ostatne bunky
        if (!positionIsValid(x, y)) {
            std::cout << "Zadana pozicia nie je validna" << std::endl;
            return;
        }
        if (!biotop[x][y].getStav() == POZIAR) {
            std::cout << "Bunka nehori" << std::endl;
            return;
        }

        //*******************BEZVETRIE*******************//
        if (this->vietor == BEZVETRIE) {
            if (positionIsValid(x - 1, y)) {
                if (biotop[x - 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x + 1, y)) {
                if (biotop[x + 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x + 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y - 1)) {
                if (biotop[x][y - 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x][y - 1].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y + 1)) {
                if (biotop[x][y + 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
        }
        /************************SEVER**********************/
        else if(this->vietor == SEVER) {
            if (positionIsValid(x - 1, y)) {
                if (biotop[x - 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 90) { // 90 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x + 1, y)) {
                if (biotop[x + 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 2) { // 2 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y - 1)) {
                if (biotop[x][y - 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar ostava ako pri bezvetri
                        biotop[x][y - 1].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y + 1)) {
                if (biotop[x][y + 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar ostava ako pri bezvetri
                        biotop[x][y + 1].setStav(POZIAR);
                    }
                }
            }
        /************************VYCHOD**********************/
        } else if(this->vietor == VYCHOD) {
            if (positionIsValid(x - 1, y)) {
                if (biotop[x - 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x + 1, y)) {
                if (biotop[x + 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 2 percentna sanca na poziar
                        biotop[x + 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y - 1)) {
                if (biotop[x][y - 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 2) { // 2 percentna sanca na poziar
                        biotop[x][y - 1].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y + 1)) {
                if (biotop[x][y + 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 90) { // 90 percentna sanca na poziar
                        biotop[x][y + 1].setStav(POZIAR);
                    }
                }
            }
        /************************JUH**********************/
        } else if(this->vietor == JUH) {
            if (positionIsValid(x - 1, y)) {
                if (biotop[x - 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 2) { // 2 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x + 1, y)) {
                if (biotop[x + 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 90) { // 90 percentna sanca na poziar
                        biotop[x + 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y - 1)) {
                if (biotop[x][y - 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 2 percentna sanca na poziar
                        biotop[x][y - 1].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y + 1)) {
                if (biotop[x][y + 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 90 percentna sanca na poziar
                        biotop[x][y + 1].setStav(POZIAR);
                    }
                }
            }
        /************************ZAPAD**********************/
        } else if(this->vietor == ZAPAD) {
            if (positionIsValid(x - 1, y)) {
                if (biotop[x - 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 20 percentna sanca na poziar
                        biotop[x - 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x + 1, y)) {
                if (biotop[x + 1][y].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 20) { // 2 percentna sanca na poziar
                        biotop[x + 1][y].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y - 1)) {
                if (biotop[x][y - 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 90) { // 90 percentna sanca na poziar
                        biotop[x][y - 1].setStav(POZIAR);
                    }
                }
            }
            if (positionIsValid(x, y + 1)) {
                if (biotop[x][y + 1].isFlamable()) {
                    int pravdepodobnostPoziaru = dis(gen);
                    if (pravdepodobnostPoziaru <= 2) { // 2 percentna sanca na poziar
                        biotop[x][y + 1].setStav(POZIAR);
                    }
                }
            }
        }
    }

    Biotop* findBiotopNear(int x, int y,BiotopEnum biotopEnum) {
        if (!positionIsValid(x, y)) {
            std::cout << "Zadana pozicia nie je validna" << std::endl;
            return nullptr;
        }
        //pozri okolo
        if (positionIsValid(x - 1, y)) {
            if (biotop[x - 1][y].getStav() == biotopEnum) {
                return &biotop[x - 1][y];
            }
        }
        if (positionIsValid(x + 1, y)) {
            if (biotop[x + 1][y].getStav() == biotopEnum) {
                return &biotop[x + 1][y];
            }
        }
        if (positionIsValid(x, y - 1)) {
            if (biotop[x][y - 1].getStav() == biotopEnum) {
                return &biotop[x][y - 1];
            }
        }
        if (positionIsValid(x, y + 1)) {
            if (biotop[x][y + 1].getStav() == biotopEnum) {
                return &biotop[x][y + 1];
            }
        }
        return nullptr;
    }

    void regenerateBiotop(int x, int y) {
        if (!positionIsValid(x,y)) {
            std::cout << "Zadana pozicia nie je validna" << std::endl;
            return;
        }
        int pravdepodobnost = dis(gen);
        if (this->biotop[x][y].getStav() == ZHORENA) {
            Biotop* foundBiotop = findBiotopNear(x, y, VODA);
            if (foundBiotop != nullptr) {
                if (pravdepodobnost <= 10) {
                    this->biotop[x][y].setStav(LUKA);
                }
            }
        }
        if (this->biotop[x][y].getStav()==LUKA) {
            Biotop* foundBiotop = findBiotopNear(x, y, LES);
            if (foundBiotop != nullptr) {
                if (pravdepodobnost <= 2) {
                    this->biotop[x][y].setStav(LES);
                }
            }
        }
    }


    void step() {
        windStep(); // chod vetra
        //find fire
        for(int i = 0; i < sizeX; i++) {
            for(int j = 0; j < sizeY; j++) {
                if (biotop[i][j].getStav() == POZIAR) {
                    setFireArround(i, j);
                    biotop[i][j].setZaciatokHorenia(this->pocetSimulacii);
                    if (this->biotop[i][j].getZaciatokHorenia() + 5 <= this->pocetSimulacii) {
                        biotop[i][j].setStav(ZHORENA);
                    }
                }
                regenerateBiotop(i, j);
            }
        }
    }

};


#endif //SIMOHENKLIENT_SIMULACIA_H
