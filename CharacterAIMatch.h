#pragma once
#include "characterAI.h"


class CharacterAIMatch {

	public:

		playerHand humanHand;
		characterAI ai;

		CharacterAIMatch(playerHand human) :

			humanHand { human }

		{  }


		void characterMatch();


		void phasesAI(characterAI& ai, playerHand& human);
		void phasesHuman(playerHand& human, characterAI& ai);


		void AIattackPlayer();
		void attackPlayer();

		void addEnergyToCard(regCard& currCard, int limiterCap);
		regCardAttack& getCardAttack(regCard& currCard, const std::string displayedText);
		void attackCard(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack, int damageMultiplier);

};