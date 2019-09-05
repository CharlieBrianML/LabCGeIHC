//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes

#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"///1
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"////2


//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenWidth;
int screenHeight;

GLFWwindow * window;

Shader shader;
std::shared_ptr<FirstPersonCamera> camera (new FirstPersonCamera());///2

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);//// 1
Box box1;

bool exitApp = false;
int lastMousePosX, offsetX=0;
int lastMousePosY, offsetY=0;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {
	
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	//Inicializar los buffers VAO,VBO,EBO
	sphere1.init();
	//Metodo que setter que coloca el apuntador al shader
	sphere1.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));
	//
	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	sphere2.init();
	sphere2.setShader(&shader);
	sphere2.setColor(glm::vec4(2.0, 0.5, 1.0, 1.0));////12

	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));////1
	camera->setPosition(glm::vec3(2.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.
	//Destruccion de los VAO,EBO,VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();///1 

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;///5
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;///5
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {///5 
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){///4  Nos podemos mover con la camara hacia enfrente
		camera->moveFrontCamera(true, deltaTime);///4
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {///4  Nos podemos mover con la camara hacia atras
		camera->moveFrontCamera(false, deltaTime);///4
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {///4  Nos podemos mover con la camara hacia la izquierda
		camera->moveRightCamera(false, deltaTime);///4
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {///4  Nos podemos mover con la camara hacia la derecha
		camera->moveRightCamera(true, deltaTime);///4
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {/////5 Movimiento con el mouse
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	}
	offsetX = 0;///5
	offsetY = 0;///5
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 view = camera->getViewMatrix();///2

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);

		//Sirve para visualizar lineas
		//sphere1.enableWireMode();////1
		// Dibujado de la geometria y recibe la matriz de transicion
		//sphere1.render(model);///1

		/*cylinder1.render(model);
		cylinder1.enableWireMode();*/

		box1.enableWireMode();///6
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0,0.1)));
		//Articulacion1 
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));////7
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));////7
		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));////8
		l1 = glm::rotate(l1, glm::radians(90.0f),glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));////8
		//Articulacion2 
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));////9
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));////9
		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));////10
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));////10
		shader.turnOff();
		//Ojos
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));//11
		sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));//11
		sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
