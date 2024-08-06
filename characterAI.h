#pragma once
#include "playerHand.h"
#include "AILogic.h"


class characterAI {

public:

	bool isOutofEnergy = false;

	AILogic logic;

	std::string characterName;
	activeHand characterHand;
	std::vector<energyCard> characterEnergies;
	std::vector<trainerCard> characterTrainers;

	characterAI() :

		characterHand(createCharacter()),

		characterName(createCharacterName()),

		characterTrainers(createTrainerDeckAI())

	{ addEnergies(); }

	std::vector<trainerCard> createTrainerDeckAI();

	void attackPlayerAI(regCard& humanActiveCard);
	void chanceForEnergy(std::vector<energyCard>& cards);

	std::string createCharacterName();
	void addEnergies();
	activeHand createCharacter();
	void retreatAI();
	void itemAI();
	void useAbiAI();
	void cardChance(activeHand& hand);

};

