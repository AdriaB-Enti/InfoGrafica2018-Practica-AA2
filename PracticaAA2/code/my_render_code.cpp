#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <imgui\imgui.h>
#include <iostream>
#include <stdlib.h>

#include "GL_framework.h"

//Global variables

//Cubo 1
int _square1XRandom = rand() % 4000 - 2000;				//produce un numero entre -2000 - +2000
float _square1X = _square1XRandom / 10000.0;			//ahora lo convierte en uno entre -0.2 y 0.2
int _square1YRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square1Y = _square1YRandom / 10000.0;			//Entre -0.2 y 0.2
int _square1ZRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square1Z = _square1ZRandom / 10000.0;			//Entre -0.2 y 0.2
float _square1W = 1.f;
glm::vec4 _square1 = { _square1X,_square1Y,_square1Z,_square1W };	//coordenadas del punto central, la seed 

//Cubo 2
int _square2XRandom = rand() % 4000 - 2000;				//produce un numero entre -2000 - +2000
float _square2X = _square2XRandom / 10000.0;			//ahora lo convierte en uno entre -0.2 y 0.2
int _square2YRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square2Y = _square2YRandom / 10000.0;			//Entre -0.2 y 0.2
int _square2ZRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square2Z = _square2ZRandom / 10000.0;			//Entre -0.2 y 0.2
float _square2W = 1.f;
glm::vec4 _square2 = { _square2X,_square2Y,_square2Z,_square2W };	//coordenadas del punto central, la seed 

//Cubo 3
int _square3XRandom = rand() % 4000 - 2000;				//produce un numero entre -2000 - +2000
float _square3X = _square3XRandom / 10000.0;			//ahora lo convierte en uno entre -0.2 y 0.2
int _square3YRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square3Y = _square3YRandom / 10000.0;			//Entre -0.2 y 0.2
int _square3ZRandom = rand() % 4000 - 2000;				//Entre -2000 y 2000
float _square3Z = _square3ZRandom / 10000.0;			//Entre -0.2 y 0.2
float _square3W = 1.f;
glm::vec4 _square3 = { _square3X,_square3Y,_square3Z,_square3W };	//coordenadas del punto central, la seed 















float _distanceWall = 0.2;											//Distancia entre el punto cental y las paredes 


//Forward declarations
namespace ImGui {
	void Render();
}

namespace Scene {
	int currentScene = 1;	//1, 2, 3...
	void renderScene1();
	void detectInput();
}

namespace ShaderClass {
	
	void ShaderInitCode();
	GLuint ShaderCompile();

	void ShaderCleanupCode(void);
	void ShaderRenderCode(double currentTime);

	GLuint ShaderRenderProgram;
	GLuint ShaderVAO;
}


namespace RenderVars {
	const float FOV = glm::radians(90.f);
	const float zNear = 0.01f;
	const float zFar = 70.f;
	float currentFOV = FOV;

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, -5.f, -15.f };		//"camera" position
	float rota[2] = { 0.f, 0.f };				//"camera" rotation
	//Translates the world by the given vector3
	void travelling(glm::vec3 displacement) {
		panv[0] += displacement.x;
		panv[1] += displacement.y;
		panv[2] += displacement.z;
	}
}
namespace RV = RenderVars;



namespace Scene {
	void renderScene1() {
		ImGui::Begin("Scene #1");
		//ImGui::Text("test");
		ImGui::End();
	}

	//Detect keyboard input to change scene
	void detectInput() {
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) {
			if (io.KeysDown[49] && Scene::currentScene != 1) {		// Key 1
				Scene::currentScene = 1;
			}
			if (io.KeysDown[50] && Scene::currentScene != 2) {		// Key 2
				Scene::currentScene = 2;
			}
			if (io.KeysDown[51] && Scene::currentScene != 3) {		// Key 3
				Scene::currentScene = 3;
			}
		}
	}
}

void GLinit(int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	RV::_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, RV::zNear, RV::zFar);

	//Box::setupCube();
	//Axis::setupAxis();
	//Cube::setupCube();

	//lastWidth = width;
	//lastHeight = height;

	ShaderClass::ShaderInitCode();					//Inicizlizar los shaders 
}

void GLcleanup() {
	//Box::cleanupCube();
	//Axis::cleanupAxis();
	//Cube::cleanupCube();

	ShaderClass::ShaderCleanupCode();										
}

void GLrender(double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (Scene::currentScene)	//using the var currentScene, we render the right scene
	{
	case 1: Scene::renderScene1();
		break;
	/*case 2: Scene::renderScene2();
		break;
	case 3: Scene::renderScene3();
		break;*/
	default: //Shouldn't happen
		break;
	}


	ShaderClass::ShaderRenderCode(currentTime);												//Renderizar los shaders

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


namespace ShaderClass {
	static const GLchar * vertex_shader_source[] =
	{
		"#version 330\n\
		\n\
		void main(){ \n\
		const vec4 square1[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0),\n\
									vec4(0.25,0.25,0.5,1.0),\n\
									vec4(-0.25,-0.25,0.5,1.0));\n\
		const vec4 square2[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0),\n\
									vec4(0.25,0.25,0.5,1.0),\n\
									vec4(-0.25,-0.25,0.5,1.0));\n\
		const vec4 square3[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0),\n\
									vec4(0.25,0.25,0.5,1.0),\n\
									vec4(-0.25,-0.25,0.5,1.0));\n\
		gl_Position = square1[gl_VertexID];\n\
		gl_Position = square2[gl_VertexID];\n\
		gl_Position = square3[gl_VertexID];\n\
		}"
	};


	static const GLchar * fragment_shader_source[] =
	{
		"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main(){\n\
			color = vec4(0.0,0.8,1.0,1.0);\n\
			const vec4 colorsLados[6] = vec4[6](vec4( 0, 1, 0,1.0),				\n\												vec4(0.5, 1, 0.5, 1.0),			\n\
												vec4(1, 0.25, 0.5, 1.0),		\n\
												vec4(0.25, 0, 0, 1.0),			\n\
												vec4( 1, 0, 0, 1.0),			\n\
												vec4( 0.25, 0.25, 0.5, 1.0));	\n\
			color = colorsLados[gl_PrimitiveID ];								\n\
		}"
	};


	static const GLchar * geom_shader_source[] =
	{ "#version 330																										\n\
			uniform float time;																							\n\
			uniform mat4 mvpMat;																						\n\
			\n\
			uniform float square1X;																						\n\
			uniform float square1Y;																						\n\
			uniform float square1Z;																						\n\
			uniform float square1W;																						\n\
			\n\
			uniform float square2X;																						\n\
			uniform float square2Y;																						\n\
			uniform float square2Z;																						\n\
			uniform float square2W;																						\n\
			\n\
			uniform float square3X;																						\n\
			uniform float square3Y;																						\n\
			uniform float square3Z;																						\n\
			uniform float square3W;																						\n\
			\n\
			uniform float distanceWall;																					\n\
			\n\
			int counterCubes = 0;																						\n\
			layout(triangles) in;																						\n\
			layout(triangle_strip,max_vertices = 48) out;																\n\
																														\n\
			void main(){																								\n\
																														\n\
				float sinus = 1;																						\n\
				float cosinus = 1;																						\n\
				vec4 square1[24]=vec4[24](vec4(distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),			\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),			\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
																																				\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),				\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
																																				\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),			\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
																																				\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(-distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),				\n\
																																				\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),			\n\
										vec4(-distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
																																				\n\
										vec4(distanceWall + square1X, -distanceWall + square1Y, -distanceWall + square1Z, square1W),			\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, -distanceWall + square1Z, square1W),				\n\
										vec4(distanceWall + square1X, -distanceWall + square1Y, distanceWall + square1Z, square1W),				\n\
										vec4(distanceWall + square1X, distanceWall + square1Y, distanceWall + square1Z, square1W));				\n\
					\n\
					vec4 square2[24]=vec4[24](vec4(distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),		\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),			\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
																																				\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),				\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
																																				\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),			\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
																																				\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(-distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),				\n\
																																				\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),			\n\
										vec4(-distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
																																				\n\
										vec4(distanceWall + square2X, -distanceWall + square2Y, -distanceWall + square2Z, square2W),			\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, -distanceWall + square2Z, square2W),				\n\
										vec4(distanceWall + square2X, -distanceWall + square2Y, distanceWall + square2Z, square2W),				\n\
										vec4(distanceWall + square2X, distanceWall + square2Y, distanceWall + square2Z, square2W));				\n\
					\n\
					vec4 square3[24]=vec4[24](vec4(distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),		\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),			\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
																																				\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),				\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
																																				\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),			\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
																																				\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(-distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),				\n\
																																				\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),			\n\
										vec4(-distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
																																				\n\
										vec4(distanceWall + square3X, -distanceWall + square3Y, -distanceWall + square3Z, square3W),			\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, -distanceWall + square3Z, square3W),				\n\
										vec4(distanceWall + square3X, -distanceWall + square3Y, distanceWall + square3Z, square3W),				\n\
										vec4(distanceWall + square3X, distanceWall + square3Y, distanceWall + square3Z, square3W));				\n\
					\n\
					if(counterCubes == 0){																			\n\
					for(int i = 0; i < 4; i++){																		\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 0;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 4; i < 8; i++){																		\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 1;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 8; i < 12; i++){																	\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 2;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 12; i < 16; i++){																	\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 3;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 16; i < 20; i++){																	\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 4;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 20; i < 24; i++){																	\n\
						gl_Position = mvpMat * square1[i];															\n\
						gl_PrimitiveID = 5;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					counterCubes++;																					\n\
				}															\n\
				if(counterCubes == 1){										\n\
					for(int i = 0; i < 4; i++){																		\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 0;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 4; i < 8; i++){																		\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 1;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 8; i < 12; i++){																	\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 2;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 12; i < 16; i++){																	\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 3;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 16; i < 20; i++){																	\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 4;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 20; i < 24; i++){																	\n\
						gl_Position = mvpMat * square2[i];															\n\
						gl_PrimitiveID = 5;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					counterCubes++;																					\n\
				}\n\
																			\n\
				if(counterCubes == 2){										\n\
					for(int i = 0; i < 4; i++){																		\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 0;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 4; i < 8; i++){																		\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 1;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 8; i < 12; i++){																	\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 2;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 12; i < 16; i++){																	\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 3;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 16; i < 20; i++){																	\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 4;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					for(int i = 20; i < 24; i++){																	\n\
						gl_Position = mvpMat * square3[i];															\n\
						gl_PrimitiveID = 5;																			\n\
						EmitVertex();																				\n\
					}																								\n\
					EndPrimitive();																					\n\
					counterCubes++;																					\n\
				}\n\
			}" };


	GLuint	ShaderCompile() {			//Punter a un espai de memoria, on tenim el nostre programa. Ens donara un valor, per invocar al programa compilat. GLuint = enter si o si positiu 
		GLuint vertex_shader;				//GLuint es un unsigned int, que guarda direccions de memoria 
		GLuint geom_shader;					//Per fer el geometry shader
		GLuint fragment_shader;
		GLuint program;

		
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);		//Es pasa a la targeta grafica 
		glCompileShader(vertex_shader);										//Ara es te de compilar 

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);						//Per fer el geometry shader
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
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


	void ShaderRenderCode(double currentTime) {
		const GLfloat color[] = { 0.0,0.0,0.0f,1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(ShaderRenderProgram);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "time"), (GLfloat)currentTime);

		//Cubo 1
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square1X"), (GLfloat)_square1[0]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square1Y"), (GLfloat)_square1[1]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square1Z"), (GLfloat)_square1[2]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square1W"), (GLfloat)_square1[3]);
		//Cubo 2
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square2X"), (GLfloat)_square2[0]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square2Y"), (GLfloat)_square2[1]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square2Z"), (GLfloat)_square2[2]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square2W"), (GLfloat)_square2[3]);
		//Cubo 3 
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square3X"), (GLfloat)_square3[0]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square3Y"), (GLfloat)_square3[1]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square3Z"), (GLfloat)_square3[2]);
		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "square3W"), (GLfloat)_square3[3]);

		glUniform1f(glGetUniformLocation(ShaderRenderProgram, "distanceWall"), (GLfloat)_distanceWall);


		glUniformMatrix4fv(glGetUniformLocation(ShaderRenderProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));	 //La matriz se aplica a mvpMat, que multiplicara a los puntos en geom_shader_source[]

		
		//glPointSize(40.0f);						//Cada vertex te un tamany de 40 pixels 
		glDrawArrays(GL_TRIANGLES, 0, 24);		//Compte, es POINTS, no POINT. El 3 es que dibuixa 3 punts 
	}

}