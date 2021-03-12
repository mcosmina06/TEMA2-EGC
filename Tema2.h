#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"
#include "Platform.h"
#include "Player.h"
#include "Tema2.h"
#include "UserInterface.h"
#include "MapGame.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;
		float FOV = 90;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		
		void InitGame();
		void checkCollision();
		void UserInterface(float deltaTimeSeconds);
		void RenderPlatforms(float deltaTimeSeconds);
		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Tema::Camera *camera;
		Player* player;
		MapGame* map;
		glm::mat4 projectionMatrix;

		bool block;
		bool W_pressed;
		bool S_pressed;
		bool firstPerson;
		bool thirdPerson;
		bool GameOver;

		float max_speed;
		float blockTime;
		float fuelCapacity;
		float scaleFuelOX;
		float speedFuelConsumption;
};
