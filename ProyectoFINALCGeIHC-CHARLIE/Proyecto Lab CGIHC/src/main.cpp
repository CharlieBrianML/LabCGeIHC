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

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

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
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;

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
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Cylinder cylinderBaseLamp(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box1;
Box box2;
Box box3;
Box boxCesped;
Box boxWall;
Box paredKaki;
Box paredGoldenrod;
Box paredOrangeRed;
Box techo;
// Models complex instances
Model modelRock;
Model modelRailRoad;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelLambo;
Model modelLamboLeftDor;
Model modelPickupChasis;
Model modelPickupLlanta;
Model modelPickupPuerta;
Model modelTV;
Model modelBed, modelBed2, modelBed3, modelBed4, modelBed5, modelBed6, modelBed7;
Model modelLamp1;
Model modelChristmasTree;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;
GLuint textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16;
GLuint textureID17, textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureID24;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

/*std::string fileNames[6] = { "../Textures/hw_nightsky/nightsky_ft.tga",
		"../Textures/hw_nightsky/nightsky_bk.tga",
		"../Textures/hw_nightsky/nightsky_up.tga",
		"../Textures/hw_nightsky/nightsky_dn.tga",
		"../Textures/hw_nightsky/nightsky_rt.tga",
		"../Textures/hw_nightsky/nightsky_lf.tga" };*/

/*std::string fileNames[6] = { "../Textures/ame_starfield/starfield_ft.tga",
		"../Textures/ame_starfield/starfield_bk.tga",
		"../Textures/ame_starfield/starfield_up.tga",
		"../Textures/ame_starfield/starfield_dn.tga",
		"../Textures/ame_starfield/starfield_rt.tga",
		"../Textures/ame_starfield/starfield_lf.tga" };*/

std::string fileNames[6] = { "../Textures/hw_deepsea/underwater_ft.tga",
		"../Textures/hw_deepsea/underwater_bk.tga",
		"../Textures/hw_deepsea/underwater_up.tga",
		"../Textures/hw_deepsea/underwater_dn.tga",
		"../Textures/hw_deepsea/underwater_rt.tga",
		"../Textures/hw_deepsea/underwater_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
float rotDartHead = 0.0, rotDartBody = 0.0, advanceDartBody = 0.0, rotDartLeftArm = 0.0,
		rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;
std::vector<std::vector<glm::mat4>> keyFramesModel;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesJoints;
int indexFrameJoints = 0;
int indexFrameJointsNext = 1;
float interpolationJoints = 0.0;
int maxNumPasosJoints = 20;
int numPasosJoints = 0;

double deltaTime;
double currTime, lastTime;

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
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
			"../Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
			"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);
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
	cylinder2.setShader(&shaderMulLighting);

	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderMulLighting);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderMulLighting);

	sphere3.init();
	sphere3.setShader(&shaderMulLighting);

	box3.init();
	box3.setShader(&shaderMulLighting);

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWall.init();
	boxWall.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	//Inicializamos las paredes
	paredKaki.init();
	paredKaki.setShader(&shaderColorLighting);
	paredKaki.setColor(glm::vec4(0.623529, 0.623529, 0.372549, 1.0));

	paredGoldenrod.init();
	paredGoldenrod.setShader(&shaderMulLighting);
	//paredGoldenrod.setColor(glm::vec4(0.917647, 0.917647, 0.678431, 1.0));

	paredOrangeRed.init();
	paredOrangeRed.setShader(&shaderMulLighting);
	//paredOrangeRed.setColor(glm::vec4(1.0, 0.25, 0.0, 1.0));

	techo.init();
	techo.setShader(&shaderMulLighting);
	//techo.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));

	sphereLamp2.init();
	sphereLamp2.setShader(&shaderMulLighting);
	//sphereLamp2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinderBaseLamp.init();
	cylinderBaseLamp.setShader(&shaderMulLighting);

	/*boxWall.init();
	boxWall.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	modelTV.loadModel("../models/tv/OBJ/Samsung LED TV.obj");
	modelTV.setShader(&shaderMulLighting);//9

	modelBed.loadModel("../models/bed/bed2.obj");
	modelBed.setShader(&shaderMulLighting);

	modelBed2.loadModel("../models/bed2/bed 2.obj");
	modelBed2.setShader(&shaderMulLighting);

	modelBed3.loadModel("../models/bed3/Obj/Bed 08.obj");
	modelBed3.setShader(&shaderMulLighting);

	modelBed4.loadModel("../models/toilet/toilet.obj");
	modelBed4.setShader(&shaderMulLighting);

	modelBed5.loadModel("../models/chair/chair strong_01.obj");
	modelBed5.setShader(&shaderMulLighting);

	modelBed6.loadModel("../models/furniture/Obj/Sofa collection.obj");
	modelBed6.setShader(&shaderMulLighting);

	modelBed7.loadModel("../models/table/tbl034.obj");
	modelBed7.setShader(&shaderMulLighting);

	modelLamp1.loadModel("../models/lamp/EFG8012A-ORB.obj");
	modelLamp1.setShader(&shaderMulLighting);*/

	modelChristmasTree.loadModel("../models/christmas tree/12150_Christmas_Tree_V2_L2.obj");
	modelChristmasTree.setShader(&shaderMulLighting);

	/*modelLamborginhi.loadModel("../models/car/Lamborginhi Aventador OBJ/Lamborghini_Aventador.obj");
	modelLamborginhi.setShader(&shaderMulLighting); */

	//modelPickup.loadModel("../models/car/Pickup/L200-OBJ.obj");
	//modelPickup.setShader(&shaderMulLighting);

	// Dart Lego
	/*modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);*/

	camera->setPosition(glm::vec3(0.0, -3.0, 20.0));

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
	Texture texture2("../Textures/House/tierra.jpg");
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
	Texture texturePared("../Textures/House/pared4.jpg");
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
	bitmap = textureEscalon.loadImage();
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
	bitmap = texturePuerta1.loadImage();
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
	bitmap = texturePuerta2.loadImage();
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
	bitmap = texturePuerta3.loadImage();
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
	bitmap = textureVentana1.loadImage();
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
	glGenTextures(1, &textureID18);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID18);
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

	Texture textureMetal("../Textures/House/metal.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = textureMetal.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureMetal.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID19);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID19);
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
	textureMetal.freeImage(bitmap);

	Texture texturePantalon("../Textures/House/pantalonCafe.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texturePantalon.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePantalon.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID20);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID20);
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
	glGenTextures(1, &textureID21);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID21);
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


	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
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
	}

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 2)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_rotation.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesJoints = getKeyRotFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Condiciones para Mover el modelo del bob sponja
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 -= 0.01;
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 -= 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 -= 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 -= 0.01;

	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.1;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.1;
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.1;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.1;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotDartBody = 0.02;
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotDartBody = -0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		advanceDartBody = -0.02;
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		advanceDartBody = 0.02;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 30.0;
	glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(20, 0, 10.0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;

	glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(-10, 10, -4.0));
	float rotHelHelY = 0.0;

	int stateDoor = 0;
	float dorRotCount = 0.0;

	glm::mat4 modelMatrixDart = glm::mat4(1.0f);
	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));
	modelMatrixDart = glm::scale(modelMatrixDart, glm::vec3(0.5, 0.5, 0.5));

	// Variables to interpolation key frames

	fileName = "../animaciones/animation_rotation.txt";
	keyFramesJoints = getKeyRotFrames(fileName);

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<glm::mat4> matrixModel;
		std::vector<float> matrixJoints;
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
		shaderMaterialLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
				 glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz para objetos con SOLO color
		 *******************************************/
		shaderColorLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz para objetos con SOLO texturas
		 *******************************************/
		shaderTextureLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz para objetos con SOLO materiales
		 *******************************************/
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getFront()));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.009);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 3);
		shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-5.1, 4.5, -3.5))));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[1].position", glm::value_ptr((glm::vec3(-5.1, 4.8, -5.0))));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[1].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[1].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[1].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[2].position", glm::value_ptr((glm::vec3(-5.1, 4.5, -6.5))));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[2].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[2].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[2].quadratic", 0.004);

		/*******************************************
		 * Modelo de Luces dentro de la casa
		 *******************************************/
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.1, 4.5, -3.5));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.1, 4.8, -5.0));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.1, 4.5, -6.5));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();


		/*******************************************
		 * Luz que gira, solo se ocupa con los objetos con un solo color, materiales y con solo textura
		 * Importante: No sirve con objetos con la luz direccional, spots y points
		 *******************************************/
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
		shaderMaterialLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
									* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.setScale(glm::vec3(1, 1, 1));
		sphereLamp.setPosition(glm::vec3(0, 0, 0));
		sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereLamp.render(lightModelmatrix);


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
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Terreno<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 modelGround = glm::mat4(1.0);
		modelGround = glm::translate(modelGround, glm::vec3(0.0, -5.2, 0.0));
		modelGround = glm::rotate(modelGround, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelGround = glm::scale(modelGround, glm::vec3(100.0, 100.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(modelGround);
		glBindTexture(GL_TEXTURE_2D, 0);

		//----------------------------------------------Modelos----------------------------------------------------
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Camas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		/**glm::mat4 matrixModelBed = glm::mat4(1.0);
		matrixModelBed = glm::translate(matrixModelBed, glm::vec3(13.2, 0.0, 5.0));
		matrixModelBed = glm::rotate(matrixModelBed, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBed = glm::scale(matrixModelBed, glm::vec3(0.2, 0.2, 0.2));
		modelBed.render(matrixModelBed);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelBed2 = glm::mat4(1.0);
		matrixModelBed2 = glm::translate(matrixModelBed2, glm::vec3(14.9, 0.0, -5.0));
		matrixModelBed2 = glm::rotate(matrixModelBed2, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBed2 = glm::scale(matrixModelBed2, glm::vec3(0.02, 0.02, 0.02));
		modelBed2.render(matrixModelBed2);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelBed3 = glm::mat4(1.0);
		matrixModelBed3 = glm::translate(matrixModelBed3, glm::vec3(5.0, 4.0, 5.0));
		//matrixModelBed = glm::rotate(matrixModelBed, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelBed3 = glm::scale(matrixModelBed3, glm::vec3(0.001, 0.001, 0.001));
		//matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		modelBed3.render(matrixModelBed3);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		//glActiveTexture(GL_TEXTURE0);/

		glm::mat4 matrixModelToilet = glm::mat4(1.0);
		matrixModelToilet = glm::translate(matrixModelToilet, glm::vec3(13.0, -5.0, -0.9));
		matrixModelToilet = glm::rotate(matrixModelToilet, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelToilet = glm::scale(matrixModelToilet, glm::vec3(0.002, 0.002, 0.002));
		modelBed4.render(matrixModelToilet);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair = glm::mat4(1.0);
		matrixModelChair = glm::translate(matrixModelChair, glm::vec3(13.0, -5.0, -6.0));
		matrixModelChair = glm::scale(matrixModelChair, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelSofa = glm::mat4(1.0);
		matrixModelSofa = glm::translate(matrixModelSofa, glm::vec3(10.0, -5.0, 4.5));
		matrixModelSofa = glm::rotate(matrixModelSofa, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		matrixModelSofa = glm::scale(matrixModelSofa, glm::vec3(0.0022, 0.0022, 0.0022));
		modelBed6.render(matrixModelSofa);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable = glm::mat4(1.0);
		matrixModelTable = glm::translate(matrixModelTable, glm::vec3(13.0, -5.0, -4.0));
		matrixModelTable = glm::rotate(matrixModelTable, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//matrixModelTable = glm::rotate(matrixModelTable, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTable = glm::scale(matrixModelTable, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelLamp1 = glm::mat4(1.0);
		matrixModelLamp1 = glm::translate(matrixModelLamp1, glm::vec3(0.0, -4.0, 5.0));
		//matrixModelBed = glm::rotate(matrixModelBed, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelLamp1 = glm::scale(matrixModelLamp1, glm::vec3(0.1, 0.1, 0.1));
		//matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		modelLamp1.render(matrixModelLamp1);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		//glActiveTexture(GL_TEXTURE0);/

		glm::mat4 matrixModelTV = glm::mat4(1.0);//9
		matrixModelTV = glm::translate(matrixModelTV, glm::vec3(14.0, -3.0, 6.0));//9
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTV = glm::scale(matrixModelTV, glm::vec3(0.01, 0.01, 0.01));
		modelTV.render(matrixModelTV);//9
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);*/

		glm::mat4 matrixModelTree = glm::mat4(1.0);
		matrixModelTree = glm::translate(matrixModelTree, glm::vec3(13.5, -5.0, 4.0));
		matrixModelTree = glm::rotate(matrixModelTree, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTree = glm::scale(matrixModelTree, glm::vec3(0.028, 0.028, 0.028));
		modelChristmasTree.render(matrixModelTree);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		/*glm::mat4 matrixModelLamborginhi = glm::mat4(1.0);
		matrixModelLamborginhi = glm::translate(matrixModelLamborginhi, glm::vec3(13.5, -5.0, 4.0));
		matrixModelLamborginhi = glm::rotate(matrixModelLamborginhi, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelLamborginhi = glm::scale(matrixModelLamborginhi, glm::vec3(0.028, 0.028, 0.028));
		modelLamborginhi.render(matrixModelLamborginhi);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);*/



		//Iniciamos a definir la casa----------------------------------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 1 - Pared derecha de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1ModelA = glm::mat4(1.0);
		pared1ModelA = glm::translate(pared1ModelA, glm::vec3(-15.0, 0.0, 0.0));
		pared1ModelA = glm::rotate(pared1ModelA, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1ModelA = glm::scale(pared1ModelA, glm::vec3(18.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1ModelA);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 2 - Pared izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model2A = glm::mat4(1.0);
		pared1Model2A = glm::translate(pared1Model2A, glm::vec3(-8.0, 0.0, 0.0));
		pared1Model2A = glm::rotate(pared1Model2A, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model2A = glm::scale(pared1Model2A, glm::vec3(18.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 3 - Parte trasera de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model3A = glm::mat4(1.0);
		pared1Model3A = glm::translate(pared1Model3A, glm::vec3(-11.5, 0.0, -9.0));
		pared1Model3A = glm::scale(pared1Model3A, glm::vec3(7.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model4A = glm::mat4(1.0);
		pared1Model4A = glm::translate(pared1Model4A, glm::vec3(-11.5, 5.0, 0.0));
		pared1Model4A = glm::rotate(pared1Model4A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model4A = glm::rotate(pared1Model4A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model4A = glm::scale(pared1Model4A, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Piso planta baja<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<h
		glm::mat4 pared1Model5A = glm::mat4(1.0);
		pared1Model5A = glm::translate(pared1Model5A, glm::vec3(-11.5, -5.0, 0.0));
		pared1Model5A = glm::rotate(pared1Model5A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model5A = glm::rotate(pared1Model5A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model5A = glm::scale(pared1Model5A, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model5A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo 1er Piso<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model6A = glm::mat4(1.0);
		pared1Model6A = glm::translate(pared1Model6A, glm::vec3(-11.5, 0.0, -5.4));
		pared1Model6A = glm::rotate(pared1Model6A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model6A = glm::rotate(pared1Model6A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model6A = glm::scale(pared1Model6A, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model6A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model7A = glm::mat4(1.0);
		pared1Model7A = glm::translate(pared1Model7A, glm::vec3(-11.5, 0.0, 5.4));
		pared1Model7A = glm::rotate(pared1Model7A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model7A = glm::rotate(pared1Model7A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model7A = glm::scale(pared1Model7A, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model7A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model8A = glm::mat4(1.0);
		pared1Model8A = glm::translate(pared1Model8A, glm::vec3(-9.5, 0.0, 0.0));
		pared1Model8A = glm::rotate(pared1Model8A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model8A = glm::rotate(pared1Model8A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model8A = glm::scale(pared1Model8A, glm::vec3(3.6, 3.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model8A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Paredes de las habitaciones<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model9A = glm::mat4(1.0);
		pared1Model9A = glm::translate(pared1Model9A, glm::vec3(-13.0, 0.0, -1.8));
		pared1Model9A = glm::scale(pared1Model9A, glm::vec3(4.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model9A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model10A = glm::mat4(1.0);
		pared1Model10A = glm::translate(pared1Model10A, glm::vec3(-13.0, 2.5, 1.8));
		pared1Model10A = glm::scale(pared1Model10A, glm::vec3(4.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model10A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model11A = glm::mat4(1.0);
		pared1Model11A = glm::translate(pared1Model11A, glm::vec3(-9.5, 4.5, 1.8));
		pared1Model11A = glm::scale(pared1Model11A, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model11A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model12A = glm::mat4(1.0);
		pared1Model12A = glm::translate(pared1Model12A, glm::vec3(-8.25, 2.0, 1.8));
		pared1Model12A = glm::scale(pared1Model12A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model12A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model13A = glm::mat4(1.0);
		pared1Model13A = glm::translate(pared1Model13A, glm::vec3(-10.75, 2.0, 1.8));
		pared1Model13A = glm::scale(pared1Model13A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model13A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta1A = glm::mat4(1.0);
		puerta1A = glm::translate(puerta1A, glm::vec3(-9.5, 2.0, 1.8));
		puerta1A = glm::scale(puerta1A, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model14A = glm::mat4(1.0);
		pared1Model14A = glm::translate(pared1Model14A, glm::vec3(-9.5, 4.5, -1.8));
		pared1Model14A = glm::scale(pared1Model14A, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model14A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model15A = glm::mat4(1.0);
		pared1Model15A = glm::translate(pared1Model15A, glm::vec3(-8.25, 2.0, -1.8));
		pared1Model15A = glm::scale(pared1Model15A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model15A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model16A = glm::mat4(1.0);
		pared1Model16A = glm::translate(pared1Model16A, glm::vec3(-10.75, 2.0, -1.8));
		pared1Model16A = glm::scale(pared1Model16A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model16A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta2A = glm::mat4(1.0);
		puerta2A = glm::translate(puerta2A, glm::vec3(-9.5, 2.0, -1.8));
		puerta2A = glm::scale(puerta2A, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 escaleraModel1A = glm::mat4(1.0);
		escaleraModel1A = glm::translate(escaleraModel1A, glm::vec3(-14.375, -3.75, 0.0));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel1A = glm::scale(escaleraModel1A, glm::vec3(1.25, 2.5, 3.6));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel2A = glm::mat4(1.0);
		escaleraModel2A = glm::translate(escaleraModel2A, glm::vec3(-13.4, -4.0, 0.9));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel2A = glm::scale(escaleraModel2A, glm::vec3(0.7, 2.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel3A = glm::mat4(1.0);
		escaleraModel3A = glm::translate(escaleraModel3A, glm::vec3(-12.7, -4.25, 0.9));
		escaleraModel3A = glm::scale(escaleraModel3A, glm::vec3(0.7, 1.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel4A = glm::mat4(1.0);
		escaleraModel4A = glm::translate(escaleraModel4A, glm::vec3(-12.0, -4.5, 0.9));
		escaleraModel4A = glm::scale(escaleraModel4A, glm::vec3(0.7, 1.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel5A = glm::mat4(1.0);
		escaleraModel5A = glm::translate(escaleraModel5A, glm::vec3(-11.35, -4.75, 0.9));
		escaleraModel5A = glm::scale(escaleraModel5A, glm::vec3(0.7, 0.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel5A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel6A = glm::mat4(1.0);
		escaleraModel6A = glm::translate(escaleraModel6A, glm::vec3(-13.4, -2.1875, -0.9));
		escaleraModel6A = glm::scale(escaleraModel6A, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel6A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel7A = glm::mat4(1.0);
		escaleraModel7A = glm::translate(escaleraModel7A, glm::vec3(-12.7, -1.5625, -0.9));
		escaleraModel7A = glm::scale(escaleraModel7A, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel7A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel8A = glm::mat4(1.0);
		escaleraModel8A = glm::translate(escaleraModel8A, glm::vec3(-12.0, -0.9375, -0.9));
		escaleraModel8A = glm::scale(escaleraModel8A, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel8A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel9A = glm::mat4(1.0);
		escaleraModel9A = glm::translate(escaleraModel9A, glm::vec3(-11.35, -0.3125, -0.9));
		escaleraModel9A = glm::scale(escaleraModel9A, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel9A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared de las escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredEscaleraModel1A = glm::mat4(1.0);
		paredEscaleraModel1A = glm::translate(paredEscaleraModel1A, glm::vec3(-11.35, -2.8125, 0.0));
		paredEscaleraModel1A = glm::scale(paredEscaleraModel1A, glm::vec3(0.7, 4.375, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel2A = glm::mat4(1.0);
		paredEscaleraModel2A = glm::translate(paredEscaleraModel2A, glm::vec3(-12.05, -3.125, 0.0));
		paredEscaleraModel2A = glm::scale(paredEscaleraModel2A, glm::vec3(0.7, 3.75, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel3A = glm::mat4(1.0);
		paredEscaleraModel3A = glm::translate(paredEscaleraModel3A, glm::vec3(-12.75, -3.4375, 0.0));
		paredEscaleraModel3A = glm::scale(paredEscaleraModel3A, glm::vec3(0.7, 3.125, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel4A = glm::mat4(1.0);
		paredEscaleraModel4A = glm::translate(paredEscaleraModel4A, glm::vec3(-13.45, -3.75, 0.0));
		paredEscaleraModel4A = glm::scale(paredEscaleraModel4A, glm::vec3(0.7, 2.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell15A = glm::mat4(1.0);
		paredEscaleraModell15A = glm::translate(paredEscaleraModell15A, glm::vec3(-11.0, -0.4375, -0.9));
		paredEscaleraModell15A = glm::scale(paredEscaleraModell15A, glm::vec3(0.01, 0.375, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModell15A);
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
		glm::mat4 paredFrontalModel1A = glm::mat4(1.0);
		paredFrontalModel1A = glm::translate(paredFrontalModel1A, glm::vec3(-11.5, 4.25, 9.0));
		paredFrontalModel1A = glm::scale(paredFrontalModel1A, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel2A = glm::mat4(1.0);
		paredFrontalModel2A = glm::translate(paredFrontalModel2A, glm::vec3(-11.5, 0.75, 9.0));
		paredFrontalModel2A = glm::scale(paredFrontalModel2A, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel3A = glm::mat4(1.0);
		paredFrontalModel3A = glm::translate(paredFrontalModel3A, glm::vec3(-14.25, 2.5, 9.0));
		paredFrontalModel3A = glm::scale(paredFrontalModel3A, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel4A = glm::mat4(1.0);
		paredFrontalModel4A = glm::translate(paredFrontalModel4A, glm::vec3(-9.5, 2.5, 9.0));
		paredFrontalModel4A = glm::scale(paredFrontalModel4A, glm::vec3(3.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredFrontalModel4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta y ventana principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model17A = glm::mat4(1.0);
		pared1Model17A = glm::translate(pared1Model17A, glm::vec3(-11.5, -0.5, 9.0));
		pared1Model17A = glm::scale(pared1Model17A, glm::vec3(7.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model17A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model18A = glm::mat4(1.0);
		pared1Model18A = glm::translate(pared1Model18A, glm::vec3(-8.25, -3.0, 9.0));
		pared1Model18A = glm::scale(pared1Model18A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model18A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model19A = glm::mat4(1.0);
		pared1Model19A = glm::translate(pared1Model19A, glm::vec3(-10.75, -3.0, 9.0));
		pared1Model19A = glm::scale(pared1Model19A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model19A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model110A = glm::mat4(1.0);
		pared1Model110A = glm::translate(pared1Model110A, glm::vec3(-14.75, -3.0, 9.0));
		pared1Model110A = glm::scale(pared1Model110A, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model110A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model111A = glm::mat4(1.0);
		pared1Model111A = glm::translate(pared1Model111A, glm::vec3(-12.75, -4.75, 9.0));
		pared1Model111A = glm::scale(pared1Model111A, glm::vec3(3.5, 0.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(pared1Model111A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta3A = glm::mat4(1.0);
		puerta3A = glm::translate(puerta3A, glm::vec3(-9.5, -3.0, 9.0));
		puerta3A = glm::scale(puerta3A, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta4A = glm::mat4(1.0);
		puerta4A = glm::translate(puerta4A, glm::vec3(-11.0, -3.0, -0.9));
		puerta4A = glm::scale(puerta4A, glm::vec3(0.01, 4.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(puerta4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ventanas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 window1A = glm::mat4(1.0);
		window1A = glm::translate(window1A, glm::vec3(-12.25, 2.5, 9.0));
		window1A = glm::scale(window1A, glm::vec3(2.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(window1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window2A = glm::mat4(1.0);
		window2A = glm::translate(window2A, glm::vec3(-12.75, -2.75, 9.0));
		window2A = glm::scale(window2A, glm::vec3(3.5, 3.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(window2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Lamparas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 modelBaseLamp1A = glm::mat4(1.0);
		modelBaseLamp1A = glm::translate(modelBaseLamp1A, glm::vec3(-11.5, -0.05, 4.0));
		modelBaseLamp1A = glm::scale(modelBaseLamp1A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp1A = glm::mat4(1.0);
		modelLamp1A = glm::translate(modelLamp1A, glm::vec3(-11.5, -0.1, 4.0));
		modelLamp1A = glm::scale(modelLamp1A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp2A = glm::mat4(1.0);
		modelBaseLamp2A = glm::translate(modelBaseLamp2A, glm::vec3(-11.5, -0.05, -4.0));
		modelBaseLamp2A = glm::scale(modelBaseLamp2A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp2A = glm::mat4(1.0);
		modelLamp2A = glm::translate(modelLamp2A, glm::vec3(-11.5, -0.1, -4.0));
		modelLamp2A = glm::scale(modelLamp2A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp3A = glm::mat4(1.0);
		modelBaseLamp3A = glm::translate(modelBaseLamp3A, glm::vec3(-11.5, 4.95, -5.4));
		modelBaseLamp3A = glm::scale(modelBaseLamp3A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp3A = glm::mat4(1.0);
		modelLamp3A = glm::translate(modelLamp3A, glm::vec3(-11.5, 4.9, -5.4));
		modelLamp3A = glm::scale(modelLamp3A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp4A = glm::mat4(1.0);
		modelBaseLamp4A = glm::translate(modelBaseLamp4A, glm::vec3(-11.5, 4.95, 5.4));
		modelBaseLamp4A = glm::scale(modelBaseLamp4A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		cylinderBaseLamp.render(modelBaseLamp4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp4A = glm::mat4(1.0);
		modelLamp4A = glm::translate(modelLamp4A, glm::vec3(-11.5, 4.9, 5.4));
		modelLamp4A = glm::scale(modelLamp4A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		sphereLamp2.render(modelLamp4A);
		glBindTexture(GL_TEXTURE_2D, 0);




		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(advanceDartBody, 0.0, 0.0));
		modelMatrixDart = glm::rotate(modelMatrixDart, rotDartBody, glm::vec3(0, 1, 0));
		modelDartLegoBody.render(modelMatrixDart);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDart);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDart);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDart);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-10.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDart);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDart);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		glActiveTexture(GL_TEXTURE0); // IMPORTANTE regresar a la textura 0
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		// Para salvar el frame
		if(record && modelSelected == 1){
			matrixJoints.push_back(rotDartHead);
			matrixJoints.push_back(rotDartLeftArm);
			matrixJoints.push_back(rotDartLeftHand);
			matrixJoints.push_back(rotDartRightArm);
			matrixJoints.push_back(rotDartRightHand);
			matrixJoints.push_back(rotDartLeftLeg);
			matrixJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				appendFrame(myfile, matrixJoints);
				saveFrame = false;
			}
		}
		else if(keyFramesJoints.size() > 0){
			// Para reproducir el frame
			interpolationJoints = numPasosJoints / (float) maxNumPasosJoints;
			numPasosJoints++;
			if (interpolationJoints > 1.0) {
				numPasosJoints = 0;
				interpolationJoints = 0;
				indexFrameJoints = indexFrameJointsNext;
				indexFrameJointsNext++;
			}
			if (indexFrameJointsNext > keyFramesJoints.size() - 1)
				indexFrameJointsNext = 0;

			rotDartHead = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 0, interpolationJoints);
			rotDartLeftArm = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 1, interpolationJoints);
			rotDartLeftHand = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 2, interpolationJoints);
			rotDartRightArm = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 3, interpolationJoints);
			rotDartRightHand = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 4, interpolationJoints);
			rotDartLeftLeg = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 5, interpolationJoints);
			rotDartRightLeg = interpolate(keyFramesJoints, indexFrameJoints, indexFrameJointsNext, 6, interpolationJoints);
		}

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		// Constantes de animaciones
		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.01;

		dz = 0;
		rot0 = 0;
		offX += 0.1;
		rotHelHelY += 0.5;

		advanceDartBody = 0.0;
		rotDartBody = 0.0;

		/*******************************************
		 * State machines
		 *******************************************/
		// State machine for eclipse car
		switch(state){
		case 0:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.1));
			advanceCount += 0.1;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.02;
			if(rotWheelsY < 0)
				rotWheelsY = 0;
			if(advanceCount > 10.0){
				advanceCount = 0;
				state = 1;
			}
			break;
		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.025));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(0.5f), glm::vec3(0, 1, 0));
			rotCount += 0.5f;
			rotWheelsX += 0.05;
			rotWheelsY += 0.02;
			if(rotWheelsY > 0.25)
				rotWheelsY = 0.25;
			if(rotCount >= 90.0){
				rotCount = 0;
				state = 0;
			}
			break;
		}

		// State machine for the lambo car
		switch(stateDoor){
		case 0:
			dorRotCount += 0.5;
			if(dorRotCount > 75)
				stateDoor = 1;
			break;
		case 1:
			dorRotCount -= 0.5;
			if(dorRotCount < 0){
				dorRotCount = 0.0;
				stateDoor = 0;
			}
			break;
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
