#define _USE_MATH_DEFINES
#include <cmath>
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
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Descomentar
#include "Headers/Texture.h"

int screenWidth;
int screenHeight;

GLFWwindow* window;

Shader shader;
//Descomentar El shader de texturizado
Shader shaderTexture;
// Descomentar El shader para iluminacion
Shader shaderColorLighting;///7
//Shader para iluminar la textura
Shader shaderTextureLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box1;
Box box2;
Box box3;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0, rot5 = 0.0, rot6 = 0.0, rot7 = 0.0, rot8 = 0.0, rot9 = 0.0, rot10 = 0.0;
float rot11 = 0.0, rot12 = 0.0, rot13 = 0.0, rot14 = 0.0, rot15 = 0.0;
float rot0 = 0.0, dz = 0.0, dz2 = 0.2;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
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
	shaderTexture.initialize("../Shaders/texturizado_res.vs", "../Shaders/texturizado_res.fs");
	// Descomentar
	/*shaderColorLighting.initialize("../Shaders/iluminacion_color.vs",///7
		"../Shaders/iluminacion_color.fs");///7*/
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
		"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
		"../Shaders/iluminacion_texture_res.fs");

// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

	// Descomentar
	//Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();///10
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));//10

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);///7
	//cylinder1.setShader(&shader);///7
	cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	cylinder2.init();
	//cylinder2.setShader(&shaderTexture);
	cylinder2.setShader(&shaderTextureLighting);

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderTexture);
	//box1.setShader(&shader);//t
	//box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderTextureLighting);

	sphere3.init();
	sphere3.setShader(&shaderTextureLighting);

	box3.init();///2
	box3.setShader(&shaderTextureLighting);
	//box3.setShader(&shaderTexture);///2

	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP* bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char* data = texture1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	////Nueva textura/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)//5Espejo-GL_MIRRORED_REPEAT   6Arrastrar-GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	Texture texturePantalon("../Textures/House/pantalonCafe.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePantalon.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePantalon.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID5);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texturePantalon.freeImage(bitmap);

	Texture textureCorbata("../Textures/House/corbataBobEsponja.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureCorbata.loadImage(false);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCorbata.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID6);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCorbata.freeImage(bitmap);

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		rot2 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		rot2 -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.001;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.001;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.0001;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.0001;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.0001;

	sentido = true;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	//glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al nuevo shader
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Descomentar
		shaderColorLighting.setMatrix4("projection", 1, false, glm::value_ptr(projection));///7
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));///7
		shaderColorLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//12
		//shaderColorLighting.setVectorFloat3("viewPos", glm::value_ptr(glm::vec3(0.0, 0.0, 3.0)));//12
		shaderColorLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));////9
		shaderColorLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));//12

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,glm::value_ptr(projection));//t
		shaderTextureLighting.setMatrix4("view", 1, false,glm::value_ptr(view));//t

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",glm::value_ptr(camera->getPosition()));
		//shaderTextureLighting.setVectorFloat3("viewPos", glm::value_ptr(glm::vec3(0.0, 0.0, 3.0)));
		shaderTextureLighting.setVectorFloat3("light.ambient",glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",glm::value_ptr(glm::vec3(0.3, 0.0, 0.0)));
		shaderTextureLighting.setVectorFloat3("light.specular",glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));

		/*glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix, glm::vec3(0.0f, 0.0f, -ratio));
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(glm::vec4(lightModelmatrix* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.render(lightModelmatrix);*/

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(lightModelmatrix * glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		/*shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(
					lightModelmatrix * glm::vec4(0.0, 1.0, 0.0, 1.0))));

		sphereLamp.render(lightModelmatrix);*/

		/*model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		//Descomentar
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		//Descomentar
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		// Articulacion 1
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder2.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));*/

		/*// Articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		// Ojo
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.setFloat("offsetX", 0);

		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		sphere3.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder,
			glm::vec3(-3.0, 0.0, 0.0));
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		// Se usa la textura 1 ( Bon sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
			modelCylinder);
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
			cylinder2.getSlices() * 3, modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder2.render(
			cylinder2.getSlices() * cylinder2.getStacks() * 6
			+ cylinder2.getSlices() * 3, cylinder2.getSlices() * 3,
			modelCylinder);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cubeTextureModel = glm::mat4(1.0);///2
		cubeTextureModel = glm::translate(cubeTextureModel, glm::vec3(3.0, 2.0, 3.0));///2
		glBindTexture(GL_TEXTURE_2D, textureID4);///2
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0,1.0)));///4
		box3.render(cubeTextureModel);///2
		glBindTexture(GL_TEXTURE_2D,0);///2*/

		//------------------------------------------------------------------------------------------------------------
		glm::mat4 model = glm::mat4(1.0);

		shaderColorLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(model* glm::vec4(0.0, 0.0, 1.0, 1.0))));

		shaderTextureLighting.setVectorFloat3("light.position",
			glm::value_ptr(
				glm::vec4(model * glm::vec4(0.0, 0.0, 1.0, 1.0))));

		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box2.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Articulacion1 
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		//j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		//j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		//Articulacion2 
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));
		//shader.turnOff();
		//glBindTexture(GL_TEXTURE_2D, 0);

		//Ojos
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));
		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		//Articulacion 3
		glm::mat4 j3 = glm::translate(model, glm::vec3(-0.5, 0.0f, 0.0f));
		//sphere1.enableWireMode();
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));
		j3 = glm::rotate(j3, rot5, glm::vec3(0, 0, 1));
		j3 = glm::rotate(j3, rot6, glm::vec3(0, 1, 0));

		// Hueso 3
		glm::mat4 l3 = glm::translate(j3, glm::vec3(-0.25, 0.0, 0.0));
		l3 = glm::rotate(l3, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l3, glm::vec3(0.1, 0.5, 0.1)));
		//shader.turnOff();

		//Articulacion 4
		glm::mat4 j4 = glm::translate(j3, glm::vec3(-0.5, 0.0f, 0.0f));
		j4 = glm::rotate(j4, rot7, glm::vec3(0.0, 0.0, 1.0));
		j4 = glm::rotate(j4, rot8, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j4, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 4
		glm::mat4 l4 = glm::translate(j4, glm::vec3(-0.25, 0.0, 0.0));
		l4 = glm::rotate(l4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l4, glm::vec3(0.1, 0.5, 0.1)));
		//shader.turnOff();
		//glBindTexture(GL_TEXTURE_2D, 0);

		//Articulacion 5
		glm::mat4 j5 = glm::translate(model, glm::vec3(-0.2, -0.5, 0.0f));
		//sphere1.enableWireMode();
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		sphere1.render(glm::scale(j5, glm::vec3(0.1, 0.1, 0.1)));
		j5 = glm::rotate(j5, rot9, glm::vec3(0, 0, 1));
		j5 = glm::rotate(j5, rot10, glm::vec3(0, 1, 0));

		// Hueso 5
		glm::mat4 l5 = glm::translate(j5, glm::vec3(0.0, -0.25, 0.0));
		l5 = glm::rotate(l5, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l5, glm::vec3(0.1, 0.5, 0.1)));
		//shader.turnOff();

		//Articulacion 6
		glm::mat4 j6 = glm::translate(model, glm::vec3(0.2, -0.5, 0.0f));
		j6 = glm::rotate(j6, rot11, glm::vec3(0.0, 0.0, 1.0));
		j6 = glm::rotate(j6, rot12, glm::vec3(1.0, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j6, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 6
		glm::mat4 l6 = glm::translate(j6, glm::vec3(0.0, -0.25, 0.0));
		l6 = glm::rotate(l6, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l6, glm::vec3(0.1, 0.5, 0.1)));
		//glBindTexture(GL_TEXTURE_2D, 0);
		//shader.turnOff();

		//Articulacion 7
		glm::mat4 j7 = glm::translate(j5, glm::vec3(0.0, -0.5, 0.0f));
		//sphere1.enableWireMode();
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		sphere1.render(glm::scale(j7, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 7
		glm::mat4 l7 = glm::translate(j7, glm::vec3(0.0, -0.25, 0.0));
		l7 = glm::rotate(l7, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder2.enableWireMode();
		cylinder1.render(glm::scale(l7, glm::vec3(0.1, 0.5, 0.1)));
		//shader.turnOff();
		//glBindTexture(GL_TEXTURE_2D, 0);

		//Articulacion 8
		glm::mat4 j8 = glm::translate(j6, glm::vec3(0.0, -0.5, 0.0f));
		//sphere1.enableWireMode();
		//glBindTexture(GL_TEXTURE_2D, textureID1);
		sphere1.render(glm::scale(j8, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 8
		glm::mat4 l8 = glm::translate(j8, glm::vec3(0.0, -0.25, 0.0));
		l8 = glm::rotate(l8, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder2.enableWireMode();
		cylinder1.render(glm::scale(l8, glm::vec3(0.1, 0.5, 0.1)));
		//glBindTexture(GL_TEXTURE_2D, 0);
		//shader.turnOff();

		//Pantalones cuadrados
		glm::mat4 b2 = glm::translate(model, glm::vec3(0.0, -0.35, 0.0));
		b2 = glm::rotate(b2, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
		//box2.enableWireMode();///6
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(glm::scale(b2, glm::vec3(1.01, 0.3, 0.11)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Corbata
		glm::mat4 corbata = glm::translate(model, glm::vec3(0.0, -0.35, 0.06));
		corbata = glm::rotate(corbata, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(glm::scale(corbata, glm::vec3(1.01, 0.3, 0.01)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Iris
		glm::mat4 iris = glm::translate(model, glm::vec3(0.25, 0.25, 0.1));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(iris, glm::vec3(0.07, 0.07, 0.04)));
		glm::mat4 iris2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.1));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(iris2, glm::vec3(0.07, 0.07, 0.04)));

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		shader.turnOff();

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}


/////TAREA
/*Revisar iluminacion_texture_res.vs y iluminacion_texture_res.fs y ponerle iluminacion a bob espoja
Crear objeto tipo shader
Shader shaderTextureLinghting
Shader textureLinghtnigInit
....
box.setShader(&shaderTextureLinghtnig)
...
application loop()
copiar shader.lightnig.setMa
...
copiar
pegar
shaderTextureLightnig*/
