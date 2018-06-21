#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <imgui\imgui.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "GL_framework.h"
#include <vector>
#include <list>
#include "glm/ext.hpp"		//Libreria para imprimir con cout vec3 y mat4 


//Global variables

float _distanceWall = 0.8f;	//0.07										//Distancia entre el punto cental y las paredes 
float rotation_angle = 0.f;												//Variable, con la que podemos rotar los cubos a cierta velocidad 
bool CubeCanRotate = false;												//Si el cubo puede rotar
const int MaxCubes = 10;									//Warning: if all cubes are drawn in the same exact position, visualitzation could be wrong



//Forward declarations and namespaces

namespace randomPositions {
	void setNewRandPositions();									//Sets a random position for all cubes
	glm::vec3 arrayCubes[MaxCubes];
}
//Namespace para las rotaciones de los cubos 
namespace rotationCubes {
	glm::vec3 arrayRotationCubes[MaxCubes];
}
//Namespace para los valores de la Y cuando deban caer o no 
namespace Fall {
	const float maxFallDist = 9.f;
	const float fallSpeed = 0.02f;	//0.07
	float currentFallDist = 0;
}

namespace arrangedCubes {
	std::vector<glm::vec3> cubicLatticePositions;						//Cube lattice positions
	void arrangeCubesIntoLattice();
}

namespace Matrix {
	const float maxTimeLife = 3;
	struct objectTrail
	{
		float timeLife;
		glm::vec3 position;
		float rotation;
		glm::vec3 rotationAxis;
	};
	std::list<objectTrail> trailObjectsList;
	float rotation = 0;
	const float maxFallDist = 8.f;
	const float fallSpeed = 0.07f;
	float currentFallDist = 0;

	const float nextTrailObTime = _distanceWall * 1.72f;			//time until we add a new trail object
	const float objTrailMaxLife = nextTrailObTime * 4;			//time until a trail object is destroyed
	float nextTrailCounter = 0;					//Distance objects needs to fall so we can add new trail objects
}

namespace ImGui {
	void Render();
}

namespace Scene {
	int currentScene = 1;	//1, 2, 3...
	void renderScene1(double currentTime);
	void renderScene2(double currentTime);
	void renderScene3(double currentTime);
	void renderScene4(double currentTime);
	void renderScene5(double currentTime);
	void renderScene6(double currentTime);
	void detectInput();
}

namespace CubeShader {

	void ShaderInitCode();
	GLuint ShaderCompile();

	void ShaderCleanupCode(void);
	void ShaderRenderCode(double currentTime, int cubeN, float rotation_angle);
	void renderCubeInPos(glm::vec3 position, float rotation_angle, float sideLenght);

	GLuint ShaderRenderProgram;
	GLuint ShaderVAO;
}

namespace truncOctahedronShader {
	void ShaderInitCode();
	GLuint ShaderCompile(bool wireframe);
	void ShaderCleanupCode(void);
	void ShaderRenderWithRotation(bool wireframe, glm::vec3 otruncOctPos, int color=-1, float rotationRads = 0, glm::vec3 rotationAxis = glm::vec3(1));		//Color/Rotation/rotationAxis are optional parameters. Color can only be [0...6]

	GLuint ShaderRenderProgram;
	GLuint WireframeShaderRenderProgram;
	GLuint ShaderVAO;
	GLuint WireframeShaderVAO;

	float getHeight();

	//glm::vec4 truncatedOctTest = _square1;
	glm::vec4 tOctPositions[5] = { glm::vec4(30.0f, 30.0f, -30.0f, 1.0f) };
	glm::vec4 truncatedOctTest = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float sideLenght = 1.2f;
	float rotationAngleOct = 0.0f;
}

namespace RenderVars {
	const float FOV = glm::radians(90.f);
	const float zNear = 0.01f;
	const float zFar = 210.f;
	float currentFOV = FOV;

	glm::mat4 _projection;
	glm::mat4 _view;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	glm::vec3 cameraPos = glm::vec3(0, 0, 5);
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
	glm::vec3 cameraUpVec = glm::vec3(0, 1, 0);
	float ortoSize = 10.f;

	void testingCamera() {
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) {
			if (io.KeysDown[110]) { // N
				cameraPos += glm::vec3(-0.5f, 0, 0);
			}
			if (io.KeysDown[109]) { // M
				cameraPos += glm::vec3(0.5f, 0, 0);
			}
			if (io.KeysDown[104]) { // H
				cameraPos += glm::vec3(0, 0.5f, 0);
			}
			if (io.KeysDown[98]) { // B
				cameraPos += glm::vec3(0, -0.5f, 0);
			}
			if (io.KeysDown[119]) { // W
				glm::vec3 dirToTarget = glm::normalize(cameraTarget - cameraPos);
				cameraPos += dirToTarget*0.3f;
			}
			if (io.KeysDown[115]) { // S
				glm::vec3 dirToTarget = glm::normalize(cameraTarget - cameraPos);
				cameraPos -= dirToTarget * 0.3f;
			}
			if (io.KeysDown[97]) { // A
				glm::vec3 dirToTarget = glm::normalize(cameraTarget - cameraPos);
				glm::vec3 leftVector = glm::cross(glm::vec3(0,1,0),dirToTarget);
				cameraTarget += glm::normalize(leftVector) * 0.3f;
			}
			if (io.KeysDown[100]) { // D
				glm::vec3 dirToTarget = glm::normalize(cameraTarget - cameraPos);
				glm::vec3 rightVector = glm::cross(dirToTarget, glm::vec3(0, 1, 0));
				cameraTarget += glm::normalize(rightVector) * 0.3f;
			}
			//actualitzar camera
			_view = glm::lookAt(cameraPos, cameraTarget, cameraUpVec);
		}
	}
	void goOrto() {
		_projection = glm::ortho(-ortoSize, ortoSize, -ortoSize, ortoSize, zNear, zFar);
	}
	void goPersp() {
		_projection = glm::perspective(glm::radians(70.0f), (4.0f / 3.0f), zNear, zFar);
	}
	void resetToDefaultValues() {
		cameraPos = glm::vec3(0, 0, 5);
		cameraTarget = glm::vec3(0, 0, 0);
		cameraUpVec = glm::vec3(0, 1, 0);
	}
}
namespace RV = RenderVars;



namespace Scene {
	void renderScene1(double currentTime) {
		ImGui::Begin("Scene #1");
		ImGui::Text("Cubes");
		if (ImGui::Button("Generate random pos."))
			randomPositions::setNewRandPositions();
		ImGui::End();

		for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
			CubeShader::renderCubeInPos(randomPositions::arrayCubes[cubeN],0,_distanceWall);
			//CubeShader::ShaderRenderCode(currentTime, cubeN, 0.f);												//Renderizar los cubos. 0.f es rotation_angle
		}
	}

	void renderScene2(double currentTime) {
		ImGui::Begin("Scene #2");
		ImGui::Text("Truncated octahedrons");
		ImGui::End();

		for (int i = 0; i < arrangedCubes::cubicLatticePositions.size(); i++)
		{
			//std::cout << arrangedCubes::cubicLatticePositions.at(i).x << "," << arrangedCubes::cubicLatticePositions.at(i).y << "," << arrangedCubes::cubicLatticePositions.at(i).height << std::endl;
			//truncOctahedronShader::ShaderRenderCode(currentTime, false, arrangedCubes::cubicLatticePositions.at(i));
			CubeShader::renderCubeInPos(arrangedCubes::cubicLatticePositions.at(i), 0, _distanceWall);
		}
	}

	void renderScene3(double currentTime) {
		ImGui::Begin("Scene #3");
		ImGui::Text("Falling cubes");
		if (ImGui::Button("Reset"))
			Fall::currentFallDist = 0;
		ImGui::End();
		
		Fall::currentFallDist += Fall::fallSpeed;
		if (Fall::currentFallDist > Fall::maxFallDist)
			Fall::currentFallDist = 0;
		rotation_angle += 0.05f;
		for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
			CubeShader::ShaderRenderCode(currentTime, cubeN, rotation_angle);												//Renderizar los shaders. 0.f es rotation_angle
		}
	}

	void renderScene4(double currentTime) {
		ImGui::Begin("Scene #4");
		ImGui::Text("Falling truncated octahedrons");
		if (ImGui::Button("Reset"))
			Fall::currentFallDist = 0;
		ImGui::End();
		
		Fall::currentFallDist += Fall::fallSpeed;
		if (Fall::currentFallDist > Fall::maxFallDist)
			Fall::currentFallDist = 0;

		rotation_angle += 0.05f;
		
		for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
			glm::vec3 truncOctPosition = randomPositions::arrayCubes[cubeN] + glm::vec3(0, -Fall::currentFallDist, 0);
			//truncOctahedronShader::ShaderRenderCode(false, truncOctPosition);
			truncOctahedronShader::ShaderRenderWithRotation(false, truncOctPosition, -1, rotation_angle, rotationCubes::arrayRotationCubes[cubeN]);
		}
	}

	void renderScene5(double currentTime) {
		ImGui::Begin("Scene #5");
		ImGui::Text("Matrix effect");
		if (ImGui::Button("Reset"))
			Matrix::currentFallDist = 0;
		ImGui::End();
		

		//Render bottom objects (white)
		for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
			glm::vec3 truncOctPosition = randomPositions::arrayCubes[cubeN] + glm::vec3(0, -Matrix::currentFallDist, 0);
			truncOctahedronShader::ShaderRenderWithRotation(false, truncOctPosition, 6, Matrix::rotation, rotationCubes::arrayRotationCubes[cubeN]);
		}
			
		//Render trail objects (green)
		for (std::list<Matrix::objectTrail>::iterator it = Matrix::trailObjectsList.begin(); it != Matrix::trailObjectsList.end(); it++)
		{
			Matrix::objectTrail obTrail = (Matrix::objectTrail)*it;
			truncOctahedronShader::ShaderRenderWithRotation(false, obTrail.position, 0, obTrail.rotation, obTrail.rotationAxis);

			//Delete trail objects if needed
			it->timeLife += Matrix::fallSpeed;
			if (it->timeLife >= Matrix::objTrailMaxLife) {
				Matrix::trailObjectsList.erase(it);
			}
		}

		//Add new trail objects if needed;
		if (Matrix::nextTrailCounter >= Matrix::nextTrailObTime)
		{
			for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
				Matrix::objectTrail newObjectTrail;
				newObjectTrail.position = randomPositions::arrayCubes[cubeN]+glm::vec3(0,-Matrix::currentFallDist,0);
				newObjectTrail.rotation = Matrix::rotation;
				newObjectTrail.rotationAxis = rotationCubes::arrayRotationCubes[cubeN];
				newObjectTrail.timeLife = 0;
				Matrix::trailObjectsList.push_back(newObjectTrail);
			}
			Matrix::nextTrailCounter -= Matrix::nextTrailObTime;
		}
		Matrix::nextTrailCounter += Matrix::fallSpeed;

		//Update positions and rotation and reset if needed
		Matrix::rotation += 0.05f;
		Matrix::currentFallDist += Matrix::fallSpeed;

		if (Matrix::currentFallDist > Matrix::maxFallDist) {
			Matrix::currentFallDist = 0;
			randomPositions::setNewRandPositions();
		}
			
	}

	//Scane 6 vars
	namespace S6 {
		int currentPart = 1;

		const int TRUNC_OCT_PER_AXIS = 14;
		const float DISTANCE_THRESHOLD = 0.05f;	//threshold distance to the center of one of the seeds in the honeycomb to turn white

		const float TIME_FOR_NEXT_FALL_TROCT = 1.f;
		float nextFallingTrOctTimer = TIME_FOR_NEXT_FALL_TROCT;

		struct columnPos {
			glm::vec3 position;
			bool yDisplaced;
		};
		struct fallingOct {
			glm::vec3 position;
			bool yDisplaced;
			glm::vec3 rotAxis;	//rotation axis
		};

		std::vector<columnPos> columnPositions;	//list of trunc. oct. columns positions, where the new tunc. oct. can fall
		std::list<fallingOct> fallingTruncOct;	//all the falling trunc. otc.
		bool columnPositionsSet = false;

		float transitionStartTime = 0;
		enum transitionState
		{
			xyPlane,
			zyPlane,
			movingToxy,
			movingTozy
		};
		transitionState currentCameraState = transitionState::zyPlane;
	}

	void renderScene6(double currentTime) {
		ImGui::Begin("Scene #6");
		ImGui::Text("Bitruncated honeycomb:\nDrawing wireframes");
		ImGui::Separator();
		for (int b = 1; b < 5; b++)
		{
			bool popStyle = false;
			if (b == S6::currentPart) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1.f));
				popStyle = true;
			}
			if (ImGui::Button(std::string("Part " + std::to_string(b)).c_str())) {
				S6::currentPart = b;
				switch (b)
				{
				case 1:

					break;
				case 2:
					//Place camera in x-y plane
					RV::cameraPos = glm::vec3(5, 6, -5);
					RV::cameraTarget = glm::vec3(5, 6, 5);
					break;
				case 3:
					//Place camera in y-z plane
					RV::cameraPos = glm::vec3(-5, 6, 5);
					RV::cameraTarget = glm::vec3(5, 6, 5);
					break;
				case 4:
					RV::cameraPos = glm::vec3(-5, 6, 5);
					RV::cameraTarget = glm::vec3(5, 6, 5);
					S6::currentCameraState = S6::transitionState::zyPlane;
					break;
				default:
					break;
				}
			}
			if (popStyle) {
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}
		}
		
		if (S6::currentPart == 4)
		{
			if (ImGui::Button("Switch planes")) {
				if (S6::currentCameraState == S6::transitionState::zyPlane)
				{
					S6::currentCameraState = S6::transitionState::movingToxy;
				}
				else if (S6::currentCameraState == S6::transitionState::xyPlane)
				{
					S6::currentCameraState = S6::transitionState::movingTozy;
				}
				S6::transitionStartTime = currentTime;
			}
		}
		ImGui::End();

		if (S6::currentPart >= 2)
		{
			RV::goOrto();
		}
		else {
			RV::goPersp();
		}

		if (S6::currentPart == 4)
		{
			float angle = 0;
			if (S6::currentCameraState == S6::transitionState::movingToxy)
				angle = currentTime - S6::transitionStartTime;
			if (S6::currentCameraState == S6::transitionState::movingTozy)
				angle = glm::pi<float>()*2 -(currentTime - S6::transitionStartTime);

			angle /= 2;	//To make it slower

			if (S6::currentCameraState == S6::transitionState::movingToxy || S6::currentCameraState == S6::transitionState::movingTozy) {
				RV::cameraPos = glm::vec3(glm::cos(angle+glm::pi<float>()) * 5, 6.f, glm::sin(angle + glm::pi<float>() / 2.f) * 5);

				if (S6::currentCameraState == S6::transitionState::movingToxy && (angle > glm::pi<float>()) )
				{
					S6::currentCameraState = S6::transitionState::xyPlane;
					RV::cameraPos = glm::vec3(5, 6, -5);
				}
				if (S6::currentCameraState == S6::transitionState::movingTozy && (angle < 0) )
				{
					S6::currentCameraState = S6::transitionState::zyPlane;
					RV::cameraPos = glm::vec3(-5, 6, 5);
				}
			}
		}

		//DRAW TRUNCATED HONEYCOMB + set column positions
		glm::vec3 firstPos = glm::vec3(0);

		//each half "fits the holes" of the other half
		for (int aHalf = 0; aHalf < 2; aHalf++)
		{
			if (aHalf == 1)
			{
				firstPos += glm::vec3(0, truncOctahedronShader::getHeight() / 2, 2 * truncOctahedronShader::sideLenght / 3);
			}
			for (int nx = 0; nx < S6::TRUNC_OCT_PER_AXIS; nx++)
			{
				glm::vec3 startPos = firstPos + glm::vec3(nx*2*truncOctahedronShader::sideLenght/3,0,0);
				if (nx % 2 != 0)	//if the number is odd, 
				{
					startPos.z += 2 * truncOctahedronShader::sideLenght/3;
				}
				for (int nz = 0; nz < S6::TRUNC_OCT_PER_AXIS/2; nz++)
				{
					glm::vec3 truncOctP = startPos + glm::vec3(0, 0, nz * 4 * truncOctahedronShader::sideLenght / 3);
					
					if (!S6::columnPositionsSet) {
						S6::columnPos newColPos;
						newColPos.position = glm::vec3(truncOctP.x, 0, truncOctP.z);
						newColPos.yDisplaced = aHalf == 1;
						S6::columnPositions.push_back(newColPos);
					}
					
					for (int ny = 0; ny < S6::TRUNC_OCT_PER_AXIS/2; ny++)	//vertical column of trunc. oct. (connecting by the upper and the lower squares)
					{
						truncOctP.y = startPos.y + ny * truncOctahedronShader::getHeight();
						truncOctahedronShader::ShaderRenderWithRotation(true, truncOctP, -1, 0, glm::vec3(0, 1, 0));
					}
				}
			}
		}
		if (!S6::columnPositionsSet)
			S6::columnPositionsSet = true;

		if (S6::currentPart >= 2){
			//Add a new trunc. oct. when necessary
			if (S6::nextFallingTrOctTimer < currentTime)
			{
				S6::nextFallingTrOctTimer = currentTime + S6::TIME_FOR_NEXT_FALL_TROCT;
				//create a random position
				int randp = rand() % S6::columnPositions.size();
				glm::vec3 newPos = S6::columnPositions.at(randp).position + glm::vec3(0, truncOctahedronShader::getHeight() * (2 + S6::TRUNC_OCT_PER_AXIS/2), 0);
				if (S6::columnPositions.at(randp).yDisplaced)
					newPos.y += truncOctahedronShader::getHeight() / 2;
				S6::fallingOct newOcta;
				newOcta.position = newPos;
				newOcta.yDisplaced = S6::columnPositions.at(randp).yDisplaced;
				newOcta.rotAxis = glm::vec3((rand()%10)-5, (rand() % 10) - 5, (rand() % 10) - 5);
				newOcta.rotAxis = glm::normalize(newOcta.rotAxis);
				S6::fallingTruncOct.push_back(newOcta);
			}

			for (auto o = S6::fallingTruncOct.begin(); o != S6::fallingTruncOct.end();)
			{
				o->position += glm::vec3(0, -Fall::fallSpeed, 0);
				float distance = o->position.y / truncOctahedronShader::getHeight();
				if (o->yDisplaced)
				{
					//if displaced, add height/2 to the previous distance
					distance = (o->position.y + truncOctahedronShader::getHeight()/2) / truncOctahedronShader::getHeight();
				}
				distance -= (long)distance;	//get only the fractional part

				bool shouldIluminate = distance < S6::DISTANCE_THRESHOLD || distance > 1 - S6::DISTANCE_THRESHOLD;
				float maxHeight = o->yDisplaced ? (truncOctahedronShader::getHeight() * (S6::TRUNC_OCT_PER_AXIS / 2)-0.5f) + S6::DISTANCE_THRESHOLD
					: (truncOctahedronShader::getHeight() * (S6::TRUNC_OCT_PER_AXIS / 2)-1) + S6::DISTANCE_THRESHOLD;
				
				if (o->position.y > maxHeight )
				{
					shouldIluminate = false;
				}
				truncOctahedronShader::ShaderRenderWithRotation(false, o->position, shouldIluminate ? 6 : -1, distance * 2 * glm::pi<float>(), o->rotAxis);
				
				if (o->position.y < 0)
				{
					o = S6::fallingTruncOct.erase(o);
				} else {
					o++;
				}
			}

		}

	}
	
	//Detect keyboard input to change scene
	void detectInput() {
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) {
			if (io.KeysDown[49] && Scene::currentScene != 1) {		// Key 1
				Scene::currentScene = 1;
				RV::goPersp();
				RV::resetToDefaultValues();
			}
			if (io.KeysDown[50] && Scene::currentScene != 2) {		// Key 2
				arrangedCubes::arrangeCubesIntoLattice();
				Scene::currentScene = 2;
				RV::goPersp();
				RV::resetToDefaultValues();
			}
			if (io.KeysDown[51] && Scene::currentScene != 3) {		// Key 3
				Scene::currentScene = 3;
				RV::goOrto();
				RV::resetToDefaultValues();
			}
			if (io.KeysDown[52] && Scene::currentScene != 4) {		// Key 4
				Scene::currentScene = 4;
				RV::goOrto();
				RV::resetToDefaultValues();
			}
			if (io.KeysDown[53] && Scene::currentScene != 5) {		// Key 5
				Matrix::trailObjectsList.clear();
				Matrix::nextTrailCounter = 0;
				Matrix::currentFallDist = 0;
				Matrix::rotation = 0;
				Scene::currentScene = 5;
				RV::goOrto();
			}
			if (io.KeysDown[54] && Scene::currentScene != 6) {		// Key 6
				Scene::currentScene = 6;
				//persp? orto?
				RV::resetToDefaultValues();
			}
		}
	}
}

void randomPositions::setNewRandPositions() {
	float _squareX;
	float _squareY;
	float _squareZ;
	for (int i = 0; i < MaxCubes; i++) {
		//Las posiciones X se hacen randoms 
		_squareX = ((rand() % 100) - 50) / 20.0f;						//Random between [-2.5 and 2.5]
		randomPositions::arrayCubes[i].x = _squareX;
		//Posiciones Y randoms 
		_squareY = ((rand() % 100) - 50) / 20.0f;
		randomPositions::arrayCubes[i].y = _squareY;
		//Posiciones Z randoms
		_squareZ = ((rand() % 100) - 50) / 20.0f;
		randomPositions::arrayCubes[i].z = _squareZ;
	}
}

void GLinit(int width, int height) {
	srand(time(NULL));																				//The random numbers are diferent from each one 
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	RV::_view = glm::lookAt(RV::cameraPos, RV::cameraTarget, glm::vec3(0, 1, 0));
	//RV::_projection = glm::ortho(-5.f, 5.f, -5.f, 5.f, RV::zNear, RV::zFar);
	RV::goPersp();

	randomPositions::setNewRandPositions();

	int _squareXRotationRandom;							//Los cubos deben rotar, para ello, hay que crear un numero aletarorio entre 0 y 1 
	float _squareXRotation;
	int _squareYRotationRandom;							
	float _squareYRotation;
	int _squareZRotationRandom;
	float _squareZRotation;

	for (int i = 0; i < MaxCubes; i++) {
		_squareXRotationRandom = rand() % 1000 + 1000;					//Creo numero entre 0 y 1000
		_squareXRotation = _squareXRotationRandom / 2000;		//El aleatorio esta entre 0 y 1
		rotationCubes::arrayRotationCubes[i].x = ((float)rand()) / 40000.f;
		_squareYRotationRandom = rand() % 1000 + 1000;
		_squareYRotation = _squareYRotationRandom / 2000;		
		rotationCubes::arrayRotationCubes[i].y = ((float)rand()) / 40000.f;
		_squareZRotationRandom = rand() % 1000 + 1000;
		_squareZRotation = ((float)rand()) / 40000.f;
		rotationCubes::arrayRotationCubes[i].z = _squareZRotation;
		//std::cout << rotationCubes::arrayRotationCubes[i].x << std::endl;
		//std::cout << rotationCubes::arrayRotationCubes[i].y << std::endl;
		//std::cout << rotationCubes::arrayRotationCubes[i].height << std::endl;
		//rotationCubes::arrayRotationCubes[i] = glm::vec3(1.f, 1.f, 1.f);
		//std::cout << ((float)rand())/40000.f<< std::endl;				//Imprimir los randoms. De esta forma, se puede controlar mucho mejor los resultados 

	}

	CubeShader::ShaderInitCode();					//Inicializar los shaders 
	truncOctahedronShader::ShaderInitCode();
}

void GLcleanup() {
	CubeShader::ShaderCleanupCode();										
	truncOctahedronShader::ShaderCleanupCode();
}

void GLrender(double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RV::testingCamera();
	switch (Scene::currentScene)	//Using the var currentScene, we render the right scene only
	{
	case 1: Scene::renderScene1(currentTime);
		break;
	case 2: Scene::renderScene2(currentTime);
		break;
	case 3: Scene::renderScene3(currentTime);
		break;
	case 4: Scene::renderScene4(currentTime);
		break;
	case 5: Scene::renderScene5(currentTime);
		break;
	case 6: Scene::renderScene6(currentTime);
		break;
	default: //Should not happen
		break;
	}



	ImGui::Render();
	Scene::detectInput();
}

void GLResize(int width, int height) {
	glViewport(0, 0, width, height);
	if (height != 0) {
		if (Scene::currentScene == 0)
			RV::_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, RV::zNear, RV::zFar);
		else
			RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);
	}
	else { RV::_projection = glm::perspective(RV::FOV, 0.f, RV::zNear, RV::zFar); }
}

namespace arrangedCubes {
	//Place the cubes from bottom to top
	void arrangeCubesIntoLattice() {
		int sideCubes = 1;
		while (sideCubes*sideCubes*sideCubes < MaxCubes)	//Finds out whats the smallest formation of cubes. Calculates how much cubes need to be per side
		{
			sideCubes++;
		}

		cubicLatticePositions = std::vector<glm::vec3>();

		float startPos = -(_distanceWall / 2)*(sideCubes - 1);		//Starting position (bottom-left, back corner) - distance to center of the lattice (for each axis)
		float separation = 0.0f;									//Adds some optional separation to make each cube more visible

		std::cout << "cubeLattic of " << sideCubes << " sides and " << MaxCubes << " cubes:\n";
		for (int z = 0; z < sideCubes; z++)
		{
			if (cubicLatticePositions.size() == MaxCubes)
				break;
			for (int y = 0; y < sideCubes; y++)
			{
				if (cubicLatticePositions.size() == MaxCubes)
					break;
				for (int x = 0; x < sideCubes; x++)
				{
					glm::vec3 newCubePostion = glm::vec3(
						startPos+x*_distanceWall+x*separation,
						startPos+y*_distanceWall+y*separation,
						startPos+z*_distanceWall+z*separation );

					cubicLatticePositions.push_back(newCubePostion);
					std::cout << "cubeLattice" << newCubePostion.x << " " << newCubePostion.y << " " << newCubePostion.z << "\n";

					if (cubicLatticePositions.size()==MaxCubes)
						break;
				}
			}
		}
		std::cout << "S'han col·locat un total de " << cubicLatticePositions.size() << std::endl;

	}
}


namespace CubeShader {
	static const GLchar * cube_vertex_shader_source[] =
	{
		"#version 330													\n\
															\n\
		void main(){													\n\
		const vec4 square1[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0),		\n\
									vec4(0.25,0.25,0.5,1.0),			\n\
									vec4(-0.25,-0.25,0.5,1.0));			\n\
		\n\
		gl_Position = square1[gl_VertexID];								\n\
		}"
	};


	static const GLchar * cube_fragment_shader_source[] =
	{
		"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main(){\n\
			color = vec4(0.0,0.8,1.0,1.0);\n\
			const vec4 colorsLados[6] = vec4[6](vec4( 0, 1, 0,1.0),				\n\
												vec4(0.5, 1, 0.5, 1.0),			\n\
												vec4(1, 0.25, 0.5, 1.0),		\n\
												vec4(0.25, 0, 0, 1.0),			\n\
												vec4( 1, 0, 0, 1.0),			\n\
												vec4( 0.25, 0.25, 0.5, 1.0));	\n\
			color = colorsLados[gl_PrimitiveID ];								\n\
		}"
	};

	//vec4 square[24] = vec4[24](vec4(distanceWall, -distanceWall, distanceWall, 1.0) + vec4(cubes.x, cubes.y, cubes.height, 0.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	\n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	\n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.height, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.height, 1.0));				\n\



	//-(time/555)
	static const GLchar * cube_geom_shader_source[] =
	{ "#version 330																										\n\
			uniform mat4 mvpMat;																						\n\
			\n\
			uniform mat4 AxisSquare;		//rotation of the cube														\n\
			\n\
			uniform float distanceWall;		//lenght of the cube SIDE													\n\
			\n\
			layout(triangles) in;																						\n\
			layout(triangle_strip,max_vertices = 48) out;																\n\
																														\n\
			void main(){																								\n\
				float halfSide = distanceWall/2.0;																										\n\
				vec4 square[24]=vec4[24](vec4(halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(halfSide, halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, halfSide, halfSide,  1.0),							\n\
																											\n\
										vec4(halfSide, halfSide, halfSide,  1.0),							\n\
										vec4(halfSide, halfSide, -halfSide,  1.0),							\n\
										vec4(-halfSide, halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, halfSide, -halfSide,  1.0),							\n\
																											\n\
										vec4(-halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, -halfSide, -halfSide,  1.0),						\n\
										vec4(-halfSide, halfSide, -halfSide,  1.0),							\n\
																											\n\
										vec4(-halfSide, -halfSide, -halfSide,  1.0),						\n\
										vec4(-halfSide, halfSide, -halfSide,  1.0),							\n\
										vec4(halfSide, -halfSide, -halfSide,  1.0),							\n\
										vec4(halfSide, halfSide, -halfSide,  1.0),							\n\
																											\n\
										vec4(-halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(-halfSide, -halfSide, -halfSide,  1.0),						\n\
										vec4(halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(halfSide, -halfSide, -halfSide,  1.0),							\n\
																											\n\
										vec4(halfSide, -halfSide, -halfSide, 1.0),							\n\
										vec4(halfSide, halfSide, -halfSide,  1.0),							\n\
										vec4(halfSide, -halfSide, halfSide,  1.0),							\n\
										vec4(halfSide, halfSide, halfSide,  1.0) );							\n\
																											\n\
					gl_PrimitiveID = 0;																		\n\
					int primitive = 0;																		\n\
																											\n\
					for(int i = 0; i < 24; i++){															\n\
						gl_Position = mvpMat * AxisSquare * square[i];										\n\
						EmitVertex();																		\n\
						primitive++;																		\n\
						if(primitive == 4){				//at the end of a primitive, change face color		\n\
							EndPrimitive();																	\n\
							primitive = 0;																	\n\
							gl_PrimitiveID++;																\n\
						}																					\n\
					}																						\n\
																											\n\
			}" };

	GLuint	ShaderCompile() {			//Punter a un espai de memoria, on tenim el nostre programa. Ens donara un valor, per invocar al programa compilat. GLuint = enter si o si positiu 
		GLuint vertex_shader;				//GLuint es un unsigned int, que guarda direccions de memoria 
		GLuint geom_shader;					//Per fer el geometry shader
		GLuint fragment_shader;
		GLuint program;

		
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, cube_vertex_shader_source, NULL);		//Es pasa a la targeta grafica 
		glCompileShader(vertex_shader);										//Ara es te de compilar 

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);						//Per fer el geometry shader
		glShaderSource(geom_shader, 1, cube_geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(fragment_shader, 1, cube_fragment_shader_source, NULL);
		glCompileShader(fragment_shader);							//Compila el shaders

		program = glCreateProgram();								//Es crea un programa
		

		glAttachShader(program, vertex_shader);						//S' asocien els shaders al programa
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);						//S' asocia el geometry shader 
		
		glLinkProgram(program);										//El programa es linka a la memoria. Com un include

		glDeleteShader(vertex_shader);								//Ja s' han asociat al programa i ja s' han linkat. Per tant, ja es poden borrar
		glDeleteShader(geom_shader);								//Es pot borrar el geometry shader
		glDeleteShader(fragment_shader);
		return program;												//Es retorna el programa 
	}

	//3. init function
	void ShaderInitCode() {
		
		ShaderRenderProgram = ShaderCompile();			//Es guarda el resultat de la compilacio
		glCreateVertexArrays(1, &ShaderVAO);
		glBindVertexArray(ShaderVAO);						//VAO = Vertex Array Object 
		
	}

	//4. cleanup function 
	void ShaderCleanupCode(void) {
		glDeleteVertexArrays(1, &ShaderVAO);
		glBindVertexArray(ShaderRenderProgram);
	}

	glm::mat4 myMVPsquare;					//E3. Matrices de rotacion para que los cubos giren 
	glm::mat4 myMVPsquare2;					
	glm::mat4 myMVPsquare3;



	
	void ShaderRenderCode(double currentTime, int cubeN, float rotation_angle) {
		glUseProgram(ShaderRenderProgram);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "distanceWall"), (GLfloat)_distanceWall);			//distancia de las paredes respecto a la seed 

		glm::mat4 rotSquare = glm::rotate(glm::mat4(), rotation_angle, rotationCubes::arrayRotationCubes[cubeN]);										//E3. Con esto, se hace que la matriz rote.
		
		//std::cout << glm::to_string(rotationCubes::arrayRotationCubes[cubeN]) << std::endl;
		glm::mat4 cubeModel = glm::translate(randomPositions::arrayCubes[cubeN] + glm::vec3(0,-Fall::currentFallDist,0)) * rotSquare;	//Translation mat(Cube pos + falling dist) * rotation
		glm::mat4 MVPmatrix = RV::_projection * RV::_view * cubeModel;
		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));			//La matriz se aplica a mvpMat, que multiplicara a los puntos en cube_geom_shader_source[]

		glDrawArrays(GL_TRIANGLES, 0, 24);		//Compte, es POINTS, no POINT. El 3 es que dibuixa 3 punts 
	}

	void renderCubeInPos(glm::vec3 position, float rotation_angle, float sideLenght) {
		glUseProgram(ShaderRenderProgram);
		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "AxisSquare"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
		//glm::mat4 rotSquare = glm::rotate(glm::mat4(), rotation_angle, glm::vec3(0));	//can be changed if needed

		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "distanceWall"), (GLfloat)sideLenght);

		glm::mat4 cubeModel = glm::translate(position);								//add rotation or scaling if needed
		glm::mat4 MVPmatrix = RV::_projection * RV::_view * cubeModel;
		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));

		glDrawArrays(GL_TRIANGLES, 0, 24);
	}
}
namespace truncOctahedronShader {

	float getHeight() {
		return 2.f * glm::sqrt(2.f) * sideLenght / 3;
	}

	static const GLchar * trOct_vertex_shader_source[] =
	{
		"#version 330\n\
		\n\
		void main(){ \n\
			gl_Position = vec4(0,0,0,1);\n\
		}"
	};
	static const GLchar * trOct_fragment_shader_source[] =
	{
		"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main(){\n\
			color = vec4(0.0,0.8,1.0,1.0);\n\
			const vec4 colorsLados[7] = vec4[7](vec4( 0, 1, 0,1.0),				\n\
												vec4(0.5, 1, 0.5, 1.0),			\n\
												vec4(1, 0.25, 0.5, 1.0),		\n\
												vec4(0.25, 0, 0, 1.0),			\n\
												vec4( 1, 0, 0, 1.0),			\n\
												vec4( 0.25, 0.25, 0.5, 1.0),	\n\
												vec4( 1, 1, 1, 1.0) );	//White	\n\
			color = colorsLados[gl_PrimitiveID ];								\n\
		}"
	};

	static const GLchar * trOct_geom_shader_source[] =
	{
		"#version 330\n\
		\n\
		uniform mat4 mvpMat;\n\
		uniform float sideLenght;\n\
		uniform int fcolor;\n\
		layout(triangles) in;												\n\
		layout(triangle_strip,max_vertices = 76) out;						\n\
		void main(){														\n\
			//float sideLenght = 0.5;										\n\
			//																\n\
			vec4 up = vec4(0.0, sqrt(2)*sideLenght/2.0, 0.0, 1.0);			\n\
			vec4 down = vec4(0.0, -sqrt(2)*sideLenght/2.0, 0.0, 1.0);		\n\
			//Octahedron square:				\n\
			vec4 a = vec4( -sideLenght/2, 0.0, sideLenght/2, 1.0);\n\
			vec4 b = vec4( sideLenght/2, 0.0, sideLenght/2, 1.0);\n\
			vec4 c = vec4( sideLenght/2, 0.0, -sideLenght/2, 1.0);\n\
			vec4 d = vec4( -sideLenght/2, 0.0, -sideLenght/2, 1.0);\n\
				//Truncated octahedron \n\
				vec4 bottomVertex[5] = vec4[5](a,b,c,d,a);							\n\
				for(int i=0;i<4;i++){												\n\
					if(fcolor != -1){						\n\
						gl_PrimitiveID = fcolor;			\n\
					} else {								\n\
						gl_PrimitiveID = i; }				\n\
					vec4 left	= bottomVertex[i];				//a,b,c,d			\n\
					vec4 right	= bottomVertex[i+1];			//b,c,d,a			\n\
					//----Top Hexagons												\n\
					gl_Position = mvpMat*(left+(up-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(right-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(up+(left-up)/3);							\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(left-right)/3);					\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(up+(right-up)/3);							\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(up-right)/3);						\n\
					EmitVertex();													\n\
					EndPrimitive();													\n\
					//----Bottom Hexagons											\n\
					gl_Position = mvpMat*(left+(down-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(down+(left-down)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(right-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(down+(right-down)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(left-right)/3);					\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(down-right)/3);					\n\
					EmitVertex();													\n\
					EndPrimitive();													\n\
					//----Bottom left squares										\n\
					vec4 squareCenter	= left;					//a,b,c,d			\n\
					vec4 squareRight	= right;				//b,c,d,a			\n\
					vec4 squareLeft	= bottomVertex[(3+i)%4];	//d,a,b,c			\n\
					if(fcolor != -1){						\n\
						gl_PrimitiveID = fcolor;			\n\
					} else {								\n\
						gl_PrimitiveID = 5; }				\n\
					gl_Position = mvpMat*(squareCenter+(squareLeft-squareCenter)/3);		\n\
					EmitVertex();															\n\
					gl_Position = mvpMat*(squareCenter+(down-squareCenter)/3);				\n\
					EmitVertex();															\n\
					gl_Position = mvpMat*(squareCenter+(up-squareCenter)/3);				\n\
					EmitVertex();															\n\
					gl_Position = mvpMat*(squareCenter+(squareRight-squareCenter)/3);		\n\
					EmitVertex();															\n\
					EndPrimitive();															\n\
				}\n\
				//----Top Square\n\
				if(fcolor != -1){						\n\
					gl_PrimitiveID = fcolor;			\n\
				} else {								\n\
					gl_PrimitiveID = 5; }				\n\
				gl_Position = mvpMat*(up+(a-up)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(up+(b-up)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(up+(d-up)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(up+(c-up)/3);		\n\
				EmitVertex();\n\
				EndPrimitive();\n\
				//----Bottom Square\n\
				gl_Position = mvpMat*(down+(a-down)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(down+(d-down)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(down+(b-down)/3);		\n\
				EmitVertex();\n\
				gl_Position = mvpMat*(down+(c-down)/3);		\n\
				EmitVertex();\n\
				EndPrimitive();\n\
		}"
	};


	//trunc. oct. geom shader -- line_strip
	static const GLchar * trOct_geom_wireframe_shader_source[] =
	{
		"#version 330\n\
		\n\
		uniform mat4 mvpMat;												\n\
		uniform float sideLenght;											\n\
		layout(triangles) in;												\n\
		layout(line_strip,max_vertices = 48) out;							\n\
		void main(){														\n\
			// Top and bottom vertexs:										\n\
			vec4 up = vec4(0.0, sqrt(2)*sideLenght/2.0, 0.0, 1.0);			\n\
			vec4 down = vec4(0.0, -sqrt(2)*sideLenght/2.0, 0.0, 1.0);		\n\
			// Octahedron square vertexs:									\n\
			vec4 a = vec4( -sideLenght/2, 0.0, sideLenght/2, 1.0);			\n\
			vec4 b = vec4( sideLenght/2, 0.0, sideLenght/2, 1.0);			\n\
			vec4 c = vec4( sideLenght/2, 0.0, -sideLenght/2, 1.0);			\n\
			vec4 d = vec4( -sideLenght/2, 0.0, -sideLenght/2, 1.0);			\n\
				gl_PrimitiveID = 1;											\n\
				vec4 bottomVertex[5] = vec4[5](a,b,c,d,a);							\n\
				for(int i=0;i<4;i++){												\n\
					vec4 left	= bottomVertex[i];				//a,b,c,d			\n\
					vec4 right	= bottomVertex[i+1];			//b,c,d,a			\n\
					//----Top Hexagons (includes top and middle squares)			\n\
					gl_Position = mvpMat*(up+(right-up)/3);							\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(up+(left-up)/3);							\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(up-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(right-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(left-right)/3);					\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(up-right)/3);						\n\
					EmitVertex();													\n\
					EndPrimitive();													\n\
					//----Bottom Hexagons (includes bottom and middle squares)		\n\
					gl_Position = mvpMat*(right+(down-right)/3);					\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(right+(left-right)/3);					\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(right-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(left+(down-left)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(down+(left-down)/3);						\n\
					EmitVertex();													\n\
					gl_Position = mvpMat*(down+(right-down)/3);						\n\
					EmitVertex();													\n\
					EndPrimitive();													\n\
				}																	\n\
		}"
	};


	GLuint	ShaderCompile(bool wireframe) {
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, trOct_vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		GLuint geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		if (!wireframe)
		{
			glShaderSource(geom_shader, 1, trOct_geom_shader_source, NULL);
		}
		else {
			glShaderSource(geom_shader, 1, trOct_geom_wireframe_shader_source, NULL);
		}

		glCompileShader(geom_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, trOct_fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, geom_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(geom_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	void ShaderInitCode() {
		ShaderRenderProgram = ShaderCompile(false);
		glCreateVertexArrays(1, &ShaderVAO);
		glBindVertexArray(ShaderVAO);
		//Wireframe shader
		WireframeShaderRenderProgram = ShaderCompile(true);
		glCreateVertexArrays(1, &WireframeShaderVAO);
		glBindVertexArray(WireframeShaderVAO);
	}
	void ShaderCleanupCode(void) {
		glDeleteVertexArrays(1, &ShaderVAO);
		glBindVertexArray(ShaderRenderProgram);
		//Wireframe shader
		glDeleteVertexArrays(1, &WireframeShaderVAO);
		glBindVertexArray(WireframeShaderRenderProgram);

	}

	void ShaderRenderWithRotation(bool wireframe, glm::vec3 otruncOctPos, int color, float rotationRads, glm::vec3 rotationAxis) {
		GLuint currentProgram = wireframe ? WireframeShaderRenderProgram : ShaderRenderProgram;
		glUseProgram(currentProgram);

		glUniform1f(glGetUniformLocation(currentProgram, "sideLenght"), (GLfloat)sideLenght);
		glUniform1i(glGetUniformLocation(currentProgram, "fcolor"), (GLint)color);
		//glUniform4f(glGetUniformLocation(ShaderRenderProgram, "centerPos"), (GLfloat)truncatedOctTest.x, (GLfloat)truncatedOctTest.y, (GLfloat)truncatedOctTest.height, (GLfloat)truncatedOctTest.w);

		glm::mat4 rot;
		if (rotationRads != 0)
			rot = glm::rotate(glm::mat4(), rotationRads, rotationAxis);
		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(otruncOctPos.x, otruncOctPos.y, otruncOctPos.z)) * rot;
		glm::mat4 MVPmatrix = RV::_projection * RV::_view * model;

		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));

		GLsizei vertexs = wireframe ? 48 : 76;				//Wireframe rendering uses less vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexs);
	}
}
