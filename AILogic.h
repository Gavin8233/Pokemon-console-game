#pragma once
#include "playerHand.h"


class AILogic {

public:

	regCard getNewActiveAIRETREAT(std::vector<regCard>& activeBench, std::vector<energyCard>& energies);

	regCardAttack& getBestAttack(regCard& activeCard, std::vector<regCardAttack>& cardAttacks, std::vector<energyCard>& energies, std::vector<regCard>& cards);
	regCardAttack& getHighestDamageAttack(std::vector<regCardAttack>& attacks);

	void attackHuman(regCard& aiActive, regCardAttack& currAttack, regCard& humanActiveCard);
	void ifItemNeeded(activeHand& currHand, std::vector<trainerCard>& cards);
	void aiUseAbility(regCard& activeCard, std::vector<regCard>& cards);
	void ifRetreatNeeded(bool& ooE, std::string cName, regCard& activeCard, std::vector<regCard>& activeBench, std::vector<energyCard>& energies);


};

