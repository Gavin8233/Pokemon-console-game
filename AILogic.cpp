#include "AILogic.h"

//FOR ATTACKING AND ENERGY

static void rmCardPassed(std::vector<energyCard>& energies, const std::string en) {

	for (int i = 0; i < energies.size(); i++) {

		if (energies.at(i).energyCardType == en) { energies.erase(energies.begin() + i); return; }

	}

}

void rmEnergyIfMetFromDeck(std::vector<energyCard>& energies, const std::string en1, const std::string en2) {

	rmCardPassed(energies, en1);
	rmCardPassed(energies, en2);

}

void rmEnergyIfMet(std::vector<std::string>& currEnergies, const std::string energy1, const std::string energy2) {

	auto iter = std::find(currEnergies.begin(), currEnergies.end(), energy1);
	if (iter != currEnergies.end()) { currEnergies.erase(iter); }

	auto iter2 = std::find(currEnergies.begin(), currEnergies.end(), energy2);
	if (iter2 != currEnergies.end()) { currEnergies.erase(iter2); }

}

bool isReqMet(std::string& req, std::vector<energyCard>& currEnergyCards) {

	if (req == "") { return true; }

	for (int i = 0; i < currEnergyCards.size(); i++) {

		if (req == currEnergyCards.at(i).energyCardType) { return true; }

	}

	return false;

}

bool compareRequirements(std::vector<std::string> reqsOriginal, std::vector<std::string> reqsNew) {

	for (std::string& req : reqsOriginal) {

		if (std::find(reqsNew.begin(), reqsNew.end(), req) == reqsNew.end()) { return false; }

	}

	return true;

}

bool checkIfAttackCanBeMet(std::vector<std::string>& attReq, std::vector<energyCard>& currEnergyCards) {

	if (attReq.size() == 0) { return true; }

	std::string req1 = attReq.at(0);
	std::string req2 = "";

	if (attReq.size() > 1) { req2 = attReq.at(1); }

	bool isReq1Met = isReqMet(req1, currEnergyCards);
	bool isReq2Met = isReqMet(req2, currEnergyCards);

	std::vector<bool> areMet{ isReq1Met, isReq2Met };

	if (areMet.at(0) == true && areMet.at(1) == true) { return true; }

	return false;

}

bool checkIfBetterAttackCanBeMet(regCardAttack& currAtt, std::vector<regCardAttack>& attacks, std::vector<std::string> attReq, std::vector<energyCard>& currEnergies) {

	for (regCardAttack& att : attacks) {

		if (att.attackDamage > currAtt.attackDamage && att.attackName != currAtt.attackName) {

			return true;

		}

	}

	return false;

}

bool areAttackReqMet(std::vector<std::string>& attReq, std::vector<std::string> currCardEnergies) {

	if (attReq.size() == 0) { return true; }

	for (std::string& currReq : attReq) {

		if (std::find(currCardEnergies.begin(), currCardEnergies.end(), currReq) == currCardEnergies.end()) {

			return false;

		}

	}

	return true;

}

std::vector<std::string>& getAttackRequirements(regCardAttack& currAtt) {

	return currAtt.reqCardEnergies;

}

static void sacrificeCardForEnergy(regCard& activeCard, std::vector<regCard>& cards, regCardAttack& currAtt) {

	if (cards.size() == 0) { std::cerr << "exittiing"; exit(0); }

	std::vector<int> HPs;

	for (regCard& currCard : cards) {

		HPs.push_back(currCard.regCardHealth);

	}


	int min = *std::min_element(HPs.begin(), HPs.end());


	for (int i = 0; i < cards.size(); i++) {

		if (min == cards[i].regCardHealth) { cards.erase(cards.begin() + i); break; }

	}

	std::string enT1 = currAtt.reqCardEnergies.at(0);
	std::string enT2;
	if (currAtt.reqCardEnergies.size() > 1) { enT2 = currAtt.reqCardEnergies.at(1); }

	activeCard.currEnergyTypes.push_back(enT1);
	activeCard.currEnergyTypes.push_back(enT2);

}

bool checkIfCardHasBlankAttacks(std::vector<regCardAttack>& attacks) {

	for (auto& att : attacks) {

		if (att.reqCardEnergies.size() < 1) { return true; }

	}

	return false;

}

regCardAttack& getBestAttackWithNoReq(std::vector<regCardAttack>& attacks) {

	std::vector<int> damages;

	for (auto& att : attacks) {

		damages.push_back(att.attackDamage);

	}

	int max = *std::max_element(damages.begin(), damages.end());

	for (int i = 0; i < attacks.size(); i++) {

		if (attacks[i].reqCardEnergies.size() < 1 && attacks[i].attackDamage == max) { 
			
			return attacks.at(i); 
		
		}

	}

	//extra check
	for (int i = 0; i < attacks.size(); i++) {

		if (attacks.at(i).reqCardEnergies.size() < 1) { return attacks.at(i); }

	}

}

regCardAttack& AILogic::getHighestDamageAttack(std::vector<regCardAttack>& attacks) {

	std::vector<int> damages;

	for (auto& att : attacks) {

		damages.push_back(att.attackDamage);

	}

	int high = *std::max_element(damages.begin(), damages.end());

	for (int i = 0; i < attacks.size(); i++) {

		if (attacks[i].attackDamage == high) { 
				
			return attacks.at(i); 

		}

	}

	return attacks.at(6); //will break but should NEVER happen

}

static void doNot(regCard& efC, regCard& ofC) {  }

regCardAttack& AILogic::getBestAttack(regCard& activeCard, std::vector<regCardAttack>& cardAttacks, std::vector<energyCard>& energies, std::vector<regCard>& cards) {

	if (checkIfCardHasBlankAttacks(cardAttacks)) { return getBestAttackWithNoReq(cardAttacks); }

	std::vector<std::string> attackRequirements;
	std::string req2;

	int cIdx = 0;

	for (auto& currAtt : cardAttacks) { 

		attackRequirements = getAttackRequirements(currAtt);
		
		if (areAttackReqMet(attackRequirements, activeCard.currEnergyTypes)) {

			if (checkIfBetterAttackCanBeMet(currAtt, cardAttacks, attackRequirements, energies)) { return cardAttacks.at(cIdx + 1); }

			if (attackRequirements.size() > 1) { req2 = attackRequirements.at(1); }

			rmEnergyIfMet(activeCard.currEnergyTypes, attackRequirements.at(0), req2);
				
			return currAtt; 
		
		}
	
		cIdx++;

	}

	std::string enReq2;

	for (auto& currAtt : cardAttacks) {

		if (checkIfAttackCanBeMet(currAtt.reqCardEnergies, energies)) { 

			if (currAtt.reqCardEnergies.size() > 1) { enReq2 = currAtt.reqCardEnergies.at(1); }

			rmEnergyIfMetFromDeck(energies, currAtt.reqCardEnergies.at(0), enReq2); 

			return currAtt;
		
		}

	}

	//regCardAttack& highestDmgAtt = getHighestDamageAttack(cardAttacks);

	regCardAttack blankAtt{ doNot, false, "brr", 0, { "" } };
	return blankAtt;

	/*sacrificeCardForEnergy(activeCard, cards, highestDmgAtt);

	std::string reqq2;
	if (highestDmgAtt.reqCardEnergies.size() > 1) { reqq2 = highestDmgAtt.reqCardEnergies.at(1); }

	rmEnergyIfMet(activeCard.currEnergyTypes, highestDmgAtt.reqCardEnergies.at(0), reqq2);

	return highestDmgAtt; */   //             NOT USED FOR BALANCING

}

static bool checkIsWeak(regCard& currCard, regCard& attackedCard) {

	for (int i = 0; i < attackedCard.cardWeaknesses.size(); i++) {

		if (attackedCard.cardWeaknesses[i] == currCard.regCardType) {

			return true;

		}

	}

	return false;

}

static void checkEffectTypeNApply(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack) {

	if (currCardAttack.isBuff == true) { currCard.applyCardEffect(currCard, attackedCard, currCardAttack); return; }

	currCard.applyCardEffect(attackedCard, currCard, currCardAttack);

}

static void attackCard(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack, int damageMultiplier) {

	std::cout << "\n\n" << currCard.regCardName << " Attacked with " << currCardAttack.attackName << "\n";

	int attackDamageWMULTIPLIER = currCardAttack.attackDamage * damageMultiplier;

	checkEffectTypeNApply(currCard, attackedCard, currCardAttack);

	attackedCard.regCardHealth -= attackDamageWMULTIPLIER;

	std::cout << "\nAttack successful! " << attackedCard.regCardName << " hit for " << attackDamageWMULTIPLIER << "!\n\n";

	attackedCard.damageCounter += attackDamageWMULTIPLIER;

	return;

}

void AILogic::attackHuman(regCard& aiActive, regCardAttack& currAttack, regCard& humanActiveCard) {

	if (aiActive.isAsleep == true) { aiActive.isAsleep = false;  return; }

	if (checkIsWeak(aiActive, humanActiveCard) == true) {

		attackCard(aiActive, humanActiveCard, currAttack, 2);

		std::cout << "\nCARD WAS WEAK; MULTIPLIED!\n";

		return;

	}

	attackCard(aiActive, humanActiveCard, currAttack, 1);

}


//RETREAT 

static bool checkIfAttacksCantBeMet(bool& ooE, regCard& activeCard, std::vector<energyCard>& energies) {

	if (ooE == true) { return false; }

	for (auto& att : activeCard.cardAttacks) {

		if (checkIfAttackCanBeMet(att.reqCardEnergies, energies)) { return false; }

	}

	return true;

}

regCard AILogic::getNewActiveAIRETREAT(std::vector<regCard>& activeBench, std::vector<energyCard>& energies) {

	for (regCard& currCard : activeBench) {

		for (regCardAttack& att : currCard.cardAttacks) {

			if (checkIfAttackCanBeMet(att.reqCardEnergies, energies)) {	return currCard; }

		}

	}

	return activeBench.at(0);

}

static bool checkIfInfl(regCard& card) {

	if (card.isAsleep == true || card.isParalyzed == true) {

		return true;

	}

	return false;

}

static void rmInfl(regCard& card) { card.isAsleep = false; card.isBurned = false; card.isParalyzed = false; card.isPoisoned = false; }


void AILogic::ifRetreatNeeded(bool& ooE, std::string cName, regCard& activeCard, std::vector<regCard>& activeBench, std::vector<energyCard>& energies) {

	if (activeCard.regCardHealth <= 50 || checkIfAttacksCantBeMet(ooE, activeCard, energies) || checkIfInfl(activeCard)) {

		rmInfl(activeCard);

		std::cout << "\n" << cName << " retreated!\n";

		regCard oldActive = activeCard;

		regCard newActive = getNewActiveAIRETREAT(activeBench, energies);

		activeCard = newActive;

		activeBench.push_back(oldActive);

		auto it = std::find(activeBench.begin(), activeBench.end(), newActive);
		activeBench.erase(it);

	}

}


//AI USE ITEM

static int searchForHeal(std::vector<trainerCard>& cards) {

	for (int i = 0; i < cards.size(); i++) {

		if (cards.at(i).trainerName == "Heal Potion") { return i; }

	}

	return -1;

}

static int searchForWakeUp(std::vector<trainerCard>& cards) {

	for (int i = 0; i < cards.size(); i++) {

		if (cards.at(i).trainerName == "Wake Up") { return i; }

	}

	return -1;

}

void AILogic::ifItemNeeded(activeHand& currHand, std::vector<trainerCard>& cards) {

	if (currHand.activeCard.damageCounter >= 40) {

		int checkIfHeal = searchForHeal(cards);

		if (checkIfHeal != -1) { cards.at(checkIfHeal).appTrainerEffect(currHand); }
		else { return; }

		cards.erase(cards.begin() + checkIfHeal);

	}

	if (currHand.activeCard.isAsleep == true) {

		int checkIfWakeUp = searchForWakeUp(cards);

		if (checkIfWakeUp != -1) { cards.at(checkIfWakeUp).appTrainerEffect(currHand); }
		else { return; }

		cards.erase(cards.begin() + checkIfWakeUp);

	}

}



//AI USE CARD ABILITY


static bool checkifAllBlank(std::vector<regCard>& pActiveBench) {

	for (regCard& currCard : pActiveBench) {

		if (currCard.currCardAbility.abilityEffect != "" || currCard.cardAbilityUsed == true) { return false; }

	}

	return true;

}

static bool mrMimeCheckAbi(regCard& activeCard, regCard& mime) {

	int check = 10;

	for (int i = 0; i < 8; i++) {

		if (check == activeCard.damageCounter) { return true; }

		check += 20;

	}

	return false;

}

static void useAbility(regCard& currCard, regCard& activeCard) {

	if (currCard.cardAbilityUsed == true) { return; }

	currCard.cardAbilityUsed = true;

	currCard.applyCardAbility(activeCard, currCard);

}

static bool checkSpecialCondit(regCard& activeCard, std::vector<regCard>& cards) {

	for (regCard& card : cards) {

		if (card.regCardName == "Mr. Mime") { if (mrMimeCheckAbi(activeCard, card)) { useAbility(card, activeCard); return true; } }

	}

	return false;

}

void AILogic::aiUseAbility(regCard& activeCard, std::vector<regCard>& cards) {

	if (checkifAllBlank(cards)) { return; }


	if (checkSpecialCondit(activeCard, cards)) { return; }

	for (regCard& card : cards) {

		if (card.currCardAbility.abilityEffect != "" && card.cardAbilityUsed != true
			&& card.regCardName != "Mr. Mime") {

			useAbility(card, activeCard);

			return;

		}

	}

}