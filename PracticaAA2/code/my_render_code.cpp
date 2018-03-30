#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <imgui\imgui.h>
#include <iostream>
#include "GL_framework.h"

//Global variables

//Forward declarations
namespace ImGui {
	void Render();
}

namespace Scene {
	int currentScene = 1;	//1, 2, 3...
	void renderScene1();
	void detectInput();
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
}

void GLcleanup() {
	//Box::cleanupCube();
	//Axis::cleanupAxis();
	//Cube::cleanupCube();
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