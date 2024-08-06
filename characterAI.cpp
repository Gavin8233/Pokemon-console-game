#include "characterAI.h"
#include <chrono>
#include <random>

static int genRandomNumm(int MINNUM, int MAXNUM) { //CHAT GPT

	std::random_device ranD;

	std::default_random_engine eng;

	unsigned int t = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	std::seed_seq seed{ ranD(), t };
	eng.seed(seed);

	std::mt19937_64 rng(eng());
	std::uniform_int_distribution<int> dist(MINNUM, MAXNUM);

	return dist(rng);

}

const std::vector<energyCard> ENERGYCARDSA{

 { "fire" },

 { "grass" },

 { "psychic" },

 { "water" },

 { "lightning" },

 { "metal" },

 { "fighting" },

 { "darkness" },

};

void characterAI::addEnergies() {

	for (int i = 0; i < genRandomNumm(7, 11); i++) {

		characterEnergies.push_back(ENERGYCARDSA[genRandomNumm(0, ENERGYCARDSA.size() - 1)]);

	}
	
}

void characterAI::chanceForEnergy(std::vector<energyCard>& cards) {

	for (int i = 0; i < genRandomNumm(0, 3); i++) {

		cards.push_back(ENERGYCARDSA.at(genRandomNumm(0, ENERGYCARDSA.size() - 1)));

	}

}

std::vector<trainerCard> characterAI::createTrainerDeckAI() {

	return characterHand.createTrainerDeckForAI();

}

void characterAI::cardChance(activeHand& hand) {

	if (genRandomNumm(0, 8) == 5) { 

		regCard card = characterHand.getRandCard();

		std::cout << "\nNew card drawn! " << card.regCardName << "\n";

		hand.playerActiveBench.push_back(card);
	
	}

}

std::string characterAI::createCharacterName() {

	std::vector<std::string> possibleNames{ "Misty", "Brock" };

	return possibleNames.at(genRandomNumm(0, possibleNames.size() - 1));

}

activeHand characterAI::createCharacter() {

	return characterHand.createAiCharacter();

}

void characterAI::attackPlayerAI(regCard& humanActiveCard) {

	regCardAttack& bestAtt = logic.getBestAttack(characterHand.activeCard, characterHand.activeCard.cardAttacks, characterEnergies, characterHand.playerActiveBench);

	if (bestAtt.attackName.length() <= 1) { isOutofEnergy = true; std::cout << "\n" << characterName << " had no energy, " << characterName << " did not attack!\n"; return; }
	
	logic.attackHuman(characterHand.activeCard, bestAtt, humanActiveCard);

}


void characterAI::retreatAI() {

	logic.ifRetreatNeeded(isOutofEnergy, characterName, characterHand.activeCard, characterHand.playerActiveBench, characterEnergies);

}

void characterAI::itemAI() {

	logic.ifItemNeeded(characterHand, characterTrainers);

}

void characterAI::useAbiAI() {

	logic.aiUseAbility(characterHand.activeCard, characterHand.playerActiveBench);

}