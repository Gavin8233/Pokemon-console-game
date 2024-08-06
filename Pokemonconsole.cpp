#include "match.h"
#include "characterAIMatch.h"
#include "aiVSai.h"

static void SIMULATEGAME_AI() {

    characterAI ai1;

    characterAI ai2;

    aiVSai k(ai1, ai2);

    k.matchV();

}

static void CHARACTERAI_MATCH() {

    std::cout << "\n\n-----Pokemon Battle!-----\n\n\n";

    std::cout << "Player 1! Choose one of your cards to put into active position!\n\n";

    playerHand p1(false);

    CharacterAIMatch newMatch(p1);

    newMatch.characterMatch();

}

static void POKEMON_MATCH() {

    std::cout << "\n\n-----Pokemon Battle!-----\n\n\n";

    std::cout << "Player 1! Choose one of your cards to put into active position!\n\n";

    playerHand p1(false);

    std::cout << "\nPlayer 2! Choose one of your cards to put into active position!\n\n";

    playerHand p2(false);

    match newPoke_Match(p1, p2);

    newPoke_Match.pokemon();

}


static void game (  ) {

    std::cout << "\n\n----Pokemon Console Game----\n1.) Player vs Player\n2.) Player vs AI\n3.) Simulate AI Match\n\ninput: ";
    short idx = 0;
    std::cin >> idx;

    switch (idx) {

    case 1:
        POKEMON_MATCH();

        break;

    case 2:
        CHARACTERAI_MATCH();

        break;

    case 3:
        SIMULATEGAME_AI();

        break;

    default:
        std::cout << "Not a valid option, try again\n";

    }


}


int main() {

    game();

}