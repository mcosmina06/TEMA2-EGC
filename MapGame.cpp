#include "MapGame.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// harta jocului este reprezenta din 5 coloane ce trebuiesc initializate
// atunci cand este apelat constructorul;
MapGame::MapGame() {

	for (int index = 0; index < 5; index++) {
		// primele 2 placi de pe fiecare coloana le voi seta ca "first"
		// pentru a nu avea distanta intre ele, a fi mai lungi si sa fie
		// platforme simple pentru a facilita jocul la inceput
		this->platforms[index].push_back(new Platform(index));
		this->platforms[index][0]->generatePlatform(true, false);
		
		this->platforms[index].push_back(new Platform(index));
		this->platforms[index][1]->generatePlatform(true, false);

		for (int i = 2; i < 6; i++) {
			this->platforms[index].push_back(new Platform(index));
			this->platforms[index][i]->generatePlatform(false, false);
			this->platforms[index][i]->computePosZ(this->platforms[index][i - 1]->posZ, this->platforms[index][i - 1]->length);
		}
	}
}

// metoda de actualizare a fiecarei coloane la un moment de timp
void MapGame::ComputeColumn(int index, float deltaTimeSeconds) {

	// calculez endul primei platforma din coloana, daca acesta este mai mare ca 0
	// atunci placuta a iesit din cadru si poate fi stearsa si adauga una noua
	float end = this->platforms[index][0]->posZ - (this->platforms[index][0]->length * 0.5f);
	if (end > 0) {
		// tin constant maxim 2 platforme cu avantaje/dezavantaje pe fiecare coloana
		// astfel verific daca platforma stearsa a fost una simpla sau nu pentru a 
		// actualiza numarul de platforma cu avantaje/dezavantaje petru fiecare coloana
		if (this->platforms[index][0]->isPowerUp) {
			this->nbPowerUps[index]--;
		}
		//sterg prima platforma
		platforms[index].erase(platforms[index].begin());
		//si adaug una noua
		this->platforms[index].push_back(new Platform(index));
		// inainte de generarea valorilor random a platformei verific daca o pot
		// face una cu avantaj/dezavantaj
		if (this->nbPowerUps[index] < 2) {
			this->nbPowerUps[index]++;
			this->platforms[index][this->platforms[index].size() - 1]->generatePlatform(false, true);
		}
		else {
			this->platforms[index][this->platforms[index].size() - 1]->generatePlatform(false, false);
		}
		// ii calzulez platformei nou adaugate centrul pe Oz in functie de platforma anterioara
		this->platforms[index][this->platforms[index].size() - 1]->computePosZ(this->platforms[index][this->platforms[index].size() - 2]->posZ,
			this->platforms[index][platforms[index].size() - 2]->length);
	}
}

MapGame::~MapGame()
{
}