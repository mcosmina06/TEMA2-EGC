#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "Platform.h"


class MapGame
{
public:
	~MapGame();
	MapGame::MapGame();
	void MapGame::ComputeColumn(int index, float deltaTimeSeconds);
	
	std::vector <Platform*> platforms[5];
	int nbPowerUps[5] = { 0, 0, 0, 0, 0 };
	void foo() {

	}
};