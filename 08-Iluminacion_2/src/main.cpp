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

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;//6
// Descomentar
//Shader con skybox
Shader shaderSkybox;//3
//Shader multiples luces
Shader shaderMulLighting;//11

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Sphere sphereLamp(20, 20);
Sphere sphereLamp2(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinderBaseLamp(20, 20, 0.5, 0.5);
// Descomentar
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);//6
Box box1;
Box box2;
Box box3;
Box paredKaki;
Box paredGoldenrod;
Box paredOrangeRed;
Box techo;
Box boxMaterials;//9

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;
GLuint textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16;
// Descomentar
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_badomen/badomen_ft.tga",//5
		"../Textures/mp_badomen/badomen_bk.tga",
		"../Textures/mp_badomen/badomen_up.tga",
		"../Textures/mp_badomen/badomen_dn.tga",
		"../Textures/mp_badomen/badomen_rt.tga",
		"../Textures/mp_badomen/badomen_lf.tga" };//5

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
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
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
			"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
			"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/iluminacion_texture_res.fs");
	// Descomentar
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
	 "../Shaders/iluminacion_material.fs");//6
	// Descomentar
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",//3
			"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("..//Shaders/iluminacion_texture_res.vs",//11
		"..//Shaders/multipleLights.fs");//11

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);///1
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);///1
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderTextureLighting);

	// Descomentar
	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);//6
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));//6

	// Descomentar
	skyboxSphere.init();//3
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));///3

	boxMaterials.init();//9
	boxMaterials.setShader(&shaderMaterialLighting);//9

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderTextureLighting);//11
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderTextureLighting);///2

	sphere3.init();
	sphere3.setShader(&shaderTextureLighting);//2

	box3.init();
	box3.setShader(&shaderTextureLighting);//2

	sphereLamp2.init();
	sphereLamp2.setShader(&shaderTextureLighting);
	//sphereLamp2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinderBaseLamp.init();
	cylinderBaseLamp.setShader(&shaderTextureLighting);

	//Inicializamos las paredes
	paredKaki.init();
	paredKaki.setShader(&shaderColorLighting);
	paredKaki.setColor(glm::vec4(0.623529, 0.623529, 0.372549, 1.0));

	paredGoldenrod.init();
	paredGoldenrod.setShader(&shaderTextureLighting);
	//paredGoldenrod.setColor(glm::vec4(0.917647, 0.917647, 0.678431, 1.0));

	paredOrangeRed.init();
	paredOrangeRed.setShader(&shaderTextureLighting);
	//paredOrangeRed.setColor(glm::vec4(1.0, 0.25, 0.0, 1.0));

	techo.init();
	techo.setShader(&shaderTextureLighting);
	//techo.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));

	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
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
	} else
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
	} else
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
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

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
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	//------------------------------------>Textura paredes
	Texture texturePared("../Textures/House/pared1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePared.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePared.convertToData(bitmap, imageWidth, imageHeight);
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
	texturePared.freeImage(bitmap);

	Texture textureWallOrange("../Textures/House/pared3.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureWallOrange.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWallOrange.convertToData(bitmap, imageWidth, imageHeight);
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
	textureWallOrange.freeImage(bitmap);

	Texture textureAzulejo("../Textures/House/azulejo1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureAzulejo.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureAzulejo.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID7);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID7);
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
	textureAzulejo.freeImage(bitmap);

	Texture textureTecho("../Textures/House/techo1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureTecho.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTecho.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID8);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID8);
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
	textureTecho.freeImage(bitmap);

	Texture textureEscalon("../Textures/House/escalon1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureEscalon.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureEscalon.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID9);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID9);
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
	textureEscalon.freeImage(bitmap);

	Texture texturePuerta1("../Textures/House/puerta1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePuerta1.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePuerta1.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID10);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID10);
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
	texturePuerta1.freeImage(bitmap);

	Texture texturePuerta2("../Textures/House/puerta2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePuerta2.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePuerta2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID11);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID11);
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
	texturePuerta2.freeImage(bitmap);

	Texture texturePuerta3("../Textures/House/puerta3.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePuerta3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePuerta3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID12);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID12);
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
	texturePuerta3.freeImage(bitmap);

	Texture textureVentana1("../Textures/House/ventana1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureVentana1.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureVentana1.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID13);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID13);
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
	textureVentana1.freeImage(bitmap);

	Texture textureMadera1("../Textures/House/madera2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureMadera1.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureMadera1.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID14);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID14);
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
	textureMadera1.freeImage(bitmap);

	Texture textureVidrio("../Textures/House/vidrioLampara.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureVidrio.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureVidrio.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID15);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID15);
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
	textureVidrio.freeImage(bitmap);

	Texture textureGround("../Textures/House/pasto1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureGround.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureGround.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID16);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID16);
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
	textureGround.freeImage(bitmap);

	// Descomentar
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");//3
	glGenTextures(1, &skyboxTextureID);
	//Tipo de textura CUBE_MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}///3

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

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
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

	glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 5.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con material
		// Descomentar
		shaderMaterialLighting.setMatrix4("projection", 1, false,//6
		 glm::value_ptr(projection));
		 shaderMaterialLighting.setMatrix4("view", 1, false,
		 glm::value_ptr(view));//6

		// Settea la matriz de vista y projection al shader con skybox
		// Descomentar
		shaderSkybox.setMatrix4("projection", 1, false,//3
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				//glm::value_ptr(view));//3
		glm::value_ptr(glm::mat4(glm::mat3(view))));//4
		//Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,//11
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(view));//11

		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		// Descomentar
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//6
		 shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		 shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		 shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));//6

		 //Propiedades de la luz para los objetos con multiples luces
		 shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));//11
		 shaderMulLighting.setVectorFloat3("directionalLight.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		 shaderMulLighting.setVectorFloat3("directionalLight.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		 shaderMulLighting.setVectorFloat3("directionalLight.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));//11
		 shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.0, 0.0, -1.0)));//11

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
				glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
				glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
										* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
										* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con materiales
		// Descomentar
		shaderMaterialLighting.setVectorFloat3("light.position",//6
				glm::value_ptr(
						glm::vec4(
							lightModelmatrix
										* glm::vec4(0.0, 0.0, 0.0, 1.0))));//6
		sphereLamp.render(lightModelmatrix);

		model = glm::translate(model, glm::vec3(0, 0, dz));
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
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		// Articulacion 2
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

		// Render del cubo con textura de ladrillos y con repeticion en x
		glm::mat4 cubeTextureModel = glm::mat4(1.0);
		cubeTextureModel = glm::translate(cubeTextureModel,
				glm::vec3(3.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
				glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(cubeTextureModel);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render del cyindro con materiales
		// Descomentar
		glm::mat4 cylinderMaterialModel = glm::mat4(1.0);//6
		 cylinderMaterialModel = glm::translate(cylinderMaterialModel,  glm::vec3(3.0, 2.0, -3.0));
		 shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.329412f, 0.223529f, 0.027451f)));
		 shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.780392f, 0.568627f, 0.113725f)));
		 shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.992157f, 0.941176f, 0.807843f)));
		 shaderMaterialLighting.setFloat("material.shininess", 27.8974f);
		 cylinderMaterials.render(cylinderMaterialModel);//6

		 glm::mat4 boxMaterialModel = glm::mat4(1.0f);//9
		 boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-3.0, 2.0, -3.0));
		 shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.0215f, 0.1745f, 0.0215f)));//10
		 shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.07568f, 0.61424f, 0.07568f)));
		 shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.633f, 0.727811f, 0.633f)));
		 shaderMaterialLighting.setFloat("material.shininess", 76.8f);//10
		 boxMaterials.render(boxMaterialModel);//9

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Descomentar
		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		//Se deshabilitan el modo del recorte de caras ocultas para ver la esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);//3
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);//3

		//Iniciamos a definir la casa----------------------------------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 1 - Pared derecha de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model = glm::mat4(1.0);
		pared1Model = glm::translate(pared1Model, glm::vec3(15.0, 0.0, 0.0));
		pared1Model = glm::rotate(pared1Model, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model = glm::scale(pared1Model, glm::vec3(18.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 2 - Pared izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model2 = glm::mat4(1.0);
		pared1Model2 = glm::translate(pared1Model2, glm::vec3(8.0, 0.0, 0.0));
		pared1Model2 = glm::rotate(pared1Model2, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model2 = glm::scale(pared1Model2, glm::vec3(18.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 3 - Parte trasera de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model3 = glm::mat4(1.0);
		pared1Model3 = glm::translate(pared1Model3, glm::vec3(11.5, 0.0, -9.0));
		pared1Model3 = glm::scale(pared1Model3, glm::vec3(7.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model3);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model4 = glm::mat4(1.0);
		pared1Model4 = glm::translate(pared1Model4, glm::vec3(11.5, 5.0, 0.0));
		pared1Model4 = glm::rotate(pared1Model4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model4 = glm::rotate(pared1Model4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model4 = glm::scale(pared1Model4, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Piso planta baja<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<h
		glm::mat4 pared1Model5 = glm::mat4(1.0);
		pared1Model5 = glm::translate(pared1Model5, glm::vec3(11.5, -5.0, 0.0));
		pared1Model5 = glm::rotate(pared1Model5, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model5 = glm::rotate(pared1Model5, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model5 = glm::scale(pared1Model5, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model5);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo 1er Piso<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model6 = glm::mat4(1.0);
		pared1Model6 = glm::translate(pared1Model6, glm::vec3(11.5, 0.0, -5.4));
		pared1Model6 = glm::rotate(pared1Model6, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model6 = glm::rotate(pared1Model6, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model6 = glm::scale(pared1Model6, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model7 = glm::mat4(1.0);
		pared1Model7 = glm::translate(pared1Model7, glm::vec3(11.5, 0.0, 5.4));
		pared1Model7 = glm::rotate(pared1Model7, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model7 = glm::rotate(pared1Model7, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model7 = glm::scale(pared1Model7, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model7);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model8 = glm::mat4(1.0);
		pared1Model8 = glm::translate(pared1Model8, glm::vec3(9.5, 0.0, 0.0));
		pared1Model8 = glm::rotate(pared1Model8, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model8 = glm::rotate(pared1Model8, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model8 = glm::scale(pared1Model8, glm::vec3(3.6, 3.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model8);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Paredes de las habitaciones<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model9 = glm::mat4(1.0);
		pared1Model9 = glm::translate(pared1Model9, glm::vec3(13.0, 0.0, -1.8));
		pared1Model9 = glm::scale(pared1Model9, glm::vec3(4.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model9);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model10 = glm::mat4(1.0);
		pared1Model10 = glm::translate(pared1Model10, glm::vec3(13.0, 2.5, 1.8));
		pared1Model10 = glm::scale(pared1Model10, glm::vec3(4.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model10);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model11 = glm::mat4(1.0);
		pared1Model11 = glm::translate(pared1Model11, glm::vec3(9.5, 4.5, 1.8));
		pared1Model11 = glm::scale(pared1Model11, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model11);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model12 = glm::mat4(1.0);
		pared1Model12 = glm::translate(pared1Model12, glm::vec3(8.25, 2.0, 1.8));
		pared1Model12 = glm::scale(pared1Model12, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model12);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model13 = glm::mat4(1.0);
		pared1Model13 = glm::translate(pared1Model13, glm::vec3(10.75, 2.0, 1.8));
		pared1Model13 = glm::scale(pared1Model13, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model13);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta1 = glm::mat4(1.0);
		puerta1 = glm::translate(puerta1, glm::vec3(9.5, 2.0, 1.8));
		puerta1 = glm::scale(puerta1, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model14 = glm::mat4(1.0);
		pared1Model14 = glm::translate(pared1Model14, glm::vec3(9.5, 4.5, -1.8));
		pared1Model14 = glm::scale(pared1Model14, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model14);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model15 = glm::mat4(1.0);
		pared1Model15 = glm::translate(pared1Model15, glm::vec3(8.25, 2.0, -1.8));
		pared1Model15 = glm::scale(pared1Model15, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model15);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model16 = glm::mat4(1.0);
		pared1Model16 = glm::translate(pared1Model16, glm::vec3(10.75, 2.0, -1.8));
		pared1Model16 = glm::scale(pared1Model16, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model16);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta2 = glm::mat4(1.0);
		puerta2 = glm::translate(puerta2, glm::vec3(9.5, 2.0, -1.8));
		puerta2 = glm::scale(puerta2, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 escaleraModel1 = glm::mat4(1.0);
		escaleraModel1 = glm::translate(escaleraModel1, glm::vec3(14.375, -3.75, 0.0));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel1 = glm::scale(escaleraModel1, glm::vec3(1.25, 2.5, 3.6));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel2 = glm::mat4(1.0);
		escaleraModel2 = glm::translate(escaleraModel2, glm::vec3(13.4, -4.0, 0.9));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel2 = glm::scale(escaleraModel2, glm::vec3(0.7, 2.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel3 = glm::mat4(1.0);
		escaleraModel3 = glm::translate(escaleraModel3, glm::vec3(12.7, -4.25, 0.9));
		escaleraModel3 = glm::scale(escaleraModel3, glm::vec3(0.7, 1.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel4 = glm::mat4(1.0);
		escaleraModel4 = glm::translate(escaleraModel4, glm::vec3(12.0, -4.5, 0.9));
		escaleraModel4 = glm::scale(escaleraModel4, glm::vec3(0.7, 1.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel5 = glm::mat4(1.0);
		escaleraModel5 = glm::translate(escaleraModel5, glm::vec3(11.35, -4.75, 0.9));
		escaleraModel5 = glm::scale(escaleraModel5, glm::vec3(0.7, 0.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel5);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel6 = glm::mat4(1.0);
		escaleraModel6 = glm::translate(escaleraModel6, glm::vec3(13.4, -2.1875, -0.9));
		escaleraModel6 = glm::scale(escaleraModel6, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel7 = glm::mat4(1.0);
		escaleraModel7 = glm::translate(escaleraModel7, glm::vec3(12.7, -1.5625, -0.9));
		escaleraModel7 = glm::scale(escaleraModel7, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel7);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel8 = glm::mat4(1.0);
		escaleraModel8 = glm::translate(escaleraModel8, glm::vec3(12.0, -0.9375, -0.9));
		escaleraModel8 = glm::scale(escaleraModel8, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel9 = glm::mat4(1.0);
		escaleraModel9 = glm::translate(escaleraModel9, glm::vec3(11.35, -0.3125, -0.9));
		escaleraModel9 = glm::scale(escaleraModel9, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel9);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared de las escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredEscaleraModel1 = glm::mat4(1.0);
		paredEscaleraModel1 = glm::translate(paredEscaleraModel1, glm::vec3(11.35, -2.8125, 0.0));
		paredEscaleraModel1 = glm::scale(paredEscaleraModel1, glm::vec3(0.7, 4.375, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel2 = glm::mat4(1.0);
		paredEscaleraModel2 = glm::translate(paredEscaleraModel2, glm::vec3(12.05, -3.125, 0.0));
		paredEscaleraModel2 = glm::scale(paredEscaleraModel2, glm::vec3(0.7, 3.75, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel3 = glm::mat4(1.0);
		paredEscaleraModel3 = glm::translate(paredEscaleraModel3, glm::vec3(12.75, -3.4375, 0.0));
		paredEscaleraModel3 = glm::scale(paredEscaleraModel3, glm::vec3(0.7, 3.125, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel4 = glm::mat4(1.0);
		paredEscaleraModel4 = glm::translate(paredEscaleraModel4, glm::vec3(13.45, -3.75, 0.0));
		paredEscaleraModel4 = glm::scale(paredEscaleraModel4, glm::vec3(0.7, 2.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell15 = glm::mat4(1.0);
		paredEscaleraModell15 = glm::translate(paredEscaleraModell15, glm::vec3(11.0, -0.4375, -0.9));
		paredEscaleraModell15 = glm::scale(paredEscaleraModell15, glm::vec3(0.01, 0.375, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModell15);
		glBindTexture(GL_TEXTURE_2D, 0);

		/*glm::mat4 paredEscaleraModell16 = glm::mat4(1.0);
		paredEscaleraModell16 = glm::translate(paredEscaleraModell16, glm::vec3(11.0, -3.0, 1.55));
		paredEscaleraModell16 = glm::scale(paredEscaleraModell16, glm::vec3(0.01, 3.0, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID4);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(paredEscaleraModell16);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell17 = glm::mat4(1.0);
		paredEscaleraModell17 = glm::translate(paredEscaleraModell17, glm::vec3(11.0, -3.0, -1.55));
		paredEscaleraModell17 = glm::scale(paredEscaleraModell17, glm::vec3(0.01, 3.0, 0.5));
		//glBindTexture(GL_TEXTURE_2D, textureID4);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(paredEscaleraModell17);
		//glBindTexture(GL_TEXTURE_2D, 0);*/

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared frontal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredFrontalModel1 = glm::mat4(1.0);
		paredFrontalModel1 = glm::translate(paredFrontalModel1, glm::vec3(11.5, 4.25, 9.0));
		paredFrontalModel1 = glm::scale(paredFrontalModel1, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel2 = glm::mat4(1.0);
		paredFrontalModel2 = glm::translate(paredFrontalModel2, glm::vec3(11.5, 0.75, 9.0));
		paredFrontalModel2 = glm::scale(paredFrontalModel2, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel3 = glm::mat4(1.0);
		paredFrontalModel3 = glm::translate(paredFrontalModel3, glm::vec3(14.25, 2.5, 9.0));
		paredFrontalModel3 = glm::scale(paredFrontalModel3, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel4 = glm::mat4(1.0);
		paredFrontalModel4 = glm::translate(paredFrontalModel4, glm::vec3(9.5, 2.5, 9.0));
		paredFrontalModel4 = glm::scale(paredFrontalModel4, glm::vec3(3.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta y ventana principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model17 = glm::mat4(1.0);
		pared1Model17 = glm::translate(pared1Model17, glm::vec3(11.5, -0.5, 9.0));
		pared1Model17 = glm::scale(pared1Model17, glm::vec3(7.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model17);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model18 = glm::mat4(1.0);
		pared1Model18 = glm::translate(pared1Model18, glm::vec3(8.25, -3.0, 9.0));
		pared1Model18 = glm::scale(pared1Model18, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model18);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model19 = glm::mat4(1.0);
		pared1Model19 = glm::translate(pared1Model19, glm::vec3(10.75, -3.0, 9.0));
		pared1Model19 = glm::scale(pared1Model19, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model19);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model110 = glm::mat4(1.0);
		pared1Model110 = glm::translate(pared1Model110, glm::vec3(14.75, -3.0, 9.0));
		pared1Model110 = glm::scale(pared1Model110, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model110);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model111 = glm::mat4(1.0);
		pared1Model111 = glm::translate(pared1Model111, glm::vec3(12.75, -4.75, 9.0));
		pared1Model111 = glm::scale(pared1Model111, glm::vec3(3.5, 0.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model111);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta3 = glm::mat4(1.0);
		puerta3 = glm::translate(puerta3, glm::vec3(9.5, -3.0, 9.0));
		puerta3 = glm::scale(puerta3, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta4 = glm::mat4(1.0);
		puerta4 = glm::translate(puerta4, glm::vec3(11.0, -3.0, -0.9));
		puerta4 = glm::scale(puerta4, glm::vec3(0.01, 4.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ventanas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 window1 = glm::mat4(1.0);
		window1 = glm::translate(window1, glm::vec3(12.25, 2.5, 9.0));
		window1 = glm::scale(window1, glm::vec3(2.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(window1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window2 = glm::mat4(1.0);
		window2 = glm::translate(window2, glm::vec3(12.75, -2.75, 9.0));
		window2 = glm::scale(window2, glm::vec3(3.5, 3.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(window2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Lamparas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 modelBaseLamp1 = glm::mat4(1.0);
		modelBaseLamp1 = glm::translate(modelBaseLamp1, glm::vec3(11.5, -0.05, 4.0));
		modelBaseLamp1 = glm::scale(modelBaseLamp1, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp1 = glm::mat4(1.0);
		modelLamp1 = glm::translate(modelLamp1, glm::vec3(11.5, -0.1, 4.0));
		modelLamp1 = glm::scale(modelLamp1, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp2 = glm::mat4(1.0);
		modelBaseLamp2 = glm::translate(modelBaseLamp2, glm::vec3(11.5, -0.05, -4.0));
		modelBaseLamp2 = glm::scale(modelBaseLamp2, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp2 = glm::mat4(1.0);
		modelLamp2 = glm::translate(modelLamp2, glm::vec3(11.5, -0.1, -4.0));
		modelLamp2 = glm::scale(modelLamp2, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp3 = glm::mat4(1.0);
		modelBaseLamp3 = glm::translate(modelBaseLamp3, glm::vec3(11.5, 4.95, -5.4));
		modelBaseLamp3 = glm::scale(modelBaseLamp3, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp3 = glm::mat4(1.0);
		modelLamp3 = glm::translate(modelLamp3, glm::vec3(11.5, 4.9, -5.4));
		modelLamp3 = glm::scale(modelLamp3, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp4 = glm::mat4(1.0);
		modelBaseLamp4 = glm::translate(modelBaseLamp4, glm::vec3(11.5, 4.95, 5.4));
		modelBaseLamp4 = glm::scale(modelBaseLamp4, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp4 = glm::mat4(1.0);
		modelLamp4 = glm::translate(modelLamp4, glm::vec3(11.5, 4.9, 5.4));
		modelLamp4 = glm::scale(modelLamp4, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Terreno<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 modelGround = glm::mat4(1.0);
		modelGround = glm::translate(modelGround, glm::vec3(0.0, -5.2, 0.0));
		modelGround = glm::rotate(modelGround, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelGround = glm::scale(modelGround, glm::vec3(100.0, 100.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID16);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(modelGround);
		glBindTexture(GL_TEXTURE_2D, 0);

		dz = 0;
		rot0 = 0;
		offX += 0.001;

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

//TAREA
//Hacer modelo de una casa y poner el bob espoja y arturito en ella
