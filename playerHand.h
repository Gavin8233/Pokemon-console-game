#pragma once


#include "regCard.h"

class playerHand;
class activeHand;


class trainerCard {

public:

    trainerCard() {}


    std::string trainerName;
    std::string trainerDescription;
    std::function<void(activeHand&)> trainerCardEffect;


    trainerCard(const std::string tName, const std::string tDesc, std::function<void(activeHand&)> tEff) :

        trainerName{ tName },

        trainerDescription{ tDesc },

        trainerCardEffect{ tEff }

    {}

    void appTrainerEffect(activeHand& currHand) const { trainerCardEffect(currHand); }

};

struct pHand {

    std::vector<regCard> playerRegCards;
    std::vector<energyCard> playerEnergyCards;

};

class activeHand {

public:

    regCard activeCard;
    trainerCard activeTrainer;
    
    std::vector<regCard> playerActiveBench;
    std::vector<regCard> prizeCards;


    activeHand(regCard aC, std::vector<regCard> bench) :

        activeCard{ aC },

        playerActiveBench{ bench }

    {}

    regCard getRandCard();
    activeHand createAiCharacter();
    std::vector<trainerCard> createTrainerDeckForAI();

};

class playerHand {

public:

    void devModeAddAllCards();


    bool isOutOfEnergy = false;

    pHand playerDeck;
    activeHand playerCurrHand;

    std::vector<trainerCard> playerTrainerCardsSupport;
    std::vector<trainerCard> playerTrainerCardsItems;


    playerHand(bool isAi) :

        playerCurrHand(createActiveHand())


    { 
        createCurrDeck(); 

        if (isAi == true) { createAiHand(); }
         else { modifyActiveHand(); }

    }

    void createAiHand();


    activeHand createActiveHand();
    void createCurrDeck();
    void modifyActiveHand();


    void viewPlayerTrainerCardsSupport();
    void viewPlayerTrainerCardsItems();
    void viewPlayerRegHand(playerHand& currHand);
    void viewPlayerEnergyHand();


    void addEnergyToCard(regCard& currCard, int limiterCap);
    void rmEnergyCard(energyCard& destroyedCard);

    energyCard& getEnergyCard(playerHand& currHand, regCard& currCard, const std::string displayedText);
    regCard& getRegCard(playerHand& currHand, const std::string displayedText);
    regCard getNREMOVErg(playerHand& currHand);


    void drawCard();
    void setTrainer();
    void setActiveCard();

    void Retreat();
    void useCardAbility();
    void useItem();
    void attackPlayer(playerHand& otherHand);

};

