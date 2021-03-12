#include "Player.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Player::Player()
{
	this->posX = 0;
	this->posY = 0.25;
	this->posZ = 0;
	this->toJump = false;
	this->currentDistance = 0;
	this->distanceJump = 3.0f;
	this->angularStep = 0;
}

// metoda ce intoarce in functie de pozitia jucatorului indexul coloanei
// din harta pe care se afla
int Player::GetIndexPlayer() {
	if ((this->posX >= 3) && (this->posX <= 5)) {
		return 4;
	}
	else if ((this->posX >= 1) && (this->posX <= 3)) {
		return 3;
	}
	else if ((this->posX >= -1) && (this->posX <= 1)) {
		return 2;
	}
	else if ((this->posX >= -3) && (this->posX <= -1)) {
		return 1;
	}
	else if ((this->posX >= -5) && (this->posX <= -3)) {
		return 0;
	}
	else {
		return -1;
	}
}
	
// metoda de randare a jucatorului
glm::mat4 Player::RenderPlayer(float deltaTimeSeconds, bool firstPerson, bool GameOver, Tema::Camera * camera) {
	glm::mat4 modelMatrix = glm::mat4(1);
	// pentru rotirea in permanenta a jucatorului de a da ideea de miscare
	this->angularStep += deltaTimeSeconds * 4.0f;
	
	// daca jocul s-a terminat atunci trebuie sa se realizeze animatia de cadere
	// deci jucatorul va fi translat in jos la fel si camera daca ne aflam pe 
	//modul firstPerson
	if (GameOver) {
		this->posY -= deltaTimeSeconds * 2.0f;
		if (firstPerson) {
			camera->TranslateUpword(-1.f * deltaTimeSeconds * 2.0f);
		}
		if (this->posY < -5) {
			cout << "GAME OVER!";
			exit(0);
		}
	}

	// daca s-a apasat SPACE atunci jucatorul trebuie sa sara;
	// am setat o distanta a sariturii pe care jucatorul trebuie sa 
	// o parcurga
	if (this->toJump) {
		// daca distana curenta parcura este mai mica decat jumatate din
		// distanta sariturii atunci jucatorul trebuie translat in sus
		// la fel si camera daca ne afla pe modul firstPerson
		if (this->currentDistance <= (this->distanceJump * 0.5f)) {
			this->posY += deltaTimeSeconds * 2.0f;
			this->currentDistance += deltaTimeSeconds * 2.0f;
			if (firstPerson) {
				camera->TranslateUpword(deltaTimeSeconds * 2.0f);
			}
		}
		// in caz contrar jucatorul este translatat in jos la fel si camera
		// daca suntem pe modul firstPerson
		else if ((this->currentDistance >= (this->distanceJump * 0.5f)) && (this->currentDistance <= this->distanceJump)) {
			this->posY -= deltaTimeSeconds * 2.0f;
			this->currentDistance += deltaTimeSeconds * 2.0f;
			if (firstPerson) {
				camera->TranslateUpword(-1.f * deltaTimeSeconds * 2.0f);
			}
		}
		// daca s-a parcurs toata distanta sariturii repozitionez jucatorul pe Oy
		else {
			this->posY = 0.25f;
			this->toJump = false;
			this->currentDistance = 0;
		}
	}

	modelMatrix = glm::translate(modelMatrix, glm::vec3(this->posX, this->posY, this->posZ));
	modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(1, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	return modelMatrix;
}

Player::~Player()
{
	
}