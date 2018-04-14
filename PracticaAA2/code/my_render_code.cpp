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

#include "glm/ext.hpp"		//Libreria para imprimir con cout vec3 y mat4 


//Global variables

float _distanceWall = 0.8f;	//0.07										//Distancia entre el punto cental y las paredes 
float rotation_angle = 0.f;												//Variable, con la que podemos rotar los cubos a cierta velocidad 
bool CubeCanRotate = false;												//Si el cubo puede rotar
const int MaxCubes = 10;									//Warning: if all cubes are drawn in the same exact position, visualitzation could be wrong



//Forward declarations and namespaces

namespace randomPositions {
	glm::vec3 arrayCubes[MaxCubes];
}
//Namespace para las rotaciones de los cubos 
namespace rotationCubes {
	glm::vec3 arrayRotationCubes[MaxCubes];
}
//Namespace para los valores de la Y cuando deban caer o no 
namespace Fall {
	glm::vec3 fallingCubes[MaxCubes];
}

namespace arrangedCubes {
	std::vector<glm::vec3> cubicLatticePositions;						//Cube lattice positions
	void arrangeCubesIntoLattice();
}

namespace ImGui {
	void Render();
}

namespace Scene {
	int currentScene = 1;	//1, 2, 3...
	void renderScene1(double currentTime);
	void renderScene2(double currentTime);
	void renderScene3(double currentTime);
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
	void ShaderRenderCode(double currentTime, bool wireframe, glm::vec3 otruncOctPos);

	GLuint ShaderRenderProgram;
	GLuint WireframeShaderRenderProgram;
	GLuint ShaderVAO;
	GLuint WireframeShaderVAO;

	//glm::vec4 truncatedOctTest = _square1;
	glm::vec4 tOctPositions[5] = { glm::vec4(30.0f, 30.0f, -30.0f, 1.0f) };
	glm::vec4 truncatedOctTest = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float sideLenght = 1.2f;
	float rotationAngleOct = 0.0f;
}

namespace RenderVars {
	const float FOV = glm::radians(90.f);
	const float zNear = 0.01f;
	const float zFar = 70.f;
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

	float rota[2] = { 0.f, 0.f };				//"camera" rotation
	//Translates the world by the given vector3
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
			//actualitzar camera
			_view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}
	}
}
namespace RV = RenderVars;



namespace Scene {
	void renderScene1(double currentTime) {
		ImGui::Begin("Scene #1");
		ImGui::Text("Cubes");
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
			//std::cout << arrangedCubes::cubicLatticePositions.at(i).x << "," << arrangedCubes::cubicLatticePositions.at(i).y << "," << arrangedCubes::cubicLatticePositions.at(i).z << std::endl;
			//truncOctahedronShader::ShaderRenderCode(currentTime, false, arrangedCubes::cubicLatticePositions.at(i));
			CubeShader::renderCubeInPos(arrangedCubes::cubicLatticePositions.at(i), 0, _distanceWall);
		}
	}

	void renderScene3(double currentTime) {
		ImGui::Begin("Scene #3");
		ImGui::Text("Cubes");
		ImGui::End();

		rotation_angle += 0.01f;
		for (int cubeN = 0; cubeN < MaxCubes; cubeN++) {
			CubeShader::ShaderRenderCode(currentTime, cubeN, rotation_angle);												//Renderizar los shaders. 0.f es rotation_angle
			
		}
	}

	void renderScene6(double currentTime) {
		ImGui::Begin("Scene #6");
		ImGui::Text("Truncated octahedrons:\nDrawing wireframes");
		ImGui::End();

		//std::cout << truncatedOctTest.x << "," << truncatedOctTest.y << "," << truncatedOctTest.z;
		truncOctahedronShader::ShaderRenderCode(currentTime, true, truncOctahedronShader::truncatedOctTest);

	}
	
	//Detect keyboard input to change scene
	void detectInput() {
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) {
			if (io.KeysDown[49] && Scene::currentScene != 1) {		// Key 1
				Scene::currentScene = 1;
				for (int i = 0; i < MaxCubes; i++) {
					Fall::fallingCubes[MaxCubes].y = 0.f;
				}
			}
			if (io.KeysDown[50] && Scene::currentScene != 2) {		// Key 2
				arrangedCubes::arrangeCubesIntoLattice();
				Scene::currentScene = 2;
				for (int i = 0; i < MaxCubes; i++) {
					Fall::fallingCubes[MaxCubes].y = 0.f;
				}
			}
			if (io.KeysDown[51] && Scene::currentScene != 3) {		// Key 3
				Scene::currentScene = 3;
				for (int i = 0; i < MaxCubes; i++) {
					Fall::fallingCubes[MaxCubes].y -= 0.01f;

				}
			}
			if (io.KeysDown[54] && Scene::currentScene != 6) {		// Key 6
				Scene::currentScene = 6;
				for (int i = 0; i < MaxCubes; i++) {
					Fall::fallingCubes[MaxCubes].y = 0.f;

				}
			}
		}
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
	RV::_projection = glm::perspective(glm::radians(70.0f), (4.0f / 3.0f), 0.1f, 10.0f);

	//lastWidth = width;
	//lastHeight = height;
	//glm::vec3 arrayCubes[MaxCubes];
	//Inicializacion para crear las posiciones randoms 
	int _squareXRandom;				
	float _squareX;			
	int _squareYRandom;
	float _squareY;
	int _squareZRandom;
	float _squareZ;

	for (int i = 0; i < MaxCubes; i++) {
		//Las posiciones X se hacen randoms 
		_squareXRandom = (rand() % 8000) - 4000;				//produce un numero entre -2000 - +2000
		//_squareX = _square1XRandom / 10000.0f;				//ahora lo convierte en uno entre -0.2 y 0.2
		_squareX = (rand()%100) / 100.0f;				//ahora lo convierte en uno entre -0.2 y 0.2
		randomPositions::arrayCubes[i].x = _squareX;
		//Posiciones Y randoms 
		_squareYRandom = rand() % 4000 - 2000;				
		//_squareY = _squareYRandom / 10000.0;
		_squareY = ((rand() % 100)-50) / 100.0f;
		randomPositions::arrayCubes[i].y = _squareY;
		//Posiciones Z randoms
		_squareZRandom = rand() % 4000 - 2000;
		//_squareZ = _squareZRandom / 10000.0;
		_squareZ = ((rand() % 100) - 50) / 100.0f;
		randomPositions::arrayCubes[i].z = _squareZ;
	}


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
		//std::cout << rotationCubes::arrayRotationCubes[i].z << std::endl;
		//rotationCubes::arrayRotationCubes[i] = glm::vec3(1.f, 1.f, 1.f);
		//std::cout << ((float)rand())/40000.f<< std::endl;				//Imprimir los randoms. De esta forma, se puede controlar mucho mejor los resultados 

	}

	for (int i = 0; i < MaxCubes; i++) {
		Fall::fallingCubes[MaxCubes].x = 0.f;
		Fall::fallingCubes[MaxCubes].y = 0.5f;
		Fall::fallingCubes[MaxCubes].z = 0.f;
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
	switch (Scene::currentScene)	//using the var currentScene, we render the right scene
	{
	case 1: Scene::renderScene1(currentTime);
		break;
	case 2: Scene::renderScene2(currentTime);
		break;
	case 3: Scene::renderScene3(currentTime);
		break;
	case 6: Scene::renderScene6(currentTime);
		break;
	default: //Shouldn't happen
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

	//vec4 square[24] = vec4[24](vec4(distanceWall, -distanceWall, distanceWall, 1.0) + vec4(cubes.x, cubes.y, cubes.z, 0.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	\n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	\n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(-distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	\n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, -distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, -distanceWall + cubes.y, distanceWall + cubes.z, 1.0), \n\
	//	vec4(distanceWall + cubes.x, distanceWall + cubes.y, distanceWall + cubes.z, 1.0));				\n\



	//-(time/555)
	static const GLchar * cube_geom_shader_source[] =
	{ "#version 330																										\n\
			uniform float time;																							\n\
			uniform mat4 mvpMat;																						\n\
			\n\
			uniform vec3 cubes;			//Position of current cube we are creating (a SINGLE cube)						\n\
			uniform float square1W;																						\n\
			uniform mat4 AxisSquare;	//rotation of the cube															\n\
			\n\
			uniform float distanceWall;		//lenght of the cube SIDE													\n\
			\n\
			uniform vec3 falling;																						\n\
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
										vec4(halfSide, halfSide, halfSide,  1.0));							\n\
					\n\
					\n\
					for(int i = 0; i < 4; i++){																			\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 0;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
					for(int i = 4; i < 8; i++){																			\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 1;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
					for(int i = 8; i < 12; i++){																		\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 2;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
					for(int i = 12; i < 16; i++){																		\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 3;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
					for(int i = 16; i < 20; i++){																		\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 4;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
					for(int i = 20; i < 24; i++){																		\n\
						gl_Position = ((mvpMat * AxisSquare * square[i]) + vec4(falling.x,falling.y,falling.z,0.0) + vec4(cubes.x,cubes.y,cubes.z,1.0));															\n\
						gl_PrimitiveID = 5;																				\n\
						EmitVertex();																					\n\
					}																									\n\
					EndPrimitive();																						\n\
																			\n\
				\n\
			}" };

	//Para el ejericio 3, se aplica un vector4 que le restara continuamente la posicion Y (time siempre esta aumentando de valor) para que caiga 

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
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "time"), (GLfloat)currentTime);
		glUniform3f(glGetUniformLocation(ShaderRenderProgram, "cubes"), (GLfloat)randomPositions::arrayCubes[cubeN].x, (GLfloat)randomPositions::arrayCubes[cubeN].y, (GLfloat)randomPositions::arrayCubes[cubeN].z);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "distanceWall"), (GLfloat)_distanceWall);			//distancia de las paredes respecto a la seed 
		
		glUniform3f(glGetUniformLocation(ShaderRenderProgram, "falling"), (GLfloat)Fall::fallingCubes[cubeN].x, (GLfloat)Fall::fallingCubes[cubeN].y, (GLfloat)Fall::fallingCubes[cubeN].z);				//Caida de los cubos 


		glm::mat4 rotSquare;
		/*if (CubeCanRotate) {
			rotation_angle += 0.01f;
		}
		else rotation_angle = 0.f;*/
		rotSquare = glm::rotate(glm::mat4(), rotation_angle, rotationCubes::arrayRotationCubes[cubeN]);										//E3. Con esto, se hace que la matriz rote.
		
		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "AxisSquare"), 1, GL_FALSE, glm::value_ptr(rotSquare));		//E3. La matriz se aplica a mvpMat, que multiplicara a los puntos en cube_geom_shader_source[]
		//myMVPsquare = myMVPsquare * rotSquare;																						//E3. Para aplicar la transformacion
		//glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "AxisSquare"), 1, GL_FALSE, glm::value_ptr(myMVPsquare));		//E3. La matriz se aplica a mvpMat, que multiplicara a los puntos en cube_geom_shader_source[]
		
		
		//std::cout << glm::to_string(rotationCubes::arrayRotationCubes[cubeN]) << std::endl;
		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));			//La matriz se aplica a mvpMat, que multiplicara a los puntos en cube_geom_shader_source[]

		
		//glPointSize(40.0f);						//Cada vertex te un tamany de 40 pixels 
		glDrawArrays(GL_TRIANGLES, 0, 24);		//Compte, es POINTS, no POINT. El 3 es que dibuixa 3 punts 
	}

	void renderCubeInPos(glm::vec3 position, float rotation_angle, float sideLenght) {
		glUseProgram(ShaderRenderProgram);
		glUniform3f(glGetUniformLocation(ShaderRenderProgram, "cubes"), (GLfloat)0, (GLfloat)0, (GLfloat)0);					//Useless variables
		glUniform3f(glGetUniformLocation(ShaderRenderProgram, "falling"), (GLfloat)0, (GLfloat)0, (GLfloat)0);
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
			const vec4 colorsLados[6] = vec4[6](vec4( 0, 1, 0,1.0),				\n\
												vec4(0.5, 1, 0.5, 1.0),			\n\
												vec4(1, 0.25, 0.5, 1.0),		\n\
												vec4(0.25, 0, 0, 1.0),			\n\
												vec4( 1, 0, 0, 1.0),			\n\
												vec4( 0.25, 0.25, 0.5, 1.0));	\n\
			color = colorsLados[gl_PrimitiveID ];								\n\
		}"
	};

	//TODO
	static const GLchar * trOct_geom_shader_source[] =
	{
		"#version 330\n\
		\n\
		uniform mat4 mvpMat;\n\
		uniform float sideLenght;\n\
		layout(triangles) in;																						\n\
		layout(triangle_strip,max_vertices = 76) out;																\n\
		void main(){ \n\
			//float sideLenght = 0.5;													\n\
			//																		\n\
			vec4 up = vec4(0.0, sqrt(2)*sideLenght/2.0, 0.0, 1.0);			\n\
			vec4 down = vec4(0.0, -sqrt(2)*sideLenght/2.0, 0.0, 1.0);		\n\
			//Octahedron square:				\n\
			vec4 a = vec4( -sideLenght/2, 0.0, sideLenght/2, 1.0);\n\
			vec4 b = vec4( sideLenght/2, 0.0, sideLenght/2, 1.0);\n\
			vec4 c = vec4( sideLenght/2, 0.0, -sideLenght/2, 1.0);\n\
			vec4 d = vec4( -sideLenght/2, 0.0, -sideLenght/2, 1.0);\n\
				//Truncated octahedron \n\
				gl_PrimitiveID = 4; \n\
				vec4 bottomVertex[5] = vec4[5](a,b,c,d,a);							\n\
				for(int i=0;i<4;i++){												\n\
					gl_PrimitiveID = i;												\n\
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
					gl_PrimitiveID = 5;												\n\
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
				gl_PrimitiveID = 5; \n\
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
					//----Top Hexagons (includes top and middle squares				\n\
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
		//wireframe
		WireframeShaderRenderProgram = ShaderCompile(true);
		glCreateVertexArrays(1, &WireframeShaderVAO);
		glBindVertexArray(WireframeShaderVAO);
	}
	void ShaderCleanupCode(void) {
		glDeleteVertexArrays(1, &ShaderVAO);
		glBindVertexArray(ShaderRenderProgram);
		//wireframe
		glDeleteVertexArrays(1, &WireframeShaderVAO);
		glBindVertexArray(WireframeShaderRenderProgram);

	}
	void ShaderRenderCode(double currentTime, bool wireframe, glm::vec3 otruncOctPos) {
		//canviar els truncatedOctTest per octruncOctPos

		//Debug
		rotationAngleOct = currentTime * 50;


		GLuint currentProgram = wireframe ? WireframeShaderRenderProgram : ShaderRenderProgram;

		glUseProgram(currentProgram);

		glUniform1f(glGetUniformLocation(currentProgram, "sideLenght"), (GLfloat)sideLenght);
		//glUniform4f(glGetUniformLocation(ShaderRenderProgram, "centerPos"), (GLfloat)truncatedOctTest.x, (GLfloat)truncatedOctTest.y, (GLfloat)truncatedOctTest.z, (GLfloat)truncatedOctTest.w);
		//glUniform4f(glGetUniformLocation(ShaderRenderProgram, "centerPos"), 0, 0, 0, 1);

		/*glm::mat4 rot = glm::rotate(glm::mat4(), 0.05f, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 myMVP = rot * myMVP;*/

		
		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(otruncOctPos.x, otruncOctPos.y, otruncOctPos.z));
		//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		
		glm::mat4 newProjection = glm::perspective(glm::radians(70.0f), (4.0f / 3.0f), 0.1f, 10.0f);
		glm::mat4 MVPmatrix = RV::_projection * RV::_view * model;
		//glm::mat4 MVPmatrix = RV::_projection * view * model;
		//MVPmatrix = glm::mat4();	//TODO: arreglar, de mentres fer com si no projectessim res
		//MVPmatrix = glm::rotate(MVPmatrix, glm::radians(rotationAngleOct), glm::vec3(1, 1, 0));  //per anar mirant com es veu amb diferents rotacions
		glUniformMatrix4fv(glGetUniformLocation(currentProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(MVPmatrix));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //not the best way
		GLsizei vertexs = wireframe ? 48 : 76;
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexs);
		//glDrawArrays(GL_LINE_STRIP, 0, 76);
	}

}

//TODO: els compileShader i linkProgram que tinguin les seves propies funcions (est� a l'altre pr�ctic