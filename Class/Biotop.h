//
// Created by marci on 1/3/2024.
//

#ifndef SIMOHENKLIENT_BIOTOP_H
#define SIMOHENKLIENT_BIOTOP_H

enum BiotopEnum {
    LES = 0,
    LUKA = 1,
    SKALA = 2,
    VODA = 3
};


class Biotop {
private:
    int posX, posY;
    BiotopEnum stav;
public:
    Biotop(int posX,int posY,int stav);
};


#endif //SIMOHENKLIENT_BIOTOP_H
