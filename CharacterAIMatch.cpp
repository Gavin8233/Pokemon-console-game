#include "CharacterAIMatch.h"


int getCardIndex(const std::string displayedText) {

	int chosenCardIndex = 0;
	std::cout << displayedText;
	std::cin >> chosenCardIndex;

	return chosenCardIndex;

}


void CharacterAIMatch::addEnergyToCard(regCard& currCard, int limiterCap) {

	humanHand.addEnergyToCard(currCard, limiterCap);

}

static void viewPlayerActiveCardInfo(regCard& activeCard) {

	std::cout << "\n\nCARD IN PLAY\n--" << activeCard.regCardName << "--\n" <<
		"\nHealth: " << activeCard.regCardHealth << ", Damage counter: " <<
		activeCard.damageCounter <<
		", Energy attached: "; activeCard.coutAllEnergies();

	std::cout << "\nAttacks: ";

	for (regCardAttack& currAtt : activeCard.cardAttacks) {

		std::cout << currAtt.attackName << ", Req energy(ies): ";
		for (std::string& attEn : currAtt.reqCardEnergies) {

			std::cout << attEn << ", ";

		}

	}

	std::cout << "\n\n";

}

regCardAttack& CharacterAIMatch::getCardAttack(regCard& currCard, const std::string displayedText) {

	currCard.viewAttacks();

	regCardAttack& currAttack = currCard.cardAttacks[getCardIndex(displayedText)];

	return currAttack;

}

static void checkEffectTypeNApply(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack) {

	if (currCardAttack.isBuff == true) { currCard.applyCardEffect(currCard, attackedCard, currCardAttack); return; }

	currCard.applyCardEffect(attackedCard, currCard, currCardAttack);

}

static bool checkReq(regCard& currCard, regCardAttack& currCardAttack) {

	std::vector<std::string>& currEnergiesAttached = currCard.currEnergyTypes;
	std::vector<std::string>& attackEnergiesRequired = currCardAttack.reqCardEnergies;

	for (std::string& currType : attackEnergiesRequired) {

		auto iter = std::find(currEnergiesAttached.begin(), currEnergiesAttached.end(), currType);

		if (iter != currEnergiesAttached.end()) { currEnergiesAttached.erase(iter); }
		else { return false; }

	}

	return true;

}

void CharacterAIMatch::attackCard(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack, int damageMultiplier) {

	std::cout << "\n\n" << currCard.regCardName << " Attacked with " << currCardAttack.attackName << "\n";

	int attackDamageWMULTIPLIER = currCardAttack.attackDamage * damageMultiplier;

	if (checkReq(currCard, currCardAttack) == true) {

		checkEffectTypeNApply(currCard, attackedCard, currCardAttack);

		attackedCard.regCardHealth -= attackDamageWMULTIPLIER;

		std::cout << "\nAttack successful! " << attackedCard.regCardName << " hit for " << attackDamageWMULTIPLIER << "!\n\n";

		attackedCard.damageCounter += attackDamageWMULTIPLIER;

		return;

	}

	throw std::invalid_argument(" \n Attack conditions not met! \n ");

}

static bool checkIsWeak(regCard& currCard, regCard& attackedCard) {

	for (int i = 0; i < attackedCard.cardWeaknesses.size(); i++) {

		if (attackedCard.cardWeaknesses[i] == currCard.regCardType) {

			return true;

		}

	}

	return false;

}

void CharacterAIMatch::attackPlayer() {

	regCard& currCard = humanHand.playerCurrHand.activeCard;

	if (currCard.isAsleep == true) { currCard.isAsleep = false;  return; }

	regCard& attackedCard = ai.characterHand.activeCard;

	viewPlayerActiveCardInfo(currCard);


	addEnergyToCard(currCard, 2);
	if (humanHand.isOutOfEnergy == true) { humanHand.isOutOfEnergy = false; return; }

	std::cout << "\nOpponets active card is " << attackedCard.regCardName << ", Health: " << attackedCard.regCardHealth << "\n";

	std::cout << "Your turn, attack card!\n\n";

	regCardAttack& currCardAttack = getCardAttack(currCard, "\nChoose attack: ");


	if (checkIsWeak(currCard, attackedCard) == true) {

		attackCard(currCard, attackedCard, currCardAttack, 2);

		std::cout << "\nCARD WAS WEAK; MULTIPLIED!\n";

		return;

	}

	attackCard(currCard, attackedCard, currCardAttack, 1);

}

void CharacterAIMatch::AIattackPlayer() {

	ai.attackPlayerAI(humanHand.playerCurrHand.activeCard);

}



//MAIN GAME

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


std::map<std::string, std::function<void(regCard&)>> cardStatusesAPP{

	{ "0", blankFunc },

	{ "burn", ifBrn },

	{ "pois", ifPois },

	{ "aslp", ifAslp },

	{ "paral", blankFunc }

};

static void appStatusEffectsAI(characterAI& ai) {

	regCard& activeC = ai.characterHand.activeCard;

	std::string currEff = isInflicted(activeC);

	cardStatusesAPP[currEff](activeC);

	for (regCard& currCard : ai.characterHand.playerActiveBench) {

		std::string ifInfl = isInflicted(currCard);

		auto it = cardStatusesAPP.find(ifInfl);

		if (it != cardStatusesAPP.end() ) { it->second(currCard); }

	}

}

static void applyStatusEffects(playerHand& currHand) {

	regCard& activeCard = currHand.playerCurrHand.activeCard;

	std::string currEffectAC = isInflicted(activeCard);

	cardStatusesAPP[currEffectAC](activeCard);

	for (regCard& currCard : currHand.playerCurrHand.playerActiveBench) {

		std::string ifEff = isInflicted(currCard);

		auto iter = cardStatusesAPP.find(ifEff);

		if (iter != cardStatusesAPP.end()) { iter->second(currCard); }

	}

	std::cout << "\n";

}

static bool checkWinConditAI(characterAI& ai, playerHand& human) {

	if (human.playerCurrHand.playerActiveBench.size() < 1) { return true; }
	if (ai.characterHand.prizeCards.size() >= 5) { return true; }

	return false;

}

static bool checkWinCondit(characterAI& ai, playerHand& human) {

	if (ai.characterHand.playerActiveBench.size() < 1) { return true; }
	if (human.playerCurrHand.prizeCards.size() >= 5) { return true; }

	return false;

}

static void chanceForCard(characterAI& forLogic, activeHand& effHand) {

	forLogic.cardChance(effHand);

}

static void ifKOAI(playerHand& winner, characterAI& loser) {

	if (loser.characterHand.activeCard.regCardHealth <= 0) {

		regCard newPrize = loser.characterHand.activeCard;

		winner.playerCurrHand.prizeCards.push_back(newPrize);

		std::cout << newPrize.regCardName << " was knocked out!\n";

		regCard newActive = loser.logic.getNewActiveAIRETREAT(loser.characterHand.playerActiveBench, loser.characterEnergies);

		loser.characterHand.activeCard = newActive;

		auto itr = std::find(loser.characterHand.playerActiveBench.begin(),
			loser.characterHand.playerActiveBench.end(), newActive);

		if (itr != loser.characterHand.playerActiveBench.end()) {

			loser.characterHand.playerActiveBench.erase(itr);

		}

	}

}

static void IfKOHuman(playerHand& loser, characterAI& winner) {

	if (loser.playerCurrHand.activeCard.regCardHealth <= 0) {

		regCard newPrize = loser.playerCurrHand.activeCard;

		winner.characterHand.prizeCards.push_back(newPrize);

		std::cout << "\nYour card was knocked out, pick a new one!\n\n";

		loser.setActiveCard();

	}

}

void CharacterAIMatch::phasesAI(characterAI& ai, playerHand& human) {

	std::cout << "\n\n" << ai.characterName << " TURN!\n";

	appStatusEffectsAI(ai);

	chanceForCard(ai, ai.characterHand);

	ai.chanceForEnergy(ai.characterEnergies);

	ai.itemAI();

	ai.retreatAI();

	ai.useAbiAI();

	AIattackPlayer();

}

void CharacterAIMatch::phasesHuman(playerHand& human, characterAI& ai) {

	applyStatusEffects(human);

	chanceForCard(ai, human.playerCurrHand);

	ai.chanceForEnergy(human.playerDeck.playerEnergyCards);


	std::cout << "\n\n\nPLAYER TURN!\n";

	while (true) {

		std::string uChoice;
		std::cout << "\nattack / item / retreat / ability: ";
		std::cin >> uChoice;


		if (uChoice.at(0) == 'i') { human.useItem(); continue; }
		if (uChoice.at(0) == 'r') { human.Retreat(); continue; }
		if (uChoice.at(0) == 'a' && uChoice.at(1) == 'b') { human.useCardAbility(); continue; }

		else { attackPlayer(); return; }

	}

}

void CharacterAIMatch::characterMatch() {

	while (true) {

		phasesHuman(humanHand, ai);

		ifKOAI(humanHand, ai);

		if (checkWinCondit(ai, humanHand)) { std::cout << "\n\nPlayer wins!!!\n\n\n"; return; }

		phasesAI(ai, humanHand);

		IfKOHuman(humanHand, ai);

		if (checkWinConditAI(ai, humanHand)) { std::cout << ai.characterName << " wins!!!\n\n\n"; return; }

	}

}