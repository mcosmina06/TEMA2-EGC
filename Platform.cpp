#include "Platform.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
using namespace std;
Platform::~Platform()
{
}

// constructorul platformei
Platform::Platform(int i)
{
	// in functie de coloana din matrice, pozitionez platforma pe Ox
	if (i == 0) {
		this->posX = -4;
	}
	else if (i == 1) {
		this->posX = -2;
	}
	else if (i == 2) {
		this->posX = 0;
	}
	else if (i == 3) {
		this->posX = 2;
	}
	else {
		this->posX = 4;
	}

	this->posY = -0.1f;
	this->posZ = 0;
	this->isHit = false;
	this->isPowerUp = false;
	this->distance = 0;
}

// se calculeaza centrul platformei pe Oz in functie de centrul si lungimea
// platformei anterioare
void Platform::computePosZ(float posZ_ant, float length_ant) {
	this->posZ = posZ_ant - (length_ant * 0.5f) - (this->length * 0.5f) - this->distance;
}

// generarea valorilor caracteristice fiecarei placute
void Platform::generatePlatform(bool isFirst, bool isPowerUp)
{	
	this->isPowerUp = isPowerUp;

	// dacca placuta este setata ca "first" atunci va avea o lungime mai lunga si
	// nu va xista dintanta intre placi iar culoarea va fi 0
	if (isFirst) {
		this->color = glm::vec3(0, 0, 1); //pentru a evita posibilitatile de a avea o platforma cu dezavantaj prima
		this->colorIndex = 4;
		this->length = (float)(rand() % ((20 - 15) + 1) + 15); //pun platforme mai lungi pentru primul rand
	}
	else {
		// daca nu este "first" dar poate fi o platforma cu avantaj/dezavantaj
		// atunci generez random un intreg si asociez fiecarui int de la 0 la 4 o culoare
		if (this->isPowerUp) {
			int color = rand() % 5;
			if (color == 0) {
				this->color = glm::vec3(1, 0.62, 0.12); // portocaliu
				this->colorIndex = 0;
			}
			else if (color == 1) {
				this->color = glm::vec3(1, 0, 0); // rosu
				this->colorIndex = 1;
			}
			else if (color == 2) {
				this->color = glm::vec3(0.98f, 1, 0.08); // galben
				this->colorIndex = 2;
			}
			else if (color == 3) {
				this->color = glm::vec3(0, 1, 0); // verde
				this->colorIndex = 3;
			}
			else {
				this->color = glm::vec3(0, 0, 1); // albastru
				this->colorIndex = 4;
				this->isPowerUp = false;
			}
		}
		//daca este o platforma simpla va avea culoarea albastra
		else {
			this->color = glm::vec3(0, 0, 1);
			this->colorIndex = 4;
		}
		this->length = (float)(rand() % ((15 - 3) + 1) + 3);
		this->distance = (float)(rand() % 6);
	}
}