#include "match.h"

static void devModeEmplaceTypes(regCard& activeCard, regCard& card, const std::string t1, const std::string t2, const std::string t3, const std::string t4, const std::string t5, const std::string t6, const std::string t7) {

    std::vector<std::string> appTypes{ t1, t2, t3, t4, t5, t6, t7, t1, t2, t3, t4, t5, t6, t1, t2, t3, t4, t5, t6, t7, t1, t2, t3, t4, t5, t6, t7, t1, t2, t3, t4, t5, t6, t1, t2, t3, t4, t5, t6, t7 };

    card.currEnergyTypes = appTypes;

    activeCard.currEnergyTypes = appTypes;

}

static void devModeFILLENERGY(regCard& activeCard, std::vector<regCard>& cards) {

    for (regCard& card : cards) {

        devModeEmplaceTypes(activeCard, card, "fire", "lightning", "water", "psychic", "fighting", "metal", "darkness");

    }

}

void match::ifDevMode(bool& dvM) {

    if (dvM == true) {

        std::cout << "\n\n\n=====GAME STARING IN DEV MODE=====\n\n\n\n";

        devModeFILLENERGY(player1.playerCurrHand.activeCard, player1.playerCurrHand.playerActiveBench);

        player1.devModeAddAllCards();

        devModeFILLENERGY(player2.playerCurrHand.activeCard, player2.playerCurrHand.playerActiveBench);

        player2.devModeAddAllCards();

    }

}




static bool checkWinConditions(playerHand& loserHand, playerHand& currHand) {

    if ( currHand.playerCurrHand.prizeCards.size() == 3 ) { return true; }

    if (loserHand.playerCurrHand.playerActiveBench.size() == 0) { return true; }

    return false;

}

static std::string isInflicted(regCard& currCard) {

    std::vector<bool> currInflic{ currCard.isAsleep, currCard.isBurned, currCard.isParalyzed, currCard.isPoisoned };

    short cnt = 0;

    for (bool tF : currInflic) {

        if (tF == true) {

            switch (cnt) {

            case 0:
                return "aslp";

            case 1:
                return "burn";

            case 2:
                return "paral";

            case 3:
                return "pois";

            }

        }

        cnt++;

    }

    return "0";

}

static void blankFunc(regCard& currCard) { return; }
static void ifBrn(regCard& currCard) { std::cout << "" << currCard.regCardName << " is burned!\n"; currCard.regCardHealth -= 20; currCard.damageCounter += 20; }
static void ifPois(regCard& currCard) { std::cout << "" << currCard.regCardName << " is poisoned!\n"; currCard.regCardHealth -= 20; currCard.damageCounter += 20; }
static void ifAslp(regCard& currCard) { std::cout << currCard.regCardName << " is asleep!, turn skipped! however, you can still play any item or retreat\n"; }


std::map<std::string, std::function<void(regCard&)>> cardStatusesAPPLY{

    { "0", blankFunc },

    { "burn", ifBrn },

    { "pois", ifPois },

    { "aslp", ifAslp },

    { "paral", blankFunc }

};

static void applyStatusEffects(playerHand& currHand) {

    regCard& activeCard = currHand.playerCurrHand.activeCard;

    std::string currEffectAC = isInflicted(activeCard);

    cardStatusesAPPLY[currEffectAC](activeCard);

    for (regCard& currCard : currHand.playerCurrHand.playerActiveBench) {

        std::string ifEff = isInflicted(currCard);

        auto iter = cardStatusesAPPLY.find(ifEff);

        if (iter != cardStatusesAPPLY.end()) { iter->second(currCard); }

    }

    std::cout << "\n";

}

void match::phases(playerHand& currPlayer, playerHand& otherPlayer) {

    applyStatusEffects(currPlayer);

    currPlayer.drawCard();

    while (true) {

        std::string uChoice;
        std::cout << "\nattack / item / retreat / ability: ";
        std::cin >> uChoice;


        if (uChoice.at(0) == 'i') { currPlayer.useItem(); continue; }
        if (uChoice.at(0) == 'r') { currPlayer.Retreat(); continue; }
        if (uChoice.at(0) == 'a' && uChoice.at(1) == 'b') { currPlayer.useCardAbility(); continue; }

        else { currPlayer.attackPlayer(otherPlayer); return; }

    }

}

static void ifKO(playerHand& WP, playerHand& LP) {

    regCard newPrizeCard = LP.playerCurrHand.activeCard;

    WP.playerCurrHand.prizeCards.push_back(newPrizeCard);

    std::cout << "\nOpponent card was knocked out!; Opponent! Choose new active card!\n\n";

    LP.setActiveCard();

    std::cout << "\n";

}

void match::pokemon() {


    ifDevMode(devMode);


    while (true) {

        std::cout << "\nPlayer 1 turn!\n";

        std::cout << "Active card: " << player1.playerCurrHand.activeCard.regCardName << "\n\n";

        phases(player1, player2);

        if (player2.playerCurrHand.activeCard.regCardHealth <= 0) { ifKO(player1, player2); }

        if (checkWinConditions(player2, player1) == true) { std::cout << "\n\nPLAYER 1 WINS!!!\n\n\n";   exit(0); }

        std::cout << "\n\nPlayer 2 turn!\n";

        std::cout << "Active card: " << player2.playerCurrHand.activeCard.regCardName << "\n\n";

        phases(player2, player1);

        if (player1.playerCurrHand.activeCard.regCardHealth <= 0) { ifKO(player2, player1); }

        if (checkWinConditions(player1, player2) == true) { std::cout << "\n\nPLAYER 2 WINS!!!\n\n\n";   exit(0); }

    }

}