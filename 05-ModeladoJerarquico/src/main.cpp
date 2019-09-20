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

GLFWwindow* window;

Shader shader;
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());///2

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box1, box2, box3;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot1 = 0.0, rot2 = 0.0;///1
float rot3 = 0.0, rot4 = 0.0;///2
float rot5 = 0.0, rot6 = 0.0;
float rot7 = 0.0, rot8 = 0.0;
float rot9 = 0.0, rot10 = 0.0;
float rot11 = 0.0, rot12 = 0.0;
float rot13 = 0.0, rot14 = 0.0;
float rot15 = 0.0;
float rot0 = 0.0, dz = 0.0, dz2 = 0.2;;///Variables para rotar y desplazar en el eje z
bool sentido = true;

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
	sphere1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//
	cylinder1.init();
	cylinder1.setShader(&shader);
	//cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));
	cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphere2.init();
	sphere2.setShader(&shader);
	sphere2.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));////12

	box1.init();
	box1.setShader(&shader);
	//box1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));////1
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));////1

	box2.init();
	box2.setShader(&shader);
	//box1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));////1
	box2.setColor(glm::vec4(0.647059, 0.164706, 0.164706, 1.0));////1

	box3.init();
	box3.setShader(&shader);
	//box1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));////1
	box3.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));////1

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

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {///4  Nos podemos mover con la camara hacia enfrente
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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.001;///1
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.001;///3
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && sentido)
		rot2 += 0.001;///1
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.001;///2
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.001;///2
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && sentido)
		rot5 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && sentido)
		rot6 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && sentido)
		rot7 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && sentido)
		rot8 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && sentido)
		rot9 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && sentido)
		rot10 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && sentido)
		rot11 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && sentido)
		rot12 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && sentido)
		rot13 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && sentido)
		rot13 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && sentido)
		rot14 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && sentido)
		rot15 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && sentido)
		rot0 = 0.001;///4
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && sentido)
		rot0 = -0.001;///2
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && sentido)
		dz = 0.001;///2
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && sentido)
		dz = -0.001;///2

	offsetX = 0;///5
	offsetY = 0;///5
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 view = camera->getViewMatrix();///2

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		//glm::mat4 model = glm::mat4(1.0f);

		//Sirve para visualizar lineas
		//sphere1.enableWireMode();////1
		// Dibujado de la geometria y recibe la matriz de transicion
		//sphere1.render(model);///1

		/*cylinder1.render(model);
		cylinder1.enableWireMode();*/

///////////////////////////////////////////////////////////////////////////BOB ESPONJA
		model = glm::translate(model, glm::vec3(0, 0, dz));//Me muevo en z
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));//Rota la figura
		//box1.enableWireMode();///6
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));

		//Articulacion1 
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));////7
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));////7
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));////8
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));////8

		//Articulacion2 
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));////9
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));////9

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));////10
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Ojos
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		//Articulacion 3
		glm::mat4 j3 = glm::translate(model, glm::vec3(-0.5, 0.0f, 0.0f));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));
		j3 = glm::rotate(j3, rot5, glm::vec3(0, 0, 1));
		j3 = glm::rotate(j3, rot6, glm::vec3(0, 1, 0));

		// Hueso 3
		glm::mat4 l3 = glm::translate(j3, glm::vec3(-0.25, 0.0, 0.0));////10
		l3 = glm::rotate(l3, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l3, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Articulacion 4
		glm::mat4 j4 = glm::translate(j3, glm::vec3(-0.5, 0.0f, 0.0f));//11
		j4 = glm::rotate(j4, rot7, glm::vec3(0.0, 0.0, 1.0));
		j4 = glm::rotate(j4, rot8, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j4, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 4
		glm::mat4 l4 = glm::translate(j4, glm::vec3(-0.25, 0.0, 0.0));////10
		l4 = glm::rotate(l4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l4, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Articulacion 5
		glm::mat4 j5 = glm::translate(model, glm::vec3(-0.2, -0.5, 0.0f));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j5, glm::vec3(0.1, 0.1, 0.1)));
		j5 = glm::rotate(j5, rot9, glm::vec3(0, 0, 1));
		j5 = glm::rotate(j5, rot10, glm::vec3(0, 1, 0));

		// Hueso 5
		glm::mat4 l5 = glm::translate(j5, glm::vec3(0.0, -0.25, 0.0));////10
		l5 = glm::rotate(l5, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l5, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Articulacion 6
		glm::mat4 j6 = glm::translate(model, glm::vec3(0.2, -0.5, 0.0f));//11
		j6 = glm::rotate(j6, rot11, glm::vec3(0.0, 0.0, 1.0));
		j6 = glm::rotate(j6, rot12, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j6, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 6
		glm::mat4 l6 = glm::translate(j6, glm::vec3(0.0, -0.25, 0.0));////10
		l6 = glm::rotate(l6, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l6, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Articulacion 7
		glm::mat4 j7 = glm::translate(j5, glm::vec3(0.0,-0.5, 0.0f));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j7, glm::vec3(0.1, 0.1, 0.1)));
		
		// Hueso 7
		glm::mat4 l7 = glm::translate(j7, glm::vec3(0.0, -0.25, 0.0));////10
		l7 = glm::rotate(l7, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder2.enableWireMode();
		cylinder2.render(glm::scale(l7, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Articulacion 8
		glm::mat4 j8 = glm::translate(j6, glm::vec3(0.0, -0.5, 0.0f));//11
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j8, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 8
		glm::mat4 l8 = glm::translate(j8, glm::vec3(0.0, -0.25, 0.0));////10
		l8 = glm::rotate(l8, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder2.enableWireMode();
		cylinder2.render(glm::scale(l8, glm::vec3(0.1, 0.5, 0.1)));////10
		//shader.turnOff();

		//Pantalones cuadrados
		glm::mat4 b2 = glm::translate(model, glm::vec3(0.0, -0.35, 0.0));////10
		b2 = glm::rotate(b2, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
		//box2.enableWireMode();///6
		box2.render(glm::scale(b2, glm::vec3(1.01, 0.3, 0.11)));

		//Iris
		glm::mat4 iris = glm::translate(model, glm::vec3(0.25, 0.25, 0.1));//11
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(iris, glm::vec3(0.07, 0.07, 0.04)));
		glm::mat4 iris2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.1));//11
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(iris2, glm::vec3(0.07, 0.07, 0.04)));

		//shader.turnOff();

///////////////////////////////////////////////////////////////////////////R2D2

		//Cabeza
		model2 = glm::translate(model, glm::vec3(3.0, 0.0, 0.0));
		model2 = glm::rotate(model2, rot13, glm::vec3(0, 1, 0));
		sphere1.render(glm::scale(model2, glm::vec3(1.0, 1.0, 1.0)));

		//Cuerpo 
		glm::mat4 l9 = glm::translate(model2, glm::vec3(0.0, -0.6, 0.0f));
		//l9 = glm::rotate(l9, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder2.render(glm::scale(l9, glm::vec3(1.0, 1.0, 1.0)));
		//j9 = glm::rotate(j9, rot14, glm::vec3(0, 0, 1));
		//j9 = glm::rotate(j9, rot15, glm::vec3(0, 1, 0));

		//Brazo robótico 1
		glm::mat4 j10 = glm::translate(l9, glm::vec3(0.61, -0.1, 0.0));
		j10 = glm::rotate(j10, rot14, glm::vec3(1.0, 0.0, 0.0));
		box3.render(glm::scale(j10, glm::vec3(0.15, 1.3, 0.15)));

		//Brazo robótico 2
		glm::mat4 j11 = glm::translate(l9, glm::vec3(-0.61, -0.1, 0.0));
		j11 = glm::rotate(j11, rot15, glm::vec3(1.0, 0.0, 0.0));
		box3.render(glm::scale(j11, glm::vec3(0.15, 1.3, 0.15)));

		//Pie robótico 1
		glm::mat4 j12 = glm::translate(j10, glm::vec3(0.0, -0.7, 0.0));
		box3.render(glm::scale(j12, glm::vec3(0.15, 0.15, 0.4)));

		//Pie robótico 2
		glm::mat4 j13 = glm::translate(j11, glm::vec3(0.0, -0.7, 0.0));
		box3.render(glm::scale(j13, glm::vec3(0.15, 0.15, 0.4)));

		//Sosten del cuerpo
		glm::mat4 l10 = glm::translate(l9, glm::vec3(0.0, -0.3, 0.0f));
		cylinder2.render(glm::scale(l10, glm::vec3(0.7, 0.7, 0.7)));
		glm::mat4 l11 = glm::translate(l10, glm::vec3(0.0, -0.3, 0.0f));
		cylinder2.render(glm::scale(l11, glm::vec3(0.3, 0.3, 0.3)));
		glm::mat4 j14 = glm::translate(l11, glm::vec3(0.0, -0.2, 0.0));
		box3.render(glm::scale(j14, glm::vec3(0.15, 0.15, 0.3)));

		//Ojo robot
		glm::mat4 eye = glm::translate(model2, glm::vec3(0.0, 0.04, 0.45));
		sphere2.render(glm::scale(eye, glm::vec3(0.2, 0.2, 0.17)));


		//shader.turnOff();
		dz = 0;////
		dz2 = 0;
		rot0 = 0;//////
		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
