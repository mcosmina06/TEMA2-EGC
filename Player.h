#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"

class Player
{
public:
	Player();
	int Player::GetIndexPlayer();
	glm::mat4 Player::RenderPlayer(float deltaTimeSeconds, bool firstPerson, bool GameOver, Tema::Camera* camera);
	~Player();

	float posX;
	float posY;
	float posZ;
	bool toJump;
	float currentDistance;
	float distanceJump;
	float angularStep;

	void foo() {

	}
};