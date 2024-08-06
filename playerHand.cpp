#include "playerHand.h"
#include <map>
#include <cmath>
#include <chrono>
#include <random>


//=========FUNCTIONAL============

static int countAmountEnergyCards(const std::string currType, playerHand& currHand) {

	int finNum = 0;

	for (energyCard& currCard : currHand.playerDeck.playerEnergyCards) {

		if (currCard.energyCardType == currType) {

			finNum++;

		}

	}

	return finNum;

}


static int genRandomNum(int MINNUM, int MAXNUM) { //CHAT GPT

	std::random_device ranD;

	std::default_random_engine eng;

	unsigned int t = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	std::seed_seq seed{ ranD(), t };
	eng.seed(seed);

	std::mt19937_64 rng(eng());
	std::uniform_int_distribution<int> dist(MINNUM, MAXNUM);

	return dist(rng);

}


static bool checkIsWeak(regCard& currCard, regCard& attackedCard) {

	for (int i = 0; i < attackedCard.cardWeaknesses.size(); i++) {

		if (attackedCard.cardWeaknesses[i] == currCard.regCardType) {

			return true;

		}

	}

	return false;

}






//=============VIEW=============

void playerHand::viewPlayerRegHand(playerHand& currHand) {

	short currIndex = 0;

	for (regCard& currCard : currHand.playerCurrHand.playerActiveBench) {

		std::cout << currIndex << ": " << currCard.regCardName << ", " << currCard.regCardType << " type; Health: " << currCard.regCardHealth << "\n";

		currCard.viewAttacks();

		std::cout << "\n";

		currIndex++;

	}

	std::cout << "\n";

}


void playerHand::viewPlayerEnergyHand() {

	short currIndex = 0;

	for (energyCard& currCard : playerDeck.playerEnergyCards) {

		std::cout << currIndex << ": " << "Energy card: " << currCard.energyCardType << "\n";

		currIndex++;

	}

	std::cout << "\n";

}


void playerHand::viewPlayerTrainerCardsSupport() {

	short currIndex = 0;

	for (trainerCard& currCard : playerTrainerCardsSupport) {

		std::cout << currIndex << ": " << currCard.trainerName << ", " << currCard.trainerDescription << "\n";

		currIndex++;

	}

	std::cout << "\n";

}


void playerHand::viewPlayerTrainerCardsItems() {

	short currIndex = 0;

	for (trainerCard& currCard : playerTrainerCardsItems) {

		std::cout << currIndex << ": " << currCard.trainerName << ", " << currCard.trainerDescription << "\n";

		currIndex++;

	}

	std::cout << "\n";

}









//===================ACCESS CARDS=================

static int getCardIndex(const std::string displayedText) {

	int chosenCardIndex = 0;
	std::cout << displayedText;
	std::cin >> chosenCardIndex;

	return chosenCardIndex;

}


regCard& playerHand::getRegCard(playerHand& currHand, const std::string displayedText) {

	viewPlayerRegHand(currHand);

	regCard& currCard = currHand.playerCurrHand.playerActiveBench[getCardIndex(displayedText)];

	std::cout << "\nChosen card: " << currCard.regCardName << "\n\n";

	return currCard;

}


energyCard& playerHand::getEnergyCard(playerHand& currHand, regCard& currCard, const std::string displayedText) {

	viewPlayerEnergyHand();

	energyCard& currPlayerEnergyCard = currHand.playerDeck.playerEnergyCards[getCardIndex(displayedText)];

	std::cout << "\n" << "Chosen Energy: " << currPlayerEnergyCard.energyCardType << "\n\n";

	return currPlayerEnergyCard;

}








//=================WEAKNESSES AND ENERGY==================

const std::vector<energyCard> ENERGYCARDS{

	{ "fire" },

	{ "grass" },

	{ "psychic" },

	{ "water" },

	{ "lightning" },
 
	{ "metal" },

	{ "fighting" },

	{ "darkness" },

};



//======================CARD ATTACK EFFECTS ( GENERALIZED EFFECTS )========================= 
//=========CURR CARD IS ATTACKED CARD IF FALSE, IF TRUE IT IS EFFECTED CARD(refer to POKEMONCARDS)============

static void doNothing(regCard& currCard, regCard& effectedCard) { return; }

static void poisonEffect(regCard& currCard, regCard& effectedCard) { currCard.isPoisoned = true; }

static void burnEffect(regCard& currCard, regCard& effectedCard) {	std::cout << "\nEnemy burned!\n"; currCard.isBurned = true; }

static void putToSleep(regCard& currCard, regCard& effectedCard) { currCard.isAsleep = true; }

static void paralyzeEffect(regCard& currCard, regCard& effectedCard) { currCard.isParalyzed = true; }




//===============CARD ATTACK EFFECTS ( CARD SPECIFIC EFFECTS )=================

static void coutText(const std::string displayedText) { std::cout << "\nAttack Special Ability!, " << displayedText << "\n"; }

static std::string getRandEnergy() { return ENERGYCARDS[genRandomNum(0, ENERGYCARDS.size() - 1)].energyCardType; }


static void Baxcalibur_SuperCold(regCard& currCard, regCard& effectedCard) {

	coutText("Adds 1 energy to card!");

	currCard.currEnergyTypes.push_back(getRandEnergy());

}

static void Meowth_Nap(regCard& currCard, regCard& effectedCard) {

	std::cout << "\nMeowth took a nap! + 20 HP!\n\n";

	currCard.regCardHealth += 20;

}

static void Thundrus_Charge(regCard& currCard, regCard& effectedCard) {

	coutText("Add 1 energy card to card!");

	currCard.currEnergyTypes.push_back(getRandEnergy());

}

static void Mewtwo_XBALL(regCard& currCard, regCard& effectedCard) {

	coutText("Add 40 damage to card attack!");

	currCard.cardAttacks.at(1).attackDamage += 40;

}

static void Rayquaza_CelestialRoar(regCard& currCard, regCard& effectedCard) {

	coutText("Adds 2 energy cards to card!");

	for (int i = 0; i < 2; i++) { currCard.currEnergyTypes.push_back(getRandEnergy()); }

}


static int countCardType(regCard& currCard, const std::string currType) {

	int cccc = 0;
	int fin = 0;

	for (std::string currT : currCard.currEnergyTypes) {

		if (currT == currType) { fin++; currCard.currEnergyTypes.erase(currCard.currEnergyTypes.begin() + cccc); }

		cccc++;

	}

	return fin;

}

static void RayquazaEX_DragonBurst(regCard& currCard, regCard& effectedCard) {

	coutText("Discards all fire and lightning cards dealing an additional 40 damage for each");

	int fireCardCount = countCardType(effectedCard, "fire");
	int lightningCount = countCardType(effectedCard, "lightning");

	int totalAdditional = 40 * (fireCardCount + lightningCount);

	std::cout << "\n" << currCard.regCardName << " hit for additional " << totalAdditional << " damage!\n";

}

static void BlackKyruem_DualClaw(regCard& currCard, regCard& effectedCard) {

	coutText("Deals 20 damage * 5 if coin lands on heads");

	if (genRandomNum(0, 1) == 0) {

		currCard.regCardHealth -= 80;

		std::cout << "\nHeads! Damage dealt!\n\n";

		currCard.damageCounter += 80;

	}
	else { std::cout << "\nTails! No damage!\n\n"; }

}

static void Aggron_Rend(regCard& currCard, regCard& effectedCard) {

	coutText("Deals 30 damage, *2 if defending pokemon has taken damage");

	if (currCard.damageCounter > 0) {

		currCard.regCardHealth -= 30;

		std::cout << "\n" << currCard.regCardName << " hit for 30 extra damage!\n";

		currCard.damageCounter += 30;

	}

}

static void Kingdra_HydroBlast(regCard& currCard, regCard& effectedCard) {

	coutText("Does 50 damage + 20 for each water energy card attached to pokemon, and removes those energy cards");

	int aDamage = 20 * effectedCard.currEnergyTypes.size();

	effectedCard.currEnergyTypes.clear();

	currCard.regCardHealth -= aDamage;

	currCard.damageCounter += aDamage;

	std::cout << "\n" << currCard.regCardName << " hit for additional " << aDamage << " damage!\n";

}

static void Machamp_SteamingMad(regCard& currCard, regCard& effectedCard) {

	coutText("Does 20 damage for every 20 damage taken on this card");

	double dDmgCnt = effectedCard.damageCounter;

	int additionalDmgMulti = 0;
	int tempCheckNum = 0;

	while (tempCheckNum < dDmgCnt) { tempCheckNum += 20; additionalDmgMulti++; }

	int additionalDmg = 20 * additionalDmgMulti;

	std::cout << "\n" << currCard.regCardName << " hit for additional " << additionalDmg << " damage!\n";

	currCard.regCardHealth -= additionalDmg;

	currCard.damageCounter += additionalDmg;

}

static void Regigigas_GigatonPunch(regCard& currCard, regCard& effectedCard) {

	coutText("Flip a coin; if heads, attack does 40 more damage");

	if(genRandomNum(0, 1) == 0) {

		std::cout << "\nHeads! " << currCard.regCardName << " hit for 40 damage!\n";

		currCard.regCardHealth -= 40;

		currCard.damageCounter -= 40;

		return;

	}

	std::cout << "\nTails! No extra damage dealt!\n";

}






//===================POKEMON CARDS & ABILITIES=====================

static void cardAbility_DoNothing(regCard& activeCard) { return; }


static void cardAbility_RaiseHealth(regCard& activeCard) { activeCard.regCardHealth += 100; activeCard.legacyHP += 100; }

static void cardAbility_MrMime(regCard& activeCard) {

	int checkReq = 10;

	int intImax = 1;
	while (checkReq < activeCard.damageCounter) { checkReq += 20; intImax++; }

	checkReq = 10;

	for (int i = 0; i < intImax; i++) {

		if(checkReq == activeCard.damageCounter) { 
		
			std::cout << "\nCard ability successful!, all damage taken removed\n";

			activeCard.damageCounter = 0;

			activeCard.regCardHealth = activeCard.legacyHP;

			return;
		
		}

		checkReq += 20;

	}

	std::cout << "\nAbility requirements not met, nothing happened!\n";

}


std::vector<regCard> POKEMONCARDS {




	// GRASS TYPES

	{ { cardAbility_DoNothing, ""}, "Spidops", "grass", { { doNothing, false, "String Truss", 30, { "grass" } }, { doNothing, false, "Jet Headbutt", 100, { "grass"  } } }, 100, 0},

	{ { cardAbility_DoNothing, ""}, "Smoliv", "grass", { { doNothing, false, "Spray Fluid", 20, { "grass" } }, { doNothing, false, "Slap", 50, { "grass" } } }, 60, 0 },

	{ { cardAbility_DoNothing, ""}, "Bulbasaur", "grass", { { doNothing, false, "Vine Whip", 10, { "grass" } }, { doNothing, false, "Razor Leaf", 20, { "grass" } } }, 70, 0 },

	{ { cardAbility_DoNothing, ""}, "Sprigatito", "grass", { { doNothing, false, "Mini Drain", 10, { "grass" } }, { doNothing, false, "Kick", 20, { "grass" } } }, 70, 0 },

	{ { cardAbility_DoNothing, ""}, "Vivillon", "grass", { { doNothing, false, "Powder", 30, { "grass" } }, { poisonEffect, false, "Dizzying Poison", 0, { "grass" } } }, 120, 1 },






	// FIRE TYPES

	{ { cardAbility_DoNothing, ""}, "Fuecoco", "fire", { { doNothing, false, "Super Singe", 50, { "fire" } }, { doNothing, false, "Fire Whip", 20, { "fire" } } }, 90, 0 },

	{ { cardAbility_DoNothing, ""}, "Growlithe", "fire", { { doNothing, false, "Fire Claws", 70, { "fire" } } }, 90, 0 },

	{ { cardAbility_DoNothing, ""}, "Nine Tails", "fire", { { doNothing, false, "Tail Whip", 70, { "fire" } } }, 110, 0 },

	{ { cardAbility_DoNothing, ""}, "Blaziken", "fire", { { doNothing, false, "Clutch", 20, { } }, { doNothing, false, "Flamethrower", 80, { "fire" } } }, 90, 1 },

	{ { cardAbility_DoNothing, ""}, "Cyndaquil", "fire", { { doNothing, false, "Scratch", 10, { } }, { burnEffect, false, "Singe", 10, { "fire" } } }, 80, 0 },






	// WATER TYPES

	{ { cardAbility_DoNothing, ""}, "Baxcalibur", "water", { { doNothing, false, "Buster Tail", 130, { "water" } }, { Baxcalibur_SuperCold, true, "Super Cold", 0, { "water" } } }, 160, 2 },

	{ { cardAbility_DoNothing, ""}, "Gyarados", "water", { { doNothing, false, "Aqua sonic", 80, { "water" } } }, 100, 1 },

	{ { cardAbility_DoNothing, ""}, "Kingdra", "water", { { Kingdra_HydroBlast, false, "Hydro Blast", 50, { "water" } } }, 150, 0 },

	{ { cardAbility_DoNothing, ""}, "Kyogre", "water", { { doNothing, false, "Super Tidal Wave", 60, { "water", "water" } }, { doNothing, false, "Splash", 30, { "water" } } }, 70, 1 },





	// FIGHTING TYPES

	{ { cardAbility_DoNothing, ""}, "Geodude", "fighting", { { doNothing, false, "Light Punch", 25, { } } }, 50, 0 },

	{ { cardAbility_DoNothing, ""}, "Machamp", "fighting", { { Machamp_SteamingMad, false, "Steaming Mad", 20, { "fighting" } }, { doNothing, false, "Crazy Hammer", 40, { "fighting" } } }, 180, 0 },





	// COLORLESS TYPES


	{ { cardAbility_RaiseHealth, "Raises health by 100"}, "Meowth", "colorless", { { Meowth_Nap, true, "Nap", 0, { } }, { doNothing, false, "Jump On", 25, { } } }, 60, 0 },

	{ { cardAbility_DoNothing, ""}, "Jigglypuff", "colorless", { { putToSleep, false, "Sleepy Song", 0, {  } } }, 50, 0 },





	// LIGHTNING TYPES


	{ { cardAbility_DoNothing, ""}, "Luxia", "lightning", { { doNothing, false, "Random Spark", 50, { "lightning" } } }, 80, 1 },

	{ { cardAbility_DoNothing, ""}, "Pikachu", "lightning", { { doNothing, false, "Thunderbolt", 100, { "lightning" } } }, 90, 0 },

	{ { cardAbility_DoNothing, ""}, "Flaafy", "lightning", { { doNothing, false, "Lightning Ball", 40, { "lightning" } } }, 80, 1 },

	{ { cardAbility_DoNothing, ""}, "Thundrus", "lightning", { { Thundrus_Charge, true, "Charge", 0, { "lightning" } }, { doNothing, false, "Disaster Volt", 80, { "lightning" } } }, 110, 0 },





	// PSYCHIC TYPES


	{ { cardAbility_DoNothing, ""}, "Mewtwo", "psychic", { { Mewtwo_XBALL, true, "X-Ball", 20, { "psychic" } }, { doNothing, false, "Psydrive", 120, { "psychic" } } }, 170, 0 },

	{ { cardAbility_DoNothing, ""}, "Jynx", "psychic", { { doNothing, false, "Feverish Kiss", 40, { "psychic" } } }, 70, 0 },

	{ { cardAbility_DoNothing, ""}, "Starmie", "water", { { doNothing, false, "Rainbow Star", 50, { "psychic" } } }, 80, 1 },

	{ { cardAbility_DoNothing, ""}, "Hypno", "psychic", { { putToSleep, false, "Hypnotic Ray", 25, {  } } }, 80, 0},

	{ { cardAbility_DoNothing, ""}, "Drowsee", "psychic", { { doNothing, false, "Ambush", 20, {  } } }, 100, 0},

	{ { cardAbility_MrMime, "if active card damage counter can be reached by adding 20 to 10 as many times as needed, all damage gets removed"}, "Mr. Mime", "psychic", { { putToSleep, false, "Breakdown", 30, {"psychic" } } }, 80, 0},





	// DRAGON TYPES

	{ { cardAbility_DoNothing, ""}, "Rayquaza", "dragon", { { Rayquaza_CelestialRoar, true, "Celestial Roar", 0, { "lightning" } }, { doNothing, false, "Dragon Burst", 130, { "fire", "water" } } }, 170, 0 },

	{ { cardAbility_DoNothing, ""}, "Black Kyruem", "dragon", { { BlackKyruem_DualClaw, false, "Dual Claw", 20, { "lightning" } }, { doNothing, false, "Flash Freeze", 100, { "water", "lightning" } } }, 130, 0 },

	{ { cardAbility_DoNothing, ""}, "Rayquaza EX", "dragon", { { RayquazaEX_DragonBurst, false, "Dragon Burst", 40, { "fire" } } }, 100, 0 },





	// METAL TYPES

	{ { cardAbility_DoNothing, ""}, "Aggron", "metal", { { Aggron_Rend, false, "Rend", 30, { "metal" } }, { doNothing, false, "Metal Surge", 50, { "metal" } } }, 150, 0 },

	{ { cardAbility_DoNothing, ""}, "Regigigas", "metal", { { Regigigas_GigatonPunch, false, "Gigaton Punch", 60, { "metal" } } }, 100, 0 },

	{ { cardAbility_DoNothing, ""}, "Charizard", "metal", { { doNothing, false, "Metal Burn", 120, { "metal", "lightning" } } }, 120, 0},




	// DARKNESS TYPES

	{ { cardAbility_DoNothing, ""}, "Tyranitar", "darkness", { { doNothing, false, "Hammer In", 50, { "darkness" } }, { doNothing, false, "Break Ground", 130, { "darkness", "darkness" } } }, 180, 0 },




	//throwing away sectioning for ai reasons

//starts at idx 35
	{ {cardAbility_DoNothing, ""}, "Venosaur", "grass", { { doNothing, false, "Pollen Hazard", 20, {  } }, { doNothing, false, "Solarbeam", 90, { "grass" } } }, 150, 0 },

	{ {cardAbility_DoNothing, ""}, "Charizard", "fire", { { doNothing, false, "Burn Down", 200, {"fire", "fire" } }, { doNothing, false, "Breathe Fire", 30, {  } } }, 160, 0 },

	{ {cardAbility_DoNothing, ""}, "Wailord", "water", { { doNothing, false, "Sink Deep", 40, {  } }, { doNothing, false, "Giant Wave", 100, { "water" } } }, 200, 1 },

	{ {cardAbility_DoNothing, ""}, "Golem", "fighting", { { doNothing, false, "Magnitude", 60, {  } }, { doNothing, false, "Double-Edge", 120, { "fighting" } } }, 160, 1 },

	{ { cardAbility_DoNothing, ""}, "Kommo-o", "dragon", { { doNothing, false, "Shred", 130, { "lightning", "fighting"} } }, 240, 2 },

	{ {cardAbility_DoNothing, ""}, "Ultra Necrozma", "dragon", { {doNothing, false, "Shining Burst", 100, { "psychic" } } }, 130, 0 },

	{ {cardAbility_DoNothing, ""}, "Dragonite", "dragon", { {doNothing, false, "Giga Impact", 230, { "lightning", "water" } } }, 280, 2 },

	{ {cardAbility_DoNothing, ""}, "Registeel", "metal", { {doNothing, false, "Protect Charge", 80, { "metal" } } }, 180, 1 },

	{ {cardAbility_DoNothing, ""}, "Slowbro", "darkness", { {doNothing, false, "Tripping Shot", 120, { "darkness"} } }, 210, 0 },


};







//=====================TRAINER CARD& EFFECTS================

static void brock_TRAINER(activeHand& effectedHand) {

	for (int i = 0; i < 3; i++) {

		regCard newCard = POKEMONCARDS.at(genRandomNum(0, POKEMONCARDS.size() - 1));

		std::cout << "New Card! " << newCard.regCardName << "\n";

		effectedHand.playerActiveBench.push_back(newCard);

	}

}

static void misty_TRAINER(activeHand& effectedHand) {

	effectedHand.playerActiveBench.push_back(POKEMONCARDS.at(genRandomNum(0, POKEMONCARDS.size() - 1)));

}

static void energyRestore_TRAINER(activeHand& effectedHand) {

	int counter = 0;

	for (int i = 0; i < 3; i++) {

		if (genRandomNum(0, 1) == 0) { counter++; effectedHand.activeCard.currEnergyTypes.push_back(getRandEnergy()); }

	}

	std::cout << "\nYou got " << counter << " new energy cards!\n\n";

}


std::vector<trainerCard> TRAINERCARDSSUPPORT {


 { "Brock", "Add 3 pokemon to bench", brock_TRAINER },

 { "Misty", "Add random pokemon to bench", misty_TRAINER },

 { "Energy Restore", "Flip 3 coins, for each heads get an energy card", energyRestore_TRAINER },


};



static void healPotion_TRAINERITEM(activeHand& effectedHand) {

	regCard& activeCard = effectedHand.activeCard;

	if (activeCard.regCardHealth + 40 >= activeCard.legacyHP) {

		activeCard.regCardHealth = activeCard.legacyHP;

		effectedHand.activeCard.damageCounter = 0;

		std::cout << "\nHealth restored!\n\n";

		return;

	}

	effectedHand.activeCard.regCardHealth += 40;

	effectedHand.activeCard.damageCounter -= 40;

	std::cout << "\nHealth restored!\n\n";

}

static void WakeUp_TRAINERITEM(activeHand& currHand) {

	if (currHand.activeCard.isAsleep == true) {

		std::cout << "\n" << currHand.activeCard.regCardName << " woke up!\n";

		currHand.activeCard.isAsleep = false;

		return;

	}

	std::cout << "\n" << currHand.activeCard.regCardName << " wasn't asleep? what are you doing\n";

}


std::vector<trainerCard> TRAINERCARDSITEM { 

	{ "Heal Potion", "Restore 40 health for active Pokemon", healPotion_TRAINERITEM },

	{ "Wake Up", "Pokemon will wake up if asleep", WakeUp_TRAINERITEM },

};




//=====================CARD CREATION AND MANIPULATION========================

void playerHand::createCurrDeck() {

	int regCardDeckSize = genRandomNum(50, 52);

	int energyCardDeckSize = 60 - regCardDeckSize;

	for (int i = 0; i < regCardDeckSize; i++) {

		playerDeck.playerRegCards.push_back(POKEMONCARDS[genRandomNum(0, POKEMONCARDS.size() - 1)]);

	}

	for (int i = 0; i < genRandomNum(7, 11); i++) {

		playerDeck.playerEnergyCards.push_back(ENERGYCARDS[genRandomNum(0, ENERGYCARDS.size() - 1)]);

	}

	for (int i = 0; i < 3; i++) {

		playerTrainerCardsSupport.push_back(TRAINERCARDSSUPPORT[genRandomNum(0, TRAINERCARDSSUPPORT.size() - 1)]);

	}

	for (int i = 0; i < 5; i++) {

		playerTrainerCardsItems.push_back(TRAINERCARDSITEM[genRandomNum(0, TRAINERCARDSITEM.size() - 1)]);

	}

}


activeHand playerHand::createActiveHand() {

	regCard& blankCard = POKEMONCARDS[0];

	std::vector<regCard> blankBench;

	activeHand blankHAND(blankCard, blankBench);

	return blankHAND;

}


void playerHand::modifyActiveHand() {

	for (regCard& currCard : playerDeck.playerRegCards) {

		if (currCard.currEvolution == 0) {

			if (playerCurrHand.playerActiveBench.size() == 5) { break; }

			playerCurrHand.playerActiveBench.push_back(currCard);

		}

	}

	playerCurrHand.activeCard = getNREMOVErg(*this);

}


regCard playerHand::getNREMOVErg(playerHand& currHand) {

	std::cout << "\n=====CURRENT ENERGY CARDS IN HAND=====\nENERGIES: ";

	for (energyCard& currEn : playerDeck.playerEnergyCards) { std::cout << currEn.energyCardType << ", "; }

	std::cout << "\n\n";

	viewPlayerRegHand(currHand);

	int chosenCard = getCardIndex("Choose card to set active: ");

	regCard newActive = playerCurrHand.playerActiveBench.at(chosenCard);

	std::cout << "\nNew active card: " << newActive.regCardName << "\n\n";

	playerCurrHand.playerActiveBench.erase(playerCurrHand.playerActiveBench.begin() + chosenCard);

	return newActive;

}


void playerHand::rmEnergyCard(energyCard& destroyedCard) {

	for (int i = 0; i < playerDeck.playerEnergyCards.size(); i++) {

		if (playerDeck.playerEnergyCards[i].energyCardType == destroyedCard.energyCardType) {

			playerDeck.playerEnergyCards.erase(playerDeck.playerEnergyCards.begin() + i);

			break;

		}

	}

}





//==============FUNCTIONS FOR BATTLE OPTIONS=================

void playerHand::addEnergyToCard(regCard& currCard, int limiterCap) {

	int toLimit = 0;

	while (true) {

		if (toLimit == limiterCap) { break; }

		viewPlayerEnergyHand();

		int currEnergyIdx = getCardIndex("\n--TYPE -1 TO EXIT / -2 IF NO ENERGY\nChoose energy: ");

		if (currEnergyIdx == -1) { return; }
		//else if (currEnergyIdx == -2) { ifSacrifice(); return; }
		else if (currEnergyIdx == -2) { isOutOfEnergy = true; return; }


		energyCard& currEnergy = playerDeck.playerEnergyCards[currEnergyIdx];

		std::cout << "\nNew energy: " << currEnergy.energyCardType << "\n";

		currCard.currEnergyTypes.push_back(currEnergy.energyCardType);
		rmEnergyCard(currEnergy);

		std::cout << "\n\n";

		toLimit++;

	}

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


static void attackCard(regCard& currCard, regCard& attackedCard, regCardAttack& currCardAttack, int damageMultiplier) {

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


static regCardAttack& getCardAttack(regCard& currCard, const std::string displayedText) {

	currCard.viewAttacks();

	regCardAttack& currAttack = currCard.cardAttacks[getCardIndex(displayedText)];

	return currAttack;


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


static void rmStatusEffects(regCard& activeCard) {

	activeCard.isAsleep = false;
	activeCard.isBurned = false;
	activeCard.isParalyzed = false;
	activeCard.isPoisoned = false;

}


void playerHand::setActiveCard() {

	playerCurrHand.activeCard = getNREMOVErg(*this);

	setTrainer();

}


static bool checkifAllBlank(std::vector<regCard>& pActiveBench) {

	for (regCard& currCard : pActiveBench) {

		if (currCard.currCardAbility.abilityEffect != "" || currCard.cardAbilityUsed == true) { return false; }

	}

	return true;

}


static void useAbility(regCard& currCard, regCard& activeCard) {

	if (currCard.cardAbilityUsed == true) { throw std::invalid_argument(" \n CARD ABILITY ALREADY USED \n "); }

	currCard.cardAbilityUsed = true;

	currCard.applyCardAbility(activeCard, currCard);

}


static void chanceForEnergy(pHand& currHand) {

	if (genRandomNum(0, 2) == 1) {

		energyCard newEnergy(ENERGYCARDS[genRandomNum(0, ENERGYCARDS.size() - 1)]);

		std::cout << "You drew an energy card! New energy: " << newEnergy.energyCardType << "\n";

		currHand.playerEnergyCards.push_back(newEnergy);

	}

}










//========================BATTLE OPTIONS=============================

void playerHand::setTrainer() {

	if (playerTrainerCardsSupport.size() <= 0) { return; }

	std::cout << "\nPokemon was knocked out! choose new trainer\n";

	viewPlayerTrainerCardsSupport();

	short trainerIndex = getCardIndex("Choose trainer: ");

	trainerCard newTrainer = playerTrainerCardsSupport.at(trainerIndex);

	newTrainer.appTrainerEffect(playerCurrHand);

	playerCurrHand.activeTrainer = newTrainer;

	playerTrainerCardsSupport.erase(playerTrainerCardsSupport.begin() + trainerIndex);

}



void playerHand::drawCard() {

	if (playerCurrHand.playerActiveBench.size() >= 4) { chanceForEnergy(playerDeck); return; }

	regCard& newCard = playerDeck.playerRegCards.at(0);

	std::cout << "You drew a card from your deck, new card is " << newCard.regCardName << "; " << newCard.regCardType << " type!\n\n";

	playerCurrHand.playerActiveBench.push_back(newCard);

	playerDeck.playerRegCards.erase(playerDeck.playerRegCards.begin() + 0);

}



void playerHand::useItem() {

	viewPlayerTrainerCardsItems();

	int currItemIDX = getCardIndex("\n--TYPE -1 TO EXIT--\nChoose item to use: ");

	if (currItemIDX == -1) { return; }

	playerTrainerCardsItems.at(currItemIDX).appTrainerEffect(playerCurrHand);

	playerTrainerCardsItems.erase(playerTrainerCardsItems.begin() + currItemIDX);

}



void playerHand::useCardAbility() {

	if (checkifAllBlank(playerCurrHand.playerActiveBench)) { return; }

	std::cout << "\n--Use ability--\n\n";

	short currIdx = 0;

	for (regCard& currCard : playerCurrHand.playerActiveBench) {

		if (currCard.cardAbilityUsed == true || currCard.currCardAbility.abilityEffect == "") { currIdx++; continue; }

		std::cout << currIdx << ": " << currCard.regCardName << ", Ability: " << currCard.currCardAbility.abilityEffect << "\n";

		currIdx++;

	}

	short currCardIdx = getCardIndex("\n--TYPE -1 TO EXIT--\nChoose card to use ability: ");

	if (currCardIdx == -1) { return; }

	regCard& currC = playerCurrHand.playerActiveBench.at(currCardIdx);

	useAbility(currC, playerCurrHand.activeCard);

}



void playerHand::Retreat() {

	short uChoice = 1;

	std::cout << "\nTYPE 0 TO EXIT, 1 TO RETREAT: ";
	std::cin >> uChoice;

	if (uChoice == 0) { std::cout << "\n\n"; return; }

	regCard& oldActiveCard(playerCurrHand.activeCard);

	rmStatusEffects(oldActiveCard);

	std::cout << "\n" << playerCurrHand.activeCard.regCardName << " retreated!, choose new card to set active\n\n";

	regCard newActiveCard = getNREMOVErg(*this);

	playerCurrHand.playerActiveBench.push_back(oldActiveCard);

	playerCurrHand.activeCard = newActiveCard;

}



void playerHand::attackPlayer(playerHand& otherHand) {

	regCard& currCard = playerCurrHand.activeCard;

	if (currCard.isAsleep == true) { currCard.isAsleep = false;  return; }

	regCard& attackedCard = otherHand.playerCurrHand.activeCard;

	viewPlayerActiveCardInfo(currCard);


	addEnergyToCard(currCard, 2);


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


//================NOT IN USE======================

/*void playerHand::ifSacrifice() {

	std::cout << "\n";

	viewPlayerRegHand(*this);

	int cIdx = getCardIndex("Choose card to sacrifice for energy: ");

	playerCurrHand.playerActiveBench.erase(playerCurrHand.playerActiveBench.begin() + cIdx);

	std::string typewanted;

	for (int i = 0; i < 2; i++) {

		std::cout << "\nType wanted energy: ";
		std::cin >> typewanted;

		playerCurrHand.activeCard.currEnergyTypes.push_back(typewanted);

	}

} */

//================================================================










//====================dev=========================

void playerHand::devModeAddAllCards() {

	for (auto& card : POKEMONCARDS) {

		card.currEnergyTypes = { "darkness", "fire", "lightning", "water", "grass", "fighting", "psychic", "metal", "darkness", "fire", "lightning", "water", "grass", "fighting", "psychic", "metal", "darkness", "fire", "lightning", "water", "grass", "fighting", "psychic", "metal" };

		playerCurrHand.playerActiveBench.push_back(card);

	}

}






//==============SHITTY AI===============

static int getBestCardIDX(std::vector<regCard>& currBench) {

	std::vector<int> cardsHealth;

	for (regCard& currCard : currBench) {

		cardsHealth.push_back(currCard.regCardHealth);

	}

	int highestHP = *std::max_element(cardsHealth.begin(), cardsHealth.end());
	int cardIndexWithHighest = 0;

	for (int i = 0; i < cardsHealth.size(); i++) {

		if (cardsHealth[i] == highestHP) { cardIndexWithHighest = i; break; }

	}

	return cardIndexWithHighest;

}

void playerHand::createAiHand() {

	for (regCard& currCard : playerDeck.playerRegCards) {

		if (currCard.currEvolution == 0) {

			if (playerCurrHand.playerActiveBench.size() == 5) { break; }

			playerCurrHand.playerActiveBench.push_back(currCard);

		}

	}

	int bestIDX = getBestCardIDX(playerCurrHand.playerActiveBench);

	regCard newActive = playerCurrHand.playerActiveBench.at(bestIDX);

	playerCurrHand.activeCard = newActive;

	playerCurrHand.playerActiveBench.erase(playerCurrHand.playerActiveBench.begin() + bestIDX);

}




//==================GOOD AI======================

regCard activeHand::getRandCard() {

	return POKEMONCARDS.at(genRandomNum(0, POKEMONCARDS.size() - 1));

}

std::vector<trainerCard> activeHand::createTrainerDeckForAI() {

	std::vector<trainerCard> tCards;

	for (int i = 0; i < 4; i++) {

		tCards.push_back(TRAINERCARDSITEM.at(genRandomNum(0, TRAINERCARDSITEM.size() - 1)));

	}

	return tCards;

}



static regCard getRActiveImpossible() {

	std::vector<regCard> possible{

		POKEMONCARDS.at(36),

		POKEMONCARDS.at(39),

	};

	return possible.at(genRandomNum(0, possible.size() - 1));

}

static regCard getRActiveHard() {

	std::vector<regCard> possible{

		POKEMONCARDS.at(34),

		POKEMONCARDS.at(22),

	};

	return possible.at(genRandomNum(0, possible.size() - 1));

}

static regCard getRActiveMedium() {

	std::vector<regCard> possible{

		POKEMONCARDS.at(15),

	};

	return possible.at(genRandomNum(0, possible.size() - 1));

}

static regCard getRActiveEasy() {

	std::vector<regCard> possibilities{

		POKEMONCARDS.at(2),

		POKEMONCARDS.at(5),

	};

	return possibilities.at(genRandomNum(0, possibilities.size() - 1));

}



static std::vector<regCard> createAcImpossible(regCard& activeCard) {

	std::vector<regCard> charizardDeck{ POKEMONCARDS.at(38), POKEMONCARDS.at(42), POKEMONCARDS.at(43), POKEMONCARDS.at(16), POKEMONCARDS.at(30) };
	std::vector<regCard> kommoDeck{ POKEMONCARDS.at(40), POKEMONCARDS.at(41), POKEMONCARDS.at(37), POKEMONCARDS.at(35), POKEMONCARDS.at(14) };

	if (activeCard.regCardName == "Charizard") { return charizardDeck; }

	if (activeCard.regCardName == "Kommo-o") { return kommoDeck; }

	return charizardDeck;

}

static std::vector<regCard> createAcHard(regCard& activeCard) {

	std::vector<regCard> tyranDeck{ POKEMONCARDS.at(29), POKEMONCARDS.at(10), POKEMONCARDS.at(12), POKEMONCARDS.at(25), POKEMONCARDS.at(32) };
	std::vector<regCard> mewtwoDeck{ POKEMONCARDS.at(10), POKEMONCARDS.at(25), POKEMONCARDS.at(29), POKEMONCARDS.at(33), POKEMONCARDS.at(16) };

	if (activeCard.regCardName == "Tyranitar") { return tyranDeck; }

	if (activeCard.regCardName == "Mewtwo") { return mewtwoDeck; }

	return tyranDeck;

}

static std::vector<regCard> createAcMedium(regCard& activeCard) {

	std::vector<regCard> machampDeck{ POKEMONCARDS.at(14), POKEMONCARDS.at(16), POKEMONCARDS.at(19), POKEMONCARDS.at(32), POKEMONCARDS.at(34) };

	if (activeCard.regCardName == "Machamp") { return machampDeck; }

	return machampDeck;

}

static std::vector<regCard> createAcEasy(regCard& activeCard) {

	std::vector<regCard> bulbasaurDeck{ POKEMONCARDS.at(4), POKEMONCARDS.at(9), POKEMONCARDS.at(14), POKEMONCARDS.at(16), POKEMONCARDS.at(25) };
	std::vector<regCard> fuecocoDeck{ POKEMONCARDS.at(7), POKEMONCARDS.at(9), POKEMONCARDS.at(30), POKEMONCARDS.at(1), POKEMONCARDS.at(3) };

	if (activeCard.regCardName == "Bulbasaur") { return bulbasaurDeck; }

	if (activeCard.regCardName == "Fuecoco") { return fuecocoDeck; }

	return bulbasaurDeck;

}



activeHand activeHand::createAiCharacter() {

	std::cout << "\nWelcome to Pokemon AI match! choose your difficulty(easy / medium / hard / impossible): ";

	std::string eMhI;
	std::cin >> eMhI;


	if (eMhI.at(0) == 'e') { 
		
		regCard active = getRActiveEasy(); 
		
		std::vector<regCard> activebench = createAcEasy(active);

		return activeHand(active, activebench);

	}


	if (eMhI.at(0) == 'm') {

		regCard active = getRActiveMedium();

		std::vector<regCard> activebench = createAcMedium(active);

		return activeHand(active, activebench);

	}


	if (eMhI.at(0) == 'h') {

		regCard active = getRActiveHard();

		std::vector<regCard> activebench = createAcHard(active);

		return activeHand(active, activebench);

	}


	if (eMhI.at(0) == 'i') {

		regCard active = getRActiveImpossible();

		std::vector<regCard> activebench = createAcImpossible(active);

		return activeHand(active, activebench);

	}


	regCard active = getRActiveEasy();

	std::vector<regCard> activebench = createAcEasy(active);

	return activeHand(active, activebench);

}