#pragma once
#include "CharacterAIMatch.h"


class aiVSai {

public:

	characterAI& ai1;
	characterAI& ai2;

	aiVSai(characterAI& i1, characterAI& i2) :

		ai1 { i1 },

		ai2 { i2 }

	{}

	void matchV();
	void phasesV(characterAI& aiAtt, characterAI& aiDef);
	

};

