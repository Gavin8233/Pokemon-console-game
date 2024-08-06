#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <map>

class regCard;


struct energyCard {

    std::string energyCardType;

};

struct regCardAttack {

    std::function<void(regCard&, regCard&)> attackEffect;
    bool isBuff;
    std::string attackName;
    int attackDamage;
    std::vector<std::string> reqCardEnergies;

};

struct cardAbility {

    std::function<void(regCard&)> cardAbi;
    std::string abilityEffect;

    void applyAbility(regCard& activeCard) const { cardAbi(activeCard); }

};

class regCard {

public:

    bool isPoisoned = false;
    bool isAsleep = false;
    bool isBurned = false;
    bool isParalyzed = false;
    bool cardAbilityUsed = false;


    cardAbility currCardAbility;
    std::string regCardName;
    std::string regCardType;
    std::vector<regCardAttack> cardAttacks; 
    std::vector<std::string> cardWeaknesses;

    int regCardHealth;
    int legacyHP;
    int currEvolution;
    int damageCounter = 0;

    std::vector<std::string> currEnergyTypes;

    void setWeaknesses() {

        std::map<std::string, std::vector<std::string> > WEAKNESSES {

            { "grass", { "fire", "water" } },

            { "fire", { "water", "darkness" } },

            { "lightning", { "metal", "darkness"} },

            { "water", { "lightning" } },

            { "psychic", { "metal", "fighting"} },

            { "darkness", { "psychic" } },

            { "metal", { "fire"} }

        };

        cardWeaknesses = WEAKNESSES[regCardType];

    }

    regCard(cardAbility cAbility, const std::string cName, const std::string cType, std::vector<regCardAttack> attacks, int cHealth, int evolution) :

        currCardAbility{ cAbility },

        regCardName{ cName },

        regCardType{ cType },

        cardAttacks{ attacks },

        regCardHealth{ cHealth },

        currEvolution{ evolution },

        legacyHP{ cHealth }

    { setWeaknesses(); }


    bool operator==(const regCard& othercard) const {

        return regCardName == othercard.regCardName && regCardHealth == othercard.regCardHealth;

    }

    void applyCardAbility(regCard& activeCard, regCard& cardWithAbility) { cardWithAbility.currCardAbility.applyAbility(activeCard); }

    void applyCardEffect(regCard& currCard, regCard& currAttackedCard, regCardAttack& currAttack) { currAttack.attackEffect(currCard, currAttackedCard); }

    void coutAllEnergies() {

        if (currEnergyTypes.size() == 0) { std::cout << "No energy attached to card, "; return; }

        for (std::string& currCard : currEnergyTypes) { std::cout << currCard << ", "; }

    }

    void coutReqEnergy(regCardAttack& currAtt) { for (std::string& currReq : currAtt.reqCardEnergies) { std::cout << currReq << ", "; } }

    void viewAttacks() {

        for (regCardAttack& currAtt : cardAttacks)
        {
            std::cout << "--" << currAtt.attackName << "--\n" << "Damage: " << currAtt.attackDamage << ", REQ ENERGY: ";
            coutReqEnergy(currAtt); std::cout << "\n";
        }

    }



    //forai========================
    bool isBlankAttackUsed = false;
    //=============================

};