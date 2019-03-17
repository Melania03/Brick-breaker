
#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "TransformIn2D.h"
#include "Obj2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

int beforeClick_x, afterClick_x, vietiJoc = 3, nrcaramizicazute = 0, nrcaramizilovite = 0, lungimeplaca = 150, finaljoc = 0, asteptare = 0;
const int coloane = 10, linii = 6;
bool click = false, final = false, finalvieti = false;

double bilay, bilax, movestarfin, rotatestar;
double vitezax, vitezay, vitezastea, factorViteza, varTimp = 0;

struct caramizi {
	double x1;
	double x2;
	double y1;
	double y2;
	int lives;
	double scale;
};

struct stele {
	double x;
	double y;
	int active;
	int tip;
	double rotate;
};

caramizi m[coloane][linii];
stele n[coloane][linii];


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	vitezax = 0;
	vitezay = 400;
	vitezastea = 450;


	Mesh* perete1 = Obj2D::CreateSquare("perete1", corner, 20, 650, glm::vec3(0.1, 0.7, 0.1), true);
	AddMeshToList(perete1);

	Mesh* perete2 = Obj2D::CreateSquare("perete2", corner, 1300, 20, glm::vec3(0.1, 0.7, 0.1), true);
	AddMeshToList(perete2);

	Mesh* perete3 = Obj2D::CreateSquare("perete3", corner, 20, 650, glm::vec3(0.1, 0.7, 0.1), true);
	AddMeshToList(perete3);

	Mesh* placa = Obj2D::CreateSquare("placa", corner, lungimeplaca, 10, glm::vec3(0.144, 0.63, 0.139), true);
	AddMeshToList(placa);

	Mesh* caramida = Obj2D::CreateSquare("caramida", corner, 60, 30, glm::vec3(0.74, 0.247, 0), true);
	AddMeshToList(caramida);

	Mesh* caramida2 = Obj2D::CreateSquare("caramida2", corner, 60, 30, glm::vec3(0.9, 0.3, 0), true);
	AddMeshToList(caramida2);

	Mesh* bila = Obj2D::CreateCircle("bila", corner, 9, glm::vec3(1, 1, 1), true);
	AddMeshToList(bila);

	Mesh* bila1 = Obj2D::CreateCircle("bila1", corner, 15, glm::vec3(0.9, 0.9, 0.9), true);
	AddMeshToList(bila1);

	Mesh* starmov = Obj2D::CreateStar("starmov", corner, 15, 7.5, glm::vec3(0.5, 0.14, 0.46));
	AddMeshToList(starmov);

	Mesh* stargal = Obj2D::CreateStar("stargal", corner, 15, 7.5, glm::vec3(1, 0.8, 0));
	AddMeshToList(stargal);

	Mesh* starroz = Obj2D::CreateStar("starroz", corner, 15, 7.5, glm::vec3(0.99, 0.49, 0.612));
	AddMeshToList(starroz);

	Mesh* starfinal = Obj2D::CreateStar("starfinal", corner, 300, 150, glm::vec3(0.67, 0.07, 0.35));
	AddMeshToList(starfinal);


	//matricea pt caramizi si proprietatile active si rotate pt matricea de powerups
	for (int i = 0; i < coloane; i++)
		for (int j = 0; j < linii; j++) {
			if (i == 0 || i == coloane - 1 || j == 0 || j == linii - 1)
				m[i][j].lives = 2;
			else
				m[i][j].lives = 1;
			m[i][j].x1 = (resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 80 * i;
			m[i][j].x2 = (resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 60 + 80 * i;
			m[i][j].y1 = resolution.y - 130 - 50 * j;
			m[i][j].y2 = resolution.y - 130 + 30 - 50 * j;
			m[i][j].scale = 1;
			n[i][j].active = 0;
			n[i][j].rotate = 1;

		}
	//tipul powerups
	for (int i = 0; i < coloane; i++) {
		for (int j = 0; j < linii; j++) {
			if (j % 2 == 0) {
				if (i % 3 == 0)
					n[i][j].tip = 1;
				if (i % 3 == 1)
					n[i][j].tip = 2;
				if (i % 3 == 2)
					n[i][j].tip = 3;
			}
			else {
				if (i % 3 == 0)
					n[i][j].tip = 2;
				if (i % 3 == 1)
					n[i][j].tip = 3;
				if (i % 3 == 2)
					n[i][j].tip = 1;
			}
		}
	}
		
}


void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.67, 0.80, 0.88, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();


	modelMatrix = glm::mat3(1);
	modelMatrix *= TransformIn2D::Translate(0, 70);
	RenderMesh2D(meshes["perete1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= TransformIn2D::Translate(0, resolution.y - 20);
	RenderMesh2D(meshes["perete2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= TransformIn2D::Translate(resolution.x - 20, 70);
	RenderMesh2D(meshes["perete3"], shaders["VertexColor"], modelMatrix);


	modelMatrix = glm::mat3(1);
	if (click == false)
		modelMatrix *= TransformIn2D::Translate(beforeClick_x - lungimeplaca / 2, 20);
	else
		modelMatrix *= TransformIn2D::Translate(afterClick_x - lungimeplaca / 2, 20);
	RenderMesh2D(meshes["placa"], shaders["VertexColor"], modelMatrix);

	//caramizile in scena in timpul jocului
	if (final == false && finalvieti == false) {
		if (vietiJoc > 0) {
			for (int i = 0; i < coloane; i++)
				for (int j = 0; j < linii; j++) {
					modelMatrix = glm::mat3(1);
					if (m[i][j].lives == 1) {
						modelMatrix *= TransformIn2D::Translate((resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 80 * i, resolution.y - 130 - 50 * j);
						RenderMesh2D(meshes["caramida"], shaders["VertexColor"], modelMatrix);
					}
					else if (m[i][j].lives < 1 && m[i][j].scale > 0) {
						modelMatrix *= TransformIn2D::Translate((resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 80 * i + 30, resolution.y - 130 - 50 * j + 15);
						modelMatrix *= TransformIn2D::Scale(m[i][j].scale, m[i][j].scale);
						modelMatrix *= TransformIn2D::Translate(-30, -15);
						RenderMesh2D(meshes["caramida"], shaders["VertexColor"], modelMatrix);
						modelMatrix = glm::mat3(1);
							m[i][j].scale -= deltaTimeSeconds;

					}
					if (m[i][j].lives == 2) {
						modelMatrix *= TransformIn2D::Translate((resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 80 * i, resolution.y - 130 - 50 * j);
						RenderMesh2D(meshes["caramida2"], shaders["VertexColor"], modelMatrix);
					}


				}
		}
		else
			finalvieti = true;
		//powerups
		for (int i = 0; i < coloane; i++)
			for (int j = 0; j < linii; j++)
				if (n[i][j].active == 1) {

					modelMatrix = TransformIn2D::Translate(n[i][j].x, n[i][j].y);
					modelMatrix *= TransformIn2D::Rotate(n[i][j].rotate);
					if (n[i][j].tip == 1)
						RenderMesh2D(meshes["starmov"], shaders["VertexColor"], modelMatrix);
					else if (n[i][j].tip == 2)
						RenderMesh2D(meshes["stargal"], shaders["VertexColor"], modelMatrix);
					else
						RenderMesh2D(meshes["starroz"], shaders["VertexColor"], modelMatrix);
					n[i][j].rotate += deltaTimeSeconds * 2;
					if (n[i][j].tip == 1 || n[i][j].tip == 2)
						n[i][j].y -= vitezastea * deltaTimeSeconds;
					else
						n[i][j].y -= vitezastea * deltaTimeSeconds * 4 / 3;
					if (n[i][j].y <= 37.5 && ((n[i][j].x >= (afterClick_x - lungimeplaca / 2 - 3.75)) && (n[i][j].x <= (afterClick_x + lungimeplaca / 2 + 3.75)))) {
						n[i][j].active = 0;
						if (n[i][j].tip == 1 && vietiJoc < 3)
							vietiJoc++;
						else if (n[i][j].tip == 2)
							varTimp = 5;
						else if (n[i][j].tip == 3) {
							for (int a = 0; a < linii; a++)
								if (m[i][a].lives > 0)
									m[i][a].lives--;
							for (int b = 0; b < coloane; b++)
								if (m[b][j].lives > 0)
									m[b][j].lives--;
						}
					}
					if (n[i][j].y <= 0)
						n[i][j].active = 0;
				}



		modelMatrix = glm::mat3(1);
		if (click == false) {
			modelMatrix *= TransformIn2D::Translate(beforeClick_x, 50);
			RenderMesh2D(meshes["bila"], shaders["VertexColor"], modelMatrix);
			bilay = 39;
			bilax = afterClick_x;
		}
		else {
			if (bilay >= resolution.y - 30)
				vitezay = -vitezay; //coliziunea cu peretele de sus

			if ((bilax < 30 || bilax>resolution.x - 30) && bilay > 70)
				vitezax = -vitezax; //coliziunea cu peretii laterali

			if (varTimp > 0) {
				varTimp -= deltaTimeSeconds;
				factorViteza = 1.5;
			}
			else factorViteza = 1;

		//coliziunile cu caramizile
			bilay += vitezay * deltaTimeSeconds * factorViteza;
			bilax += vitezax * deltaTimeSeconds * factorViteza;
			if (vietiJoc > 0 || final == false) {
				for (int i = 0; i < coloane; i++)
					for (int j = 0; j < linii; j++) {
						if (m[i][j].lives > 0) {
							if (bilax + 9 >= m[i][j].x1  && bilax - 9 <= m[i][j].x2  && bilay + 9 >= m[i][j].y1 && bilay - 9 <= m[i][j].y2) {
								m[i][j].lives--;
								nrcaramizilovite++;
								if (m[i][j].lives == 0) {
									nrcaramizicazute++;
									if (nrcaramizicazute % 2 == 0) {
										n[i][j].active = 1;
										n[i][j].x = (resolution.x - (coloane - 1) * 20 - coloane * 60) / 2 + 80 * i + 30 + 7.5;
										n[i][j].y = resolution.y - 130 - 50 * j + 3.5;
									}
								}

								if (bilax >= m[i][j].x1 && bilax <= m[i][j].x2)
									vitezay = -vitezay;

								if (bilay >= m[i][j].y1 && bilay <= m[i][j].y2)
									vitezax = -vitezax;
								if (bilax < m[i][j].x1) {
									if (bilay < m[i][j].y1) {
										vitezax = -abs(vitezax);
										vitezay = -abs(vitezay);

									}
									if (bilay > m[i][j].y2) {
										vitezax = -abs(vitezax);
										vitezay = abs(vitezay);
									}
								}
								if (bilax > m[i][j].x2) {
									if (bilay < m[i][j].y1) {
										vitezax = abs(vitezax);
										vitezay = -abs(vitezay);

									}
									if (bilay > m[i][j].y2) {
										vitezax = abs(vitezax);
										vitezay = abs(vitezay);
									}
								}



							}
						}
					}
			}
			//coliziunea cu placa
			if (bilay <= 39) {
				if (bilax >= afterClick_x - lungimeplaca / 2 - 9 && bilax <= afterClick_x + lungimeplaca / 2 + 9) {
					float unghi = acos((bilax - afterClick_x) / 84);
					vitezax = cos(unghi) * 400;
					vitezay = abs(sin(unghi) * 400);
				}

				else {
					bilax = afterClick_x;
					bilay = 39;
					click = false;
					vietiJoc--;
					beforeClick_x = afterClick_x;
					vitezax = 0;
				}



			}
			modelMatrix *= TransformIn2D::Translate(bilax, bilay);
			RenderMesh2D(meshes["bila"], shaders["VertexColor"], modelMatrix);
		}
		//vietile din joc
		for (int i = 0; i < vietiJoc; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= TransformIn2D::Translate(50 + i * 40, 35);
			RenderMesh2D(meshes["bila1"], shaders["VertexColor"], modelMatrix);
		}

		for (int i = 0; i < coloane; i++)
			for (int j = 0; j < linii; j++)
				finaljoc += m[i][j].lives; 

		if (finaljoc == 0) { //verific daca toate caramizile au cazut
			final = true;
		}
		else
			finaljoc = 0;

	}

	if (final == true) { //scena dupa finalul jocului in care s-au distrus toate caramizile

		modelMatrix = TransformIn2D::Translate(resolution.x / 2, resolution.y / 2 + 40);
		modelMatrix *= TransformIn2D::Rotate(movestarfin);
		RenderMesh2D(meshes["starfinal"], shaders["VertexColor"], modelMatrix);
		if (varTimp < 3)
			movestarfin += deltaTimeSeconds * 1.8;
		else {
			movestarfin -= deltaTimeSeconds * 1.8;

		}
		varTimp += deltaTimeSeconds;

		if (varTimp > 6)
			varTimp = 0;


		modelMatrix = TransformIn2D::Translate(resolution.x - 140, resolution.y / 2 - 50);
		modelMatrix *= TransformIn2D::Scale(movestarfin, movestarfin);
		modelMatrix *= TransformIn2D::Rotate(rotatestar);
		RenderMesh2D(meshes["starroz"], shaders["VertexColor"], modelMatrix);

		rotatestar -= deltaTimeSeconds * 1.5;

		modelMatrix = TransformIn2D::Translate(140, resolution.y / 2 + 100);
		modelMatrix *= TransformIn2D::Scale(movestarfin, movestarfin);
		modelMatrix *= TransformIn2D::Rotate(rotatestar);
		RenderMesh2D(meshes["stargal"], shaders["VertexColor"], modelMatrix);

		modelMatrix = TransformIn2D::Translate(resolution.x / 2, resolution.y / 2 - 270);
		modelMatrix *= TransformIn2D::Scale(movestarfin, movestarfin);
		modelMatrix *= TransformIn2D::Rotate(rotatestar);
		RenderMesh2D(meshes["starmov"], shaders["VertexColor"], modelMatrix);


	}


}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (click == false) {
		beforeClick_x = mouseX;
		afterClick_x = beforeClick_x;
	}
	else
		afterClick_x = mouseX;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{


}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (vietiJoc > 0 || final == false) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && click == false) {
			click = true;
			vitezay = 400;
		}
	}
}



void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
