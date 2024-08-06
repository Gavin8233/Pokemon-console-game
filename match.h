#ifndef MATCH_H
#define MATCH_H

#include "playerHand.h"
#include "AI.h"


class match {

public:

    bool devMode = false;
    void ifDevMode(bool& dvM);



    playerHand player1;
    playerHand player2;

    match(playerHand p1, playerHand p2) :

        player1{ p1 },

        player2{ p2 }

    {}

    void phases(playerHand& currPlayer, playerHand& otherPlayer);

    void pokemon();

};



#endif