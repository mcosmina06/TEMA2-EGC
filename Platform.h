#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

class Platform
{
public:
	~Platform();
	Platform::Platform(int i);
	void Platform::computePosZ(float posZ_ant, float length_ant);
	void Platform::generatePlatform(bool isFirst, bool isPowerUp);

	float posX;
	float posY;
	float posZ;
	float length; //lungimea platformei
	float distance; //distanta dintre placi
	int colorIndex;
	glm::vec3 color; //5 culori ( 0 = portocaliu, 1 = rosu, 2 = galben, 3 = verde, 4 = albastru
	bool isHit; 
	bool isPowerUp;

	void foo() {

	}
};