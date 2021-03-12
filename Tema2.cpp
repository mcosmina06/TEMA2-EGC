#include "Tema2.h"
#include "UserInterface.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera = new Tema::Camera();
	// setez initial camera pe third person
	camera->Set(glm::vec3(0, 1, 3), glm::vec3(0, 0.7f, 0), glm::vec3(0, 1, 0));

	// cele 2 mesh-uri pentru interfata cu utilizatorul
	Mesh* fuelBar = UserInterface::CreateRectangle1("fuelBar", glm::vec3(0, 0, 0), 1.0f, 0.1f, glm::vec3(1, 0, 0), true);
	AddMeshToList(fuelBar);

	Mesh* fuelBarBack = UserInterface::CreateRectangle1("fuelBarBack", glm::vec3(0, 0, 0), 2.0f, 0.1f, glm::vec3(1, 1, 1), true);
	AddMeshToList(fuelBarBack);
	
	InitGame();

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	{
		Shader* shader = new Shader("Tema2");
		shader->AddShader("Source/Laboratoare/Tema2.2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2.2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

// initializez toate variabilele si instantele pentru joc
void Tema2::InitGame() {
	player = new Player();
	map = new MapGame();
	
	block = false;
	W_pressed = false;
	S_pressed = false;
	firstPerson = false;
	thirdPerson = true;
	GameOver = false;

	max_speed = 1.0f;
	blockTime = 0;
	scaleFuelOX = 2;
	speedFuelConsumption = 0.05f;
}

// functie de randare a platformelor, parcurge fiecare element de pe coloana index
void Tema2::RenderPlatforms(float deltaTimeSeconds) {
	
	for (int index = 0; index < 5; index++) {
		
		// pentru fiecare coloana din matrice apelez urmatoarea metoda
		// pentru a se updata corespunzator inainte de randare
		map->ComputeColumn(index, deltaTimeSeconds);
		
		for (int i = 0; i < map->platforms[index].size(); i++) {
			// daca jocul nu s-a terminat continui miscarea platformelor
			if (!GameOver) {
				// daca jucatorul a atins o platforma portocalie, deplasez platformele
				// cu o viteza mai mare
				if (block) {
					map->platforms[index][i]->posZ += deltaTimeSeconds * 50;
				}
				else {
					map->platforms[index][i]->posZ += deltaTimeSeconds * max_speed;
				}
			}

			// pozitionarea si scalarea corespunzatoare fiecarei placute
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(map->platforms[index][i]->posX, map->platforms[index][i]->posY, map->platforms[index][i]->posZ));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, map->platforms[index][i]->length));
			
			// daca platforma este marca ca isHit atunci o fac mov
			if (map->platforms[index][i]->isHit) {
				RenderSimpleMesh(meshes["box"], shaders["Tema2"], modelMatrix, glm::vec3(1, 0, 1));
			}
			else {
				RenderSimpleMesh(meshes["box"], shaders["Tema2"], modelMatrix, map->platforms[index][i]->color);
			}
			map->platforms[index][i]->isHit = false;
		}
	}
}

// functie de verificare a coliziunilor
void Tema2::checkCollision() {
	int i = 0;
	// verific pe ce platforma se afla jucatorul
	i = player->GetIndexPlayer();
	// daca i este -1 inseamna ca jucatorul se afla in afara hartii jocului
	if (i == -1) {
		GameOver = true;
	}

	// parcurg fiecare platforma de pe coloana corespunzatoare pozitiei jucatorului
	for (int j = 0; j < map->platforms[i].size() - 1; j++) {
		
		// daca jucatorul nu sare atunci verific coliziunile
		if (!player->toJump) {
			{
				// calculez pozitia de de start si end a platformei curente in functie de centru si lungime
				float start = map->platforms[i][j]->posZ + (map->platforms[i][j]->length * 0.5f);
				float end = map->platforms[i][j]->posZ - (map->platforms[i][j]->length * 0.5f);

				// daca jucatorul se afla intre cele 2 limite atunci platforma este atinsa
				if (player->posZ <= start && player->posZ >= end) {
					map->platforms[i][j]->isHit = true;
					// verific daca sunt platforme cu dezavantaj/avantaj dupa codul culorii atribuit
					if (map->platforms[i][j]->colorIndex == 0) {
						// daca este portocalie atunci setez perioada de blocare a jucatorului si
						// fac true booleana block pentru a sti sa modific speed-ul placilor
						blockTime = 5;
						block = true;
					}
					else if (map->platforms[i][j]->colorIndex == 1) {
						// daca este rosie jocul se termina instant
						cout << "GAME OVER!" << endl;
						exit(0);
					}
					else if (map->platforms[i][j]->colorIndex == 2) {
						// daca este galbena jucatorul va pierde fuelCapacity din combustibil
						// cat timp sta pe aceasta platforma pierde mai mult
						fuelCapacity -= 0.015f;
					}
					else if (map->platforms[i][j]->colorIndex == 3) {
						// daca este verde jucatorul va castiga fuelCapacity din combustibil
						// cat timp sta pe aceasta platforma castiga mai mult
						fuelCapacity += 0.015f;
					}
				}
			}

			{
				// verific daca jucatorul a cazut
				// calculez endul platformei curente si startul urmatoarei platforme
				float start = map->platforms[i][j + 1]->posZ + (map->platforms[i][j + 1]->length * 0.5f);
				float end = map->platforms[i][j]->posZ - (map->platforms[i][j]->length * 0.5f);
				
				// daca se afla intre aceste 2 limite inseamna ca jucatorul a cazut si jocul
				// se va termina
				if (player->posZ >= start && player->posZ <= end) {
					GameOver = true;
				}
			}
		}
	}
}

void Tema2::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

// interfata cu utilizatorul este reprezentata prin 2 dreptunghiuri 2D
void Tema2::UserInterface(float deltaTimeSeconds) {
	// variabila scaleFuelOx este setata initial la valoarea de scalare maxima si
	// scade pe parcursul jocului
	scaleFuelOX -= deltaTimeSeconds * speedFuelConsumption;
	// adun si fuelCapacity in caz ca a fost modificata valoarea si ma asigur
	// ca este 0 pe urma
	scaleFuelOX += fuelCapacity;
	fuelCapacity = 0;
	
	// daca s-a recuperat mai mult combustibil decat valoarea maxima stabilita
	// nu se v-a scala mai mult dreptunghiul
	if (scaleFuelOX > 2) {
		scaleFuelOX = 2;

	}
	// daca s-a pierdut din combustibil, s-a ajuns cu o valoare negativa la scalare
	// atunci il fac 0 pentru a sti ca jocul s-a incheiat
	if (scaleFuelOX < 0) {
		scaleFuelOX = 0;
	}
	
	// in functie de variabilele thirdPerson si firstPerson stiu unde sa pozitionez
	// corect cele 2 drepunghiuri
	if (thirdPerson) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX + 0.5f, 2, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFuelOX, 1, 1));
		RenderMesh(meshes["fuelBar"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX + 0.5f, 2, 0));
		RenderMesh(meshes["fuelBarBack"], shaders["VertexColor"], modelMatrix);
	}
	else {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX + 0.2f, player->posY + 1.25f, -3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFuelOX, 1, 1));
		RenderMesh(meshes["fuelBar"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(player->posX + 0.2f, player->posY + 1.25f, -3));
		RenderMesh(meshes["fuelBarBack"], shaders["VertexColor"], modelMatrix);
	}
}


void Tema2::Update(float deltaTimeSeconds)
{
	// daca nu mai este combustibil jocul se termina
	if (scaleFuelOX == 0) {
		cout << "GAME OVER! YOU RUN OUT OF FUEL!" << endl;
		exit(0);
	}
	// daca jucatorul este bloat atunci nu mai verific coliziunile si
	// jucatorul nu poate sa isi modifice singur viteza
	if (block) {
		if (blockTime < 0) {
			block = false;
		}
		else if (blockTime >= 0) {
			blockTime -= deltaTimeSeconds;
		}
	}
	// daca nu este blocat la o viteza maxima
	else {
		// verific apasarea tastelor corespunzatoare pentru cresterea/scaderea vitezei
		if (W_pressed) {
			if (max_speed < 10) {
				max_speed += deltaTimeSeconds;
			}
		}
		else if (S_pressed) {
			if (max_speed > 4) {
				max_speed -= deltaTimeSeconds;
			}
		}
		checkCollision();
	}
	if (!GameOver) {
		UserInterface(deltaTimeSeconds);
	}
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], player->RenderPlayer(deltaTimeSeconds, firstPerson, GameOver, camera));
	RenderPlatforms(deltaTimeSeconds);
}

void Tema2::FrameEnd()
{
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		W_pressed = true;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		S_pressed = true;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		if (!block) {
			camera->TranslateRight(-deltaTime * 2.0f);
			player->posX += -deltaTime * 2.0f;
		}
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		if (!block) {
			camera->TranslateRight(deltaTime * 2.0f);
			player->posX += deltaTime * 2.0f;
		}
	}

	if (window->KeyHold(GLFW_KEY_SPACE)) {
		if (!GameOver) {
			player->toJump = true;
		}
	}

}

void Tema2::OnKeyPress(int key, int mods)
{
	// schimbarea modului de viziualizare a jocului
	if (key == GLFW_KEY_C) {
		if (thirdPerson) {
			thirdPerson = false;
			firstPerson = true;
			camera->Set(glm::vec3(0, 0.3f, -0.25f), glm::vec3(0, 0.3f, -1), glm::vec3(0, 1, 0));
			camera->TranslateRight(player->posX);
		}
		else {
			thirdPerson = true;
			firstPerson = false;
			camera->Set(glm::vec3(0, 1, 3), glm::vec3(0, 0.7f, 0), glm::vec3(0, 1, 0));
			camera->TranslateRight(player->posX);
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
