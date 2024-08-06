#include "aiVSai.h"

static std::string isInflictedV(regCard& currCard) {

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


std::map<std::string, std::function<void(regCard&)>> cardStatusesAPPV{

	{ "0", blankFunc },

	{ "burn", ifBrn },

	{ "pois", ifPois },

	{ "aslp", ifAslp },

	{ "paral", blankFunc }

};

static void appStatusEffectsAIV(characterAI& ai) {

	regCard& activeC = ai.characterHand.activeCard;

	std::string currEff = isInflictedV(activeC);

	cardStatusesAPPV[currEff](activeC);

	for (regCard& currCard : ai.characterHand.playerActiveBench) {

		std::string ifInfl = isInflictedV(currCard);

		auto it = cardStatusesAPPV.find(ifInfl);

		if (it != cardStatusesAPPV.end()) { it->second(currCard); }

	}

}

static void ifKOAIV(characterAI& winner, characterAI& loser) {

	if (loser.characterHand.activeCard.regCardHealth <= 0) {

		regCard newPrize = loser.characterHand.activeCard;

		winner.characterHand.prizeCards.push_back(newPrize);

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

static bool checkWinConditV(characterAI& WinAI, characterAI& LoseAI) {

	if (WinAI.characterHand.prizeCards.size() >= 5) { return true; }
	if (LoseAI.characterHand.playerActiveBench.size() <= 0) { return true; }

	return false;

}

void aiVSai::phasesV(characterAI& aiAtt, characterAI& aiDef) {

	appStatusEffectsAIV(aiAtt);

	aiAtt.chanceForEnergy(aiAtt.characterEnergies);

	aiAtt.cardChance(aiAtt.characterHand);

	aiAtt.retreatAI();

	aiAtt.useAbiAI();

	aiAtt.attackPlayerAI(aiDef.characterHand.activeCard);

}

void aiVSai::matchV() {

	while ( true ) {

		phasesV(ai1, ai2);

		ifKOAIV(ai1, ai2);

		if (checkWinConditV(ai1, ai2)) { std::cout << "\n\n" << ai1.characterName << " won!\n\n"; return; }

		phasesV(ai2, ai1);

		ifKOAIV(ai2, ai1);

		if (checkWinConditV(ai2, ai1)) { std::cout << "\n\n" << ai2.characterName << " won!\n\n"; return; }

	}

}