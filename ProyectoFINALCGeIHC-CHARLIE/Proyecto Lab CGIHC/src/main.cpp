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
Model modelPickup;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;
GLuint textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16;
GLuint textureID17, textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureID24;
GLuint textureID25, textureID26, textureID27, textureID28, textureID29, textureID30, textureID31, textureID32;
GLuint textureID33, textureID34, textureID35, textureID36, textureID37, textureID38, textureID39, textureID40;
GLuint textureID41, textureID42, textureID43, textureID44, textureID45, textureID46, textureID47, textureID48;
GLuint textureID49, textureID50, textureID51, textureID52, textureID53, textureID54, textureID55, textureID56;
GLuint textureID57, textureID58, textureID59, textureID60, textureID61, textureID62, textureID63, textureID64;
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
	sphereLamp.setShader(&shaderMulLighting);
	//Setter para poner el color de la geometria
	//sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

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
	modelAircraft.setShader(&shaderMulLighting);*/

	modelBed.loadModel("../models/bed/bed2.obj");
	modelBed.setShader(&shaderMulLighting);

	modelBed2.loadModel("../models/bed2/bed 2.obj");
	modelBed2.setShader(&shaderMulLighting);

	/*modelBed3.loadModel("../models/bed3/Obj/Bed 08.obj");
	modelBed3.setShader(&shaderMulLighting);*/

	modelTV.loadModel("../models/tv/OBJ/Samsung LED TV.obj");
	modelTV.setShader(&shaderMulLighting);

	modelBed4.loadModel("../models/toilet/toilet.obj");
	modelBed4.setShader(&shaderMulLighting);

	modelBed5.loadModel("../models/chair/chair strong_01.obj");
	modelBed5.setShader(&shaderMulLighting);

	modelBed6.loadModel("../models/furniture/Obj/Sofa collection.obj");
	modelBed6.setShader(&shaderMulLighting);

	modelBed7.loadModel("../models/table/tbl034.obj");
	modelBed7.setShader(&shaderMulLighting);

	/*modelLamp1.loadModel("../models/lamp/EFG8012A-ORB.obj");
	modelLamp1.setShader(&shaderMulLighting);*/

	modelChristmasTree.loadModel("../models/christmas tree/12150_Christmas_Tree_V2_L2.obj");
	modelChristmasTree.setShader(&shaderMulLighting);

	modelPickup.loadModel("../models/car/Pickup/L200-OBJ.obj");
	modelPickup.setShader(&shaderMulLighting);

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

	Texture texture2("../Textures/House/tierra.jpg");
	bitmap = texture2.loadImage();
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	texture2.freeImage(bitmap);

	Texture texture3("../Textures/House/paredLadrillos.jpg");
	bitmap = texture3.loadImage(true);
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	texture3.freeImage(bitmap);

	Texture texture4("../Textures/House/paredLadrillos2.jpg");
	bitmap = texture4.loadImage(true);
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
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

	Texture textureWallOrange("../Textures/House/pared6.jpg");
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

	Texture textureBuro("../Textures/House/buro1.jpg");
	bitmap = textureBuro.loadImage(true);
	data = textureBuro.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBuro.freeImage(bitmap);

	Texture textureBuro2("../Textures/House/buro2.jpg");
	bitmap = textureBuro2.loadImage(true);
	data = textureBuro2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBuro2.freeImage(bitmap);

	Texture textureRopero("../Textures/House/ropero.jpg");
	bitmap = textureRopero.loadImage(true);
	data = textureRopero.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureRopero.freeImage(bitmap);

	Texture textureCorbata("../Textures/luna.jpg");
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


	//--------------------------------------------------------------------------------------------------------------------
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TEXTURAS DE HALLOWEEN<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Texture textureParedPayaso("../Textures/Halloween/paredPayaso.png");
	bitmap = textureParedPayaso.loadImage(false);
	data = textureParedPayaso.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParedPayaso.freeImage(bitmap);

	Texture textureParedPayaso2("../Textures/Halloween/paredPayaso2.png");
	bitmap = textureParedPayaso2.loadImage(false);
	data = textureParedPayaso2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParedPayaso2.freeImage(bitmap);

	Texture textureParedSucia("../Textures/Halloween/paredSucia.jpg");
	bitmap = textureParedSucia.loadImage(false);
	data = textureParedSucia.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParedSucia.freeImage(bitmap);

	Texture textureparedHalloween("../Textures/Halloween/paredHalloween2.jpg");
	bitmap = textureparedHalloween.loadImage(false);
	data = textureparedHalloween.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID25);
	glBindTexture(GL_TEXTURE_2D, textureID25);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureparedHalloween.freeImage(bitmap);

	Texture textureParedSucia2("../Textures/Halloween/paredSucia2.jpg");
	bitmap = textureParedSucia2.loadImage(false);
	data = textureParedSucia2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID26);
	glBindTexture(GL_TEXTURE_2D, textureID26);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParedSucia2.freeImage(bitmap);

	Texture textureParedSucia3("../Textures/Halloween/paredSucia3.jpg");
	bitmap = textureParedSucia3.loadImage(false);
	data = textureParedSucia3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID27);
	glBindTexture(GL_TEXTURE_2D, textureID27);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParedSucia3.freeImage(bitmap);

	Texture texturePuertaH1("../Textures/Halloween/puerta1.jpg");
	bitmap = texturePuertaH1.loadImage(false);
	data = texturePuertaH1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID28);
	glBindTexture(GL_TEXTURE_2D, textureID28);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuertaH1.freeImage(bitmap);

	Texture texturePuertaH2("../Textures/Halloween/puerta2.jpg");
	bitmap = texturePuertaH2.loadImage(false);
	data = texturePuertaH2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID29);
	glBindTexture(GL_TEXTURE_2D, textureID29);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuertaH2.freeImage(bitmap);

	Texture texturePuertaH3("../Textures/Halloween/puerta3.png");
	bitmap = texturePuertaH3.loadImage(false);
	data = texturePuertaH3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID30);
	glBindTexture(GL_TEXTURE_2D, textureID30);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuertaH3.freeImage(bitmap);

	Texture textureCuadro1("../Textures/Halloween/cuadro3.jpg");
	bitmap = textureCuadro1.loadImage(false);
	data = textureCuadro1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID31);
	glBindTexture(GL_TEXTURE_2D, textureID31);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro1.freeImage(bitmap);

	Texture textureCuadro2("../Textures/Halloween/cuadro4.jpg");
	bitmap = textureCuadro2.loadImage(false);
	data = textureCuadro2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID32);
	glBindTexture(GL_TEXTURE_2D, textureID32);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro2.freeImage(bitmap);

	Texture textureCuadro3("../Textures/Halloween/cuadro5.jpg");
	bitmap = textureCuadro3.loadImage(false);
	data = textureCuadro3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID33);
	glBindTexture(GL_TEXTURE_2D, textureID33);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro3.freeImage(bitmap);

	Texture textureCuadro4("../Textures/Halloween/cuadro6.jpg");
	bitmap = textureCuadro4.loadImage(false);
	data = textureCuadro4.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID34);
	glBindTexture(GL_TEXTURE_2D, textureID34);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro4.freeImage(bitmap);

	Texture texturePapelpicado("../Textures/Halloween/papelpicado.png");
	bitmap = texturePapelpicado.loadImage(false);
	data = texturePapelpicado.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID35);
	glBindTexture(GL_TEXTURE_2D, textureID35);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePapelpicado.freeImage(bitmap);

	Texture texturePapelpicado2("../Textures/Halloween/papelpicado2.png");
	bitmap = texturePapelpicado2.loadImage(false);
	data = texturePapelpicado2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID36);
	glBindTexture(GL_TEXTURE_2D, textureID36);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePapelpicado2.freeImage(bitmap);

	Texture texturePapelpicado3("../Textures/Halloween/papelpicado3.png");
	bitmap = texturePapelpicado3.loadImage(false);
	data = texturePapelpicado3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID37);
	glBindTexture(GL_TEXTURE_2D, textureID37);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePapelpicado3.freeImage(bitmap);

	Texture texturePapelpicado4("../Textures/Halloween/papelpicado4.png");
	bitmap = texturePapelpicado4.loadImage(false);
	data = texturePapelpicado4.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID38);
	glBindTexture(GL_TEXTURE_2D, textureID38);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePapelpicado4.freeImage(bitmap);

	Texture texturePapelpicado5("../Textures/Halloween/papelpicado5.png");
	bitmap = texturePapelpicado5.loadImage(false);
	data = texturePapelpicado5.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID39);
	glBindTexture(GL_TEXTURE_2D, textureID39);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePapelpicado5.freeImage(bitmap);

	Texture texturePan("../Textures/Halloween/pan.jpg");
	bitmap = texturePan.loadImage(false);
	data = texturePan.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID40);
	glBindTexture(GL_TEXTURE_2D, textureID40);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePan.freeImage(bitmap);

	Texture textureCera("../Textures/Halloween/cera.jpg");
	bitmap = textureCera.loadImage(false);
	data = textureCera.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID41);
	glBindTexture(GL_TEXTURE_2D, textureID41);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCera.freeImage(bitmap);

	Texture textureIronMan("../Textures/Halloween/ironMan.jpg");
	bitmap = textureIronMan.loadImage(false);
	data = textureIronMan.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID42);
	glBindTexture(GL_TEXTURE_2D, textureID42);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureIronMan.freeImage(bitmap);
	
	Texture textureEspejo("../Textures/Halloween/fantasma.jpg");
	bitmap = textureEspejo.loadImage(false);
	data = textureEspejo.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID43);
	glBindTexture(GL_TEXTURE_2D, textureID43);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureEspejo.freeImage(bitmap);
	
	Texture textureMuerte("../Textures/Halloween/muerte.jpg");
	bitmap = textureMuerte.loadImage(false);
	data = textureMuerte.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID44);
	glBindTexture(GL_TEXTURE_2D, textureID44);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMuerte.freeImage(bitmap);

	Texture textureAlacena("../Textures/House/puertaAlacena.png");
	bitmap = textureAlacena.loadImage(false);
	data = textureAlacena.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID45);
	glBindTexture(GL_TEXTURE_2D, textureID45);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureAlacena.freeImage(bitmap);

	Texture textureMadera3("../Textures/House/madera3.jpg");
	bitmap = textureMadera3.loadImage(false);
	data = textureMadera3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID46);
	glBindTexture(GL_TEXTURE_2D, textureID46);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMadera3.freeImage(bitmap);

	Texture textureCancel("../Textures/House/cancel.png");
	bitmap = textureCancel.loadImage(false);
	data = textureCancel.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID47);
	glBindTexture(GL_TEXTURE_2D, textureID47);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCancel.freeImage(bitmap);
	
		//--------------------------------------------------------------------------------------------------------------------
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TEXTURAS DE NAVIDAD<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Texture textureCuadro1N("../Textures/Navidad/cuadro1n.jpg");
	bitmap = textureCuadro1N.loadImage(false);
	data = textureCuadro1N.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID48);
	glBindTexture(GL_TEXTURE_2D, textureID48);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro1N.freeImage(bitmap);
	
	Texture textureCuadro2N("../Textures/Navidad/cuadro2n.jpg");
	bitmap = textureCuadro2N.loadImage(false);
	data = textureCuadro2N.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID49);
	glBindTexture(GL_TEXTURE_2D, textureID49);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro2N.freeImage(bitmap);
	
	Texture textureCuadro3N("../Textures/Navidad/cuadro3n.jpg");
	bitmap = textureCuadro3N.loadImage(false);
	data = textureCuadro3N.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID50);
	glBindTexture(GL_TEXTURE_2D, textureID50);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro3N.freeImage(bitmap);
	
	Texture textureCuadro4N("../Textures/Navidad/cuadro4n.jpg");
	bitmap = textureCuadro4N.loadImage(false);
	data = textureCuadro4N.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID51);
	glBindTexture(GL_TEXTURE_2D, textureID51);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro4N.freeImage(bitmap);
	
	Texture textureCuadro5N("../Textures/Navidad/cuadro5n.jpg");
	bitmap = textureCuadro5N.loadImage(false);
	data = textureCuadro5N.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID52);
	glBindTexture(GL_TEXTURE_2D, textureID52);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCuadro5N.freeImage(bitmap);
	
	Texture textureMelchor("../Textures/Navidad/melchor.jpg");
	bitmap = textureMelchor.loadImage(false);
	data = textureMelchor.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID53);
	glBindTexture(GL_TEXTURE_2D, textureID53);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMelchor.freeImage(bitmap);
	
	Texture textureGaspar("../Textures/Navidad/gaspar.jpg");
	bitmap = textureGaspar.loadImage(false);
	data = textureGaspar.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID54);
	glBindTexture(GL_TEXTURE_2D, textureID54);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureGaspar.freeImage(bitmap);
	
	Texture textureBaltazar("../Textures/Navidad/baltazar.jpg");
	bitmap = textureBaltazar.loadImage(false);
	data = textureBaltazar.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID55);
	glBindTexture(GL_TEXTURE_2D, textureID55);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBaltazar.freeImage(bitmap);
	
	Texture textureRegalo1("../Textures/Navidad/regalo1.jpg");
	bitmap = textureRegalo1.loadImage(false);
	data = textureRegalo1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID56);
	glBindTexture(GL_TEXTURE_2D, textureID56);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureRegalo1.freeImage(bitmap);
	
	Texture textureRegalo2("../Textures/Navidad/regalo4.jpg");
	bitmap = textureRegalo2.loadImage(false);
	data = textureRegalo2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID57);
	glBindTexture(GL_TEXTURE_2D, textureID57);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureRegalo2.freeImage(bitmap);
	
	Texture textureRegalo3("../Textures/Navidad/regalo5.jpg");
	bitmap = textureRegalo3.loadImage(false);
	data = textureRegalo3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID58);
	glBindTexture(GL_TEXTURE_2D, textureID58);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureRegalo3.freeImage(bitmap);
	
	

//--------------------------------------------------------------------------------------------------------------------
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

	glm::mat4 matrixModelPickup = glm::mat4(1.0);
	matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(2.0, -5.0, -9.0));
	matrixModelPickup = glm::scale(matrixModelPickup, glm::vec3(0.1, 0.1, 0.1));


	int statePickup = 0;
	float rotPickup = 0.0;
	int onoff = 0;

	float offsetPickupAdvance = 0.0;
	float offsetPickupRot = 0.0;
	float lightSpecular = 0.0;
	float color = 1.0;
	float contador = 0.0;


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
		shaderMulLighting.setInt("spotLightCount", 5);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(glm::vec3(-11.5, -0.05, 4.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[1].position", glm::value_ptr(glm::vec3(-11.5, -0.05, -4.0)));
		shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[1].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[1].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[1].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[1].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[1].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[2].position", glm::value_ptr(glm::vec3(-11.5, 4.95, -5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[2].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[2].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[2].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[2].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[2].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[2].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[3].position", glm::value_ptr(glm::vec3(-11.5, 4.9, -5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[3].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[3].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[3].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[3].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[3].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[3].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[3].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[3].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[3].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[4].position", glm::value_ptr(glm::vec3(-11.5, 4.95, 5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[4].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[4].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[4].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[4].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[4].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[4].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[4].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[4].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[4].quadratic", 0.009);

		/*shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(glm::vec3(-18.5, 4.95, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[6].position", glm::value_ptr(glm::vec3(-17.0, 4.95, 5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[6].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[6].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[6].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[6].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[6].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[6].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[6].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[6].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[6].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[7].position", glm::value_ptr(glm::vec3(-17.0, 4.95, -5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[7].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[7].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[7].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[7].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[7].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[7].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[7].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[7].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[7].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[8].position", glm::value_ptr(glm::vec3(-26.5, 4.95, -5.0)));
		shaderMulLighting.setVectorFloat3("spotLights[8].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[8].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[8].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[8].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[8].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[8].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[8].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[8].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[8].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[9].position", glm::value_ptr(glm::vec3(-29.0, 4.95, 4.5)));
		shaderMulLighting.setVectorFloat3("spotLights[9].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[9].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[9].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[9].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[9].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[9].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[9].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[9].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[9].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[10].position", glm::value_ptr(glm::vec3(-11.0, 4.95, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[10].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[10].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[10].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[10].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[10].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[10].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[10].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[10].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[10].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[11].position", glm::value_ptr(glm::vec3(-20.0, -0.1, 5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[11].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[11].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[11].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[11].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[11].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[11].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[11].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[11].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[11].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[12].position", glm::value_ptr(glm::vec3(-20.0, -0.1, -5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[12].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[12].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[12].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[12].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[12].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[12].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[12].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[12].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[12].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[13].position", glm::value_ptr(glm::vec3(-28.0, -0.1, 5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[13].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[13].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[13].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[13].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[13].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[13].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[13].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[13].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[13].quadratic", 0.009);

		shaderMulLighting.setVectorFloat3("spotLights[14].position", glm::value_ptr(glm::vec3(-28.0, -0.1, -5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[14].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[14].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("spotLights[14].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[14].light.specular", glm::value_ptr(glm::vec3(0.8, 0.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[14].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[14].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[14].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[14].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[14].quadratic", 0.009);*/

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 3);
		shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-19.5, -2.9, 1.5))));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[1].position", glm::value_ptr((glm::vec3(-19.0, -2.9, 1.5))));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[1].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[1].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[1].quadratic", 0.004);

		shaderMulLighting.setVectorFloat3("pointLights[2].position", glm::value_ptr((glm::vec3(-18.0, -2.9, 1.5))));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[2].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[2].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[2].quadratic", 0.004);

		/*******************************************
		 * Modelo de Luces dentro de la casa
		 *******************************************/
		sphere1.setScale(glm::vec3(0.1, 0.2, 0.1));
		sphere1.setPosition(glm::vec3(-19.5, -2.9, 1.5));
		sphere1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphere1.render();

		sphere1.setScale(glm::vec3(0.1, 0.2, 0.1));
		sphere1.setPosition(glm::vec3(-19.0, -2.9, 1.5));
		sphere1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphere1.render();

		sphere1.setScale(glm::vec3(0.1, 0.2, 0.1));
		sphere1.setPosition(glm::vec3(-18.0, -2.9, 1.5));
		sphere1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphere1.render();

		sphere1.setScale(glm::vec3(0.1, 0.2, 0.1));
		sphere1.setPosition(glm::vec3(-17.5, -2.9, 1.5));
		sphere1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphere1.render();



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
		glBindTexture(GL_TEXTURE_2D, textureID21);
		sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereLamp.render(lightModelmatrix);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Esto es para colocar las esferas de las luces
		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -1.0, 4.05));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -1.05, 3.9));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.2, -1.1, 3.85));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.1, -1.15, 3.9));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -1.2, 4.05));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.1, -1.25, 4.2));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -1.3, 4.2));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.5, -1.35, 4.0));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.5, -1.4, 3.8));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.4, -1.45, 3.6));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.2, -1.5, 3.6));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -1.55, 3.8));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -1.6, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.7, -1.65, 4.4));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -1.7, 4.6));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.1, -1.75, 4.6));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -1.8, 4.4));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.5, -1.85, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.5, -1.9, 3.8));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -1.95, 3.5));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.1, -2.0, 3.5));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -2.05, 3.8));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.7, -2.1, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -2.15, 4.4));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -2.2, 4.7));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.2, -2.25, 4.7));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.4, -2.3, 4.4));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.6, -2.35, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -2.4, 3.8));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -2.45, 3.5));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -2.5, 3.7));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.6, -2.55, 3.9));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.8, -2.6, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -2.65, 4.3));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -2.7, 4.5));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.6, -2.75, 4.5));
		sphereLamp.setColor(glm::vec4(0.0, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.9, -2.8, 4.2));
		sphereLamp.setColor(glm::vec4(color, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.9, -2.85, 3.9));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.6, -2.9, 3.6));
		sphereLamp.setColor(glm::vec4(color, color, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.3, -2.95, 3.3));
		sphereLamp.setColor(glm::vec4(color, 0.0, color, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(13.0, -3.0, 3.3));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.7, -3.05, 3.7));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.4, -3.1, 4.1));
		sphereLamp.setColor(glm::vec4(0.0, 0.0, 1.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.4, -3.15, 4.5));
		sphereLamp.setColor(glm::vec4(0.8, 0.8, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.05, 0.05, 0.05));
		sphereLamp.setPosition(glm::vec3(12.7, -3.2, 4.9));
		sphereLamp.setColor(glm::vec4(0.9, 0.0, 0.9, 1.0));
		sphereLamp.render();


/*------------------------------------------------------------------------------------------------------------------------
											            CASA NAVIDAD
--------------------------------------------------------------------------------------------------------------------------*/
       //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 1 - Pared derecha de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model = glm::mat4(1.0);
		pared1Model = glm::translate(pared1Model, glm::vec3(15.0, -2.5, -5.4));
		pared1Model = glm::rotate(pared1Model, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model = glm::scale(pared1Model, glm::vec3(7.2, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(pared1Model);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 2 - Pared izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model2 = glm::mat4(1.0);
		pared1Model2 = glm::translate(pared1Model2, glm::vec3(8.0, 0.0, 0.0));
		pared1Model2 = glm::rotate(pared1Model2, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model2 = glm::scale(pared1Model2, glm::vec3(18.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model2);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 3 - Parte trasera de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model3 = glm::mat4(1.0);
		pared1Model3 = glm::translate(pared1Model3, glm::vec3(20.5, 0.0, -9.0));
		pared1Model3 = glm::scale(pared1Model3, glm::vec3(25.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model3);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 4 - Parte izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model45 = glm::mat4(1.0);
		pared1Model45 = glm::translate(pared1Model45, glm::vec3(33.0, 0.0, 0.0));
		pared1Model45 = glm::rotate(pared1Model45, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model45 = glm::scale(pared1Model45, glm::vec3(18.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model45);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 5 - Parte frontal de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model44 = glm::mat4(1.0);
		pared1Model44 = glm::translate(pared1Model44, glm::vec3(24.0, -2.5, 9.0));
		pared1Model44 = glm::scale(pared1Model44, glm::vec3(18.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,pared1Model44);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,pared1Model44);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model4 = glm::mat4(1.0);
		pared1Model4 = glm::translate(pared1Model4, glm::vec3(20.5, 5.0, 0.0));
		pared1Model4 = glm::rotate(pared1Model4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model4 = glm::rotate(pared1Model4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model4 = glm::scale(pared1Model4, glm::vec3(18.0, 25.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		techo.render(pared1Model4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Piso planta baja<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 piso1B = glm::mat4(1.0);
		piso1B = glm::translate(piso1B, glm::vec3(20.5, -5.0, 0.0));
		piso1B = glm::rotate(piso1B, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		piso1B = glm::rotate(piso1B, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		piso1B = glm::scale(piso1B, glm::vec3(18.0, 25.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		paredGoldenrod.render(piso1B);
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
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model7 = glm::mat4(1.0);
		pared1Model7 = glm::translate(pared1Model7, glm::vec3(11.5, 0.0, 5.4));
		pared1Model7 = glm::rotate(pared1Model7, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model7 = glm::rotate(pared1Model7, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model7 = glm::scale(pared1Model7, glm::vec3(7.2, 7.0, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("saleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model7);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model8 = glm::mat4(1.0);
		pared1Model8 = glm::translate(pared1Model8, glm::vec3(9.5, 0.0, 0.0));
		pared1Model8 = glm::rotate(pared1Model8, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model8 = glm::rotate(pared1Model8, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model8 = glm::scale(pared1Model8, glm::vec3(3.6, 3.0, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model8);

		glm::mat4 techo2B = glm::mat4(1.0);
		techo2B = glm::translate(techo2B, glm::vec3(24.0, 0.0, 0.0));
		techo2B = glm::rotate(techo2B, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		techo2B = glm::rotate(techo2B, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		techo2B = glm::scale(techo2B, glm::vec3(18.0, 18.0, 0.01));
		paredGoldenrod.render(techo2B);

		glm::mat4 techo3B = glm::mat4(1.0);
		techo3B = glm::translate(techo3B, glm::vec3(13.0, 0.0, 0.9));
		techo3B = glm::rotate(techo3B, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		techo3B = glm::rotate(techo3B, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		techo3B = glm::scale(techo3B, glm::vec3(1.8, 4.0, 0.01));
		paredGoldenrod.render(techo3B);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Paredes de las habitaciones<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredH = glm::mat4(1.0);
		paredH = glm::translate(paredH, glm::vec3(13.0, 0.0, -1.8));
		paredH = glm::scale(paredH, glm::vec3(4.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(paredH);

		glm::mat4 pared1H = glm::mat4(1.0);
		pared1H = glm::translate(paredH, glm::vec3(13.0, 2.5, 1.8));
		pared1H = glm::scale(pared1H, glm::vec3(4.0, 5.0, 0.01));
		paredGoldenrod.render(pared1H);

		glm::mat4 paredH2 = glm::mat4(1.0);
		paredH2 = glm::translate(paredH2, glm::vec3(22.0, 2.5, -5.4));
		paredH2 = glm::rotate(paredH2, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		paredH2 = glm::scale(paredH2, glm::vec3(7.5, 5.0, 0.01));
		paredGoldenrod.render(paredH2);

		glm::mat4 paredH3 = glm::mat4(1.0);
		paredH3 = glm::translate(paredH3, glm::vec3(22.0, 2.5, 5.4));
		paredH3 = glm::rotate(paredH3, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredH3 = glm::scale(paredH3, glm::vec3(7.2, 5.0, 0.01));
		paredGoldenrod.render(paredH3);

		glm::mat4 paredH4 = glm::mat4(1.0);
		paredH4 = glm::translate(paredH4, glm::vec3(26.0, 2.5, 4.5));
		paredH4 = glm::rotate(paredH4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredH4 = glm::scale(paredH4, glm::vec3(9.0, 5.0, 0.01));
		techo.render(paredH4);

		glm::mat4 paredH5 = glm::mat4(1.0);
		paredH5 = glm::translate(paredH5, glm::vec3(18.5, 2.5, -1.8));
		paredH5 = glm::scale(paredH5, glm::vec3(7.0, 5.0, 0.01));
		paredGoldenrod.render(paredH5);

		glm::mat4 paredH6 = glm::mat4(1.0);
		paredH6 = glm::translate(paredH6, glm::vec3(16.5, 2.5, 1.8));
		paredH6 = glm::scale(paredH6, glm::vec3(11.0, 5.0, 0.01));
		paredGoldenrod.render(paredH6);

		glm::mat4 paredH7 = glm::mat4(1.0);
		paredH7 = glm::translate(paredH7, glm::vec3(30.5, 2.5, 0.0));
		paredH7 = glm::scale(paredH7, glm::vec3(5.0, 5.0, 0.01));
		paredGoldenrod.render(paredH7);

		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredH8 = glm::mat4(1.0);
		paredH8 = glm::translate(paredH8, glm::vec3(29.5, 2.5, 9.0));
		paredH8 = glm::scale(paredH8, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredH8);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredH8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredH9 = glm::mat4(1.0);
		paredH9 = glm::translate(paredH9, glm::vec3(18.5, 2.5, 9.0));
		paredH9 = glm::scale(paredH9, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredH9);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredH9);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model11 = glm::mat4(1.0);
		pared1Model11 = glm::translate(pared1Model11, glm::vec3(9.5, 4.5, 1.8));
		pared1Model11 = glm::scale(pared1Model11, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		techo.render(pared1Model11);

		glm::mat4 pared1Model12 = glm::mat4(1.0);
		pared1Model12 = glm::translate(pared1Model12, glm::vec3(8.25, 2.0, 1.8));
		pared1Model12 = glm::scale(pared1Model12, glm::vec3(0.5, 4.0, 0.01));
		paredGoldenrod.render(pared1Model12);

		glm::mat4 pared1Model13 = glm::mat4(1.0);
		pared1Model13 = glm::translate(pared1Model13, glm::vec3(10.75, 2.0, 1.8));
		pared1Model13 = glm::scale(pared1Model13, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model13);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta1 = glm::mat4(1.0);
		puerta1 = glm::translate(puerta1, glm::vec3(9.5, 2.0, 1.8));
		puerta1 = glm::scale(puerta1, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		techo.render(puerta1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model14 = glm::mat4(1.0);
		pared1Model14 = glm::translate(pared1Model14, glm::vec3(9.5, 4.5, -1.8));
		pared1Model14 = glm::scale(pared1Model14, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(pared1Model14);

		glm::mat4 pared1Model15 = glm::mat4(1.0);
		pared1Model15 = glm::translate(pared1Model15, glm::vec3(8.25, 2.0, -1.8));
		pared1Model15 = glm::scale(pared1Model15, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model15);

		glm::mat4 pared1Model16 = glm::mat4(1.0);
		pared1Model16 = glm::translate(pared1Model16, glm::vec3(10.75, 2.0, -1.8));
		pared1Model16 = glm::scale(pared1Model16, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model16);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta2 = glm::mat4(1.0);
		puerta2 = glm::translate(puerta2, glm::vec3(9.5, 2.0, -1.8));
		puerta2 = glm::scale(puerta2, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		techo.render(puerta2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta3<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 puerta3B = glm::mat4(1.0);
		puerta3B = glm::translate(puerta3B, glm::vec3(11.0, -3.0, -0.9));
		puerta3B = glm::scale(puerta3B, glm::vec3(0.01, 4.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		techo.render(puerta3B);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta4<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 marco4B = glm::mat4(1.0);
		marco4B = glm::translate(marco4B, glm::vec3(27.0, 4.5, 0.0));
		marco4B = glm::scale(marco4B, glm::vec3(2.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		techo.render(marco4B);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 puerta4B = glm::mat4(1.0);
		puerta4B = glm::translate(puerta4B, glm::vec3(27.0, 2.0, 0.0));
		puerta4B = glm::scale(puerta4B, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		techo.render(puerta4B);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 escaleraModel1 = glm::mat4(1.0);
		escaleraModel1 = glm::translate(escaleraModel1, glm::vec3(14.375, -3.75, 0.0));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel1 = glm::scale(escaleraModel1, glm::vec3(1.25, 2.5, 3.6));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel1);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel2 = glm::mat4(1.0);
		escaleraModel2 = glm::translate(escaleraModel2, glm::vec3(13.4, -4.0, 0.9));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel2 = glm::scale(escaleraModel2, glm::vec3(0.7, 2.0, 1.8));
		//glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel2);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel3 = glm::mat4(1.0);
		escaleraModel3 = glm::translate(escaleraModel3, glm::vec3(12.7, -4.25, 0.9));
		escaleraModel3 = glm::scale(escaleraModel3, glm::vec3(0.7, 1.5, 1.8));
		techo.render(escaleraModel3);

		glm::mat4 escaleraModel4 = glm::mat4(1.0);
		escaleraModel4 = glm::translate(escaleraModel4, glm::vec3(12.0, -4.5, 0.9));
		escaleraModel4 = glm::scale(escaleraModel4, glm::vec3(0.7, 1.0, 1.8));
		techo.render(escaleraModel4);

		glm::mat4 escaleraModel5 = glm::mat4(1.0);
		escaleraModel5 = glm::translate(escaleraModel5, glm::vec3(11.35, -4.75, 0.9));
		escaleraModel5 = glm::scale(escaleraModel5, glm::vec3(0.7, 0.5, 1.8));
		techo.render(escaleraModel5);

		glm::mat4 escaleraModel6 = glm::mat4(1.0);
		escaleraModel6 = glm::translate(escaleraModel6, glm::vec3(13.4, -2.1875, -0.9));
		escaleraModel6 = glm::scale(escaleraModel6, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel6);

		glm::mat4 escaleraModel7 = glm::mat4(1.0);
		escaleraModel7 = glm::translate(escaleraModel7, glm::vec3(12.7, -1.5625, -0.9));
		escaleraModel7 = glm::scale(escaleraModel7, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel7);

		glm::mat4 escaleraModel8 = glm::mat4(1.0);
		escaleraModel8 = glm::translate(escaleraModel8, glm::vec3(12.0, -0.9375, -0.9));
		escaleraModel8 = glm::scale(escaleraModel8, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel8);
		
		glm::mat4 escaleraModel9 = glm::mat4(1.0);
		escaleraModel9 = glm::translate(escaleraModel9, glm::vec3(11.35, -0.3125, -0.9));
		escaleraModel9 = glm::scale(escaleraModel9, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel9);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared de las escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredEscaleraModel1 = glm::mat4(1.0);
		paredEscaleraModel1 = glm::translate(paredEscaleraModel1, glm::vec3(11.35, -2.8125, 0.0));
		paredEscaleraModel1 = glm::scale(paredEscaleraModel1, glm::vec3(0.7, 4.375, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel1);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel2 = glm::mat4(1.0);
		paredEscaleraModel2 = glm::translate(paredEscaleraModel2, glm::vec3(12.05, -3.125, 0.0));
		paredEscaleraModel2 = glm::scale(paredEscaleraModel2, glm::vec3(0.7, 3.75, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel2);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel3 = glm::mat4(1.0);
		paredEscaleraModel3 = glm::translate(paredEscaleraModel3, glm::vec3(12.75, -3.4375, 0.0));
		paredEscaleraModel3 = glm::scale(paredEscaleraModel3, glm::vec3(0.7, 3.125, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel3);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel4 = glm::mat4(1.0);
		paredEscaleraModel4 = glm::translate(paredEscaleraModel4, glm::vec3(13.45, -3.75, 0.0));
		paredEscaleraModel4 = glm::scale(paredEscaleraModel4, glm::vec3(0.7, 2.5, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel4);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell15 = glm::mat4(1.0);
		paredEscaleraModell15 = glm::translate(paredEscaleraModell15, glm::vec3(11.0, -0.5, -0.9));
		paredEscaleraModell15 = glm::scale(paredEscaleraModell15, glm::vec3(0.01, 1.0, 1.8));
		paredGoldenrod.render(paredEscaleraModell15);

		glm::mat4 paredEscaleraModell16 = glm::mat4(1.0);
		paredEscaleraModell16 = glm::translate(paredEscaleraModell16, glm::vec3(15.0, -2.5, 0.0));
		paredEscaleraModell16 = glm::rotate(paredEscaleraModell16, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		paredEscaleraModell16 = glm::scale(paredEscaleraModell16, glm::vec3(3.6, 5.0, 0.01));
		techo.render(paredEscaleraModell16);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared frontal superior<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredFrontalModel1 = glm::mat4(1.0);
		paredFrontalModel1 = glm::translate(paredFrontalModel1, glm::vec3(11.5, 4.25, 9.0));
		paredFrontalModel1 = glm::scale(paredFrontalModel1, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredFrontalModel1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredFrontalModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel2 = glm::mat4(1.0);
		paredFrontalModel2 = glm::translate(paredFrontalModel2, glm::vec3(11.5, 0.75, 9.0));
		paredFrontalModel2 = glm::scale(paredFrontalModel2, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredFrontalModel2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredFrontalModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel3 = glm::mat4(1.0);
		paredFrontalModel3 = glm::translate(paredFrontalModel3, glm::vec3(14.25, 2.5, 9.0));
		paredFrontalModel3 = glm::scale(paredFrontalModel3, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredFrontalModel3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredFrontalModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel4 = glm::mat4(1.0);
		paredFrontalModel4 = glm::translate(paredFrontalModel4, glm::vec3(9.5, 2.5, 9.0));
		paredFrontalModel4 = glm::scale(paredFrontalModel4, glm::vec3(3.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,paredFrontalModel4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,paredFrontalModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model17 = glm::mat4(1.0);
		pared1Model17 = glm::translate(pared1Model17, glm::vec3(11.5, -2.5, 9.0));
		pared1Model17 = glm::scale(pared1Model17, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		paredGoldenrod.render(0,8,pared1Model17);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		paredGoldenrod.render(9,20,pared1Model17);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 puertaPrincipal = glm::mat4(1.0);
		puertaPrincipal = glm::translate(puertaPrincipal, glm::vec3(11.5, -3.25, 9.0));
		puertaPrincipal = glm::scale(puertaPrincipal, glm::vec3(2.0, 3.5, 0.1));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		paredGoldenrod.render(puertaPrincipal);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ventanas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 window1 = glm::mat4(1.0);
		window1 = glm::translate(window1, glm::vec3(12.25, 2.5, 9.0));
		window1 = glm::scale(window1, glm::vec3(2.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window2 = glm::mat4(1.0);
		window2 = glm::translate(window2, glm::vec3(19.5, -2.5, 0.0));
		window2 = glm::scale(window2, glm::vec3(9.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window3 = glm::mat4(1.0);
		window3 = glm::translate(window3, glm::vec3(24.0, 2.5, 9.0));
		window3 = glm::scale(window3, glm::vec3(4.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window3);
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

		glm::mat4 modelBaseLamp5 = glm::mat4(1.0);
		modelBaseLamp5 = glm::translate(modelBaseLamp5, glm::vec3(18.5, 4.95, 0.0));
		modelBaseLamp5 = glm::scale(modelBaseLamp5, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp5);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp5 = glm::mat4(1.0);
		modelLamp5 = glm::translate(modelLamp5, glm::vec3(18.5, 4.9, 0.0));
		modelLamp5 = glm::scale(modelLamp5, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp5);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp6 = glm::mat4(1.0);
		modelBaseLamp6 = glm::translate(modelBaseLamp6, glm::vec3(17.0, 4.95, 5.4));
		modelBaseLamp6 = glm::scale(modelBaseLamp6, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp6 = glm::mat4(1.0);
		modelLamp6 = glm::translate(modelLamp6, glm::vec3(17.0, 4.9, 5.4));
		modelLamp6 = glm::scale(modelLamp6, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp7 = glm::mat4(1.0);
		modelBaseLamp7 = glm::translate(modelBaseLamp7, glm::vec3(17.0, 4.95, -5.4));
		modelBaseLamp7 = glm::scale(modelBaseLamp7, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp7);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp7 = glm::mat4(1.0);
		modelLamp7 = glm::translate(modelLamp7, glm::vec3(17.0, 4.9, -5.4));
		modelLamp7 = glm::scale(modelLamp7, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp7);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp8 = glm::mat4(1.0);
		modelBaseLamp8 = glm::translate(modelBaseLamp8, glm::vec3(26.5, 4.95, -5.0));
		modelBaseLamp8 = glm::scale(modelBaseLamp8, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp8 = glm::mat4(1.0);
		modelLamp8 = glm::translate(modelLamp8, glm::vec3(26.5, 4.9, -5.0));
		modelLamp8 = glm::scale(modelLamp8, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp9 = glm::mat4(1.0);
		modelBaseLamp9 = glm::translate(modelBaseLamp9, glm::vec3(29.0, 4.95, 4.5));
		modelBaseLamp9 = glm::scale(modelBaseLamp9, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp9);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp9 = glm::mat4(1.0);
		modelLamp9 = glm::translate(modelLamp9, glm::vec3(29.0, 4.9, 4.5));
		modelLamp9 = glm::scale(modelLamp9, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp9);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp10 = glm::mat4(1.0);
		modelBaseLamp10 = glm::translate(modelBaseLamp10, glm::vec3(-1.0, 4.95, 0.0));
		modelBaseLamp10 = glm::scale(modelBaseLamp10, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp10);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp10 = glm::mat4(1.0);
		modelLamp10 = glm::translate(modelLamp10, glm::vec3(11.0, 4.9, 0.0));
		modelLamp10 = glm::scale(modelLamp10, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp10);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp11 = glm::mat4(1.0);
		modelBaseLamp11 = glm::translate(modelBaseLamp11, glm::vec3(20.0, -0.1, 5.4));
		modelBaseLamp11 = glm::scale(modelBaseLamp11, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp11);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp11 = glm::mat4(1.0);
		modelLamp11 = glm::translate(modelLamp11, glm::vec3(20.0, -0.15, 5.4));
		modelLamp11 = glm::scale(modelLamp11, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp11);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp12 = glm::mat4(1.0);
		modelBaseLamp12 = glm::translate(modelBaseLamp12, glm::vec3(20.0, -0.1, -5.4));
		modelBaseLamp12 = glm::scale(modelBaseLamp12, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp12);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp12 = glm::mat4(1.0);
		modelLamp12 = glm::translate(modelLamp12, glm::vec3(20.0, -0.15, -5.4));
		modelLamp12 = glm::scale(modelLamp12, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp12);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp13 = glm::mat4(1.0);
		modelBaseLamp13 = glm::translate(modelBaseLamp13, glm::vec3(28.0, -0.1, 5.4));
		modelBaseLamp13 = glm::scale(modelBaseLamp13, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp13);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp13 = glm::mat4(1.0);
		modelLamp13 = glm::translate(modelLamp13, glm::vec3(28.0, -0.15, 5.4));
		modelLamp13 = glm::scale(modelLamp13, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp13);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp14 = glm::mat4(1.0);
		modelBaseLamp14 = glm::translate(modelBaseLamp14, glm::vec3(28.0, -0.1, -5.4));
		modelBaseLamp14 = glm::scale(modelBaseLamp14, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp14);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp14 = glm::mat4(1.0);
		modelLamp14 = glm::translate(modelLamp14, glm::vec3(28.0, -0.15, -5.4));
		modelLamp14 = glm::scale(modelLamp14, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp14);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Muebles<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 buro1B = glm::mat4(1.0);
		buro1B = glm::translate(buro1B, glm::vec3(21.0, 1.0, 2.8));
		buro1B = glm::scale(buro1B, glm::vec3(2.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		techo.render(buro1B);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 ropero1B = glm::mat4(1.0);
		ropero1B = glm::translate(ropero1B, glm::vec3(21.0, 2.0, 6.3));
		ropero1B = glm::scale(ropero1B, glm::vec3(2.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		techo.render(ropero1B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 buro2B = glm::mat4(1.0);
		buro2B = glm::translate(buro2B, glm::vec3(21.0, 1.0, -2.8));
		buro2B = glm::scale(buro2B, glm::vec3(2.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		techo.render(buro2B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 ropero2B = glm::mat4(1.0);
		ropero2B = glm::translate(ropero2B, glm::vec3(21.0, 2.0, -6.3));
		ropero2B = glm::scale(ropero2B, glm::vec3(2.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		techo.render(ropero2B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cancel1B = glm::mat4(1.0);
		cancel1B = glm::translate(cancel1B, glm::vec3(30.0, 2.5, 1.5));
		cancel1B = glm::rotate(cancel1B, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		cancel1B = glm::scale(cancel1B, glm::vec3(3.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID47);
		techo.render(cancel1B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cancel2B = glm::mat4(1.0);
		cancel2B = glm::translate(cancel2B, glm::vec3(31.5, 2.5, 3.0));
		cancel2B = glm::scale(cancel2B, glm::vec3(3.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID47);
		techo.render(cancel2B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 maderaAlacenaB = glm::mat4(1.0);
		maderaAlacenaB = glm::translate(maderaAlacenaB, glm::vec3(32.0, -3.0, -4.0));
		maderaAlacenaB = glm::scale(maderaAlacenaB, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID46);
		techo.render(maderaAlacenaB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 maderaAlacena2B = glm::mat4(1.0);
		maderaAlacena2B = glm::translate(maderaAlacena2B, glm::vec3(32.0, -1.0, -7.0));
		maderaAlacena2B = glm::rotate(maderaAlacena2B, glm::radians(90.0f),glm::vec3(0.0, 0.0, 1.0));
		maderaAlacena2B = glm::rotate(maderaAlacena2B, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		maderaAlacena2B = glm::scale(maderaAlacena2B, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID46);
		techo.render(maderaAlacena2B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertasAlacenaB = glm::mat4(1.0);
		puertasAlacenaB = glm::translate(puertasAlacenaB, glm::vec3(31.0, -4.0, -6.5));
		puertasAlacenaB = glm::rotate(puertasAlacenaB, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		puertasAlacenaB = glm::scale(puertasAlacenaB, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID45);
		techo.render(puertasAlacenaB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertasAlacenaVidrioB = glm::mat4(1.0);
		puertasAlacenaVidrioB = glm::translate(puertasAlacenaVidrioB, glm::vec3(31.0, -2.0, -6.5));
		puertasAlacenaVidrioB = glm::rotate(puertasAlacenaVidrioB, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		puertasAlacenaVidrioB = glm::scale(puertasAlacenaVidrioB, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(puertasAlacenaVidrioB);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Cuadros<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 cuadro1B = glm::mat4(1.0);
		cuadro1B = glm::translate(cuadro1B, glm::vec3(24.0, -2.5, -8.9));
		cuadro1B = glm::scale(cuadro1B, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID48);
		techo.render(cuadro1B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro2B = glm::mat4(1.0);
		cuadro2B = glm::translate(cuadro2B, glm::vec3(24.0, 2.5, -8.9));
		cuadro2B = glm::scale(cuadro2B, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID49);
		techo.render(cuadro2B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro3B = glm::mat4(1.0);
		cuadro3B = glm::translate(cuadro3B, glm::vec3(27.0, -2.5, 8.9));
		cuadro3B = glm::scale(cuadro3B, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID50);
		techo.render(cuadro3B);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro4B = glm::mat4(1.0);
		cuadro4B = glm::translate(cuadro4B, glm::vec3(21.0, -2.5, 8.9));
		cuadro4B = glm::scale(cuadro4B, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID51);
		techo.render(cuadro4B);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 cuadroFantasma2 = glm::mat4(1.0);
		cuadroFantasma2 = glm::translate(cuadroFantasma2, glm::vec3(32.9, 2.0, 4.5));
		cuadroFantasma2 = glm::rotate(cuadroFantasma2,glm::radians(90.0f) ,glm::vec3(0.0, 1.0, 0.0));
		cuadroFantasma2 = glm::scale(cuadroFantasma2, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID48);
		techo.render(cuadroFantasma2);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 cuadroMuerte2 = glm::mat4(1.0);
		cuadroMuerte2 = glm::translate(cuadroMuerte2, glm::vec3(28.0, 2.0, -8.9));
		cuadroMuerte2 = glm::scale(cuadroMuerte2, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID49);
		techo.render(cuadroMuerte2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro5B = glm::mat4(1.0);
		cuadro5B = glm::translate(cuadro5B, glm::vec3(32.9, -2.5, 5.0));
		cuadro5B = glm::rotate(cuadro5B, glm::radians(90.0f),glm::vec3(0.0, 1.0,0.0));
		cuadro5B = glm::scale(cuadro5B, glm::vec3(2.0, 3.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID52);
		techo.render(cuadro5B);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Reyes Magos<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 rey1 = glm::mat4(1.0);
		rey1 = glm::translate(rey1, glm::vec3(12.0, -4.0, 3.0));
		rey1 = glm::scale(rey1, glm::vec3(0.2, 0.5, 0.2));
		glBindTexture(GL_TEXTURE_2D, textureID53);
		sphere3.render(rey1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 rey2 = glm::mat4(1.0);
		rey2 = glm::translate(rey2, glm::vec3(12.5, -4.0, 3.5));
		rey2 = glm::scale(rey2, glm::vec3(0.2, 0.5, 0.2));
		glBindTexture(GL_TEXTURE_2D, textureID54);
		sphere3.render(rey2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 rey3 = glm::mat4(1.0);
		rey3 = glm::translate(rey3, glm::vec3(13.0, -4.0, 4.0));
		rey3 = glm::scale(rey3, glm::vec3(0.2, 0.5, 0.2));
		glBindTexture(GL_TEXTURE_2D, textureID55);
		sphere3.render(rey3);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Regalos<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 regalo1 = glm::mat4(1.0);
		regalo1 = glm::translate(regalo1, glm::vec3(13.0, -3.5, 0.5));
		regalo1 = glm::scale(regalo1, glm::vec3(1.0, 1.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID56);
		box2.render(regalo1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 regalo2 = glm::mat4(1.0);
		regalo2 = glm::translate(regalo2, glm::vec3(13.5, -3.5, 1.0));
		regalo2 = glm::scale(regalo2, glm::vec3(1.0, 1.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID57);
		box2.render(regalo2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 regalo3 = glm::mat4(1.0);
		regalo3 = glm::translate(regalo3, glm::vec3(14.0, -3.5, 1.5));
		regalo3 = glm::scale(regalo3, glm::vec3(1.0, 1.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID58);
		box2.render(regalo3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 regalo4 = glm::mat4(1.0);
		regalo4 = glm::translate(regalo4, glm::vec3(14.5, -3.5, 2.0));
		regalo4 = glm::scale(regalo4, glm::vec3(1.0, 1.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID56);
		box2.render(regalo4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//----------------------------------------------Modelos----------------------------------------------------
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Camas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 matrixModelBedB = glm::mat4(1.0);
		matrixModelBedB = glm::translate(matrixModelBedB, glm::vec3(13.2, 0.0, 5.0));
		matrixModelBedB = glm::rotate(matrixModelBedB, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBedB = glm::scale(matrixModelBedB, glm::vec3(0.2, 0.2, 0.2));
		modelBed.render(matrixModelBedB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelBed2B = glm::mat4(1.0);
		matrixModelBed2B = glm::translate(matrixModelBed2B, glm::vec3(14.9, 0.0, -5.0));
		matrixModelBed2B = glm::rotate(matrixModelBed2B, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBed2B = glm::scale(matrixModelBed2B, glm::vec3(0.02, 0.02, 0.02));
		modelBed2.render(matrixModelBed2B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToiletB = glm::mat4(1.0);
		matrixModelToiletB = glm::translate(matrixModelToiletB, glm::vec3(13.0, -5.0, -0.9));
		matrixModelToiletB = glm::rotate(matrixModelToiletB, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelToiletB = glm::scale(matrixModelToiletB, glm::vec3(0.002, 0.002, 0.002));
		modelBed4.render(matrixModelToiletB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChairB = glm::mat4(1.0);
		matrixModelChairB = glm::translate(matrixModelChairB, glm::vec3(13.0, -5.0, -6.0));
		matrixModelChairB = glm::scale(matrixModelChairB, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChairB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelSofaB = glm::mat4(1.0);
		matrixModelSofaB = glm::translate(matrixModelSofaB, glm::vec3(20.5, -5.0, -2.0));
		matrixModelSofaB = glm::rotate(matrixModelSofaB, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		matrixModelSofaB = glm::scale(matrixModelSofaB, glm::vec3(0.0022, 0.0022, 0.0022));
		modelBed6.render(matrixModelSofaB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTableB = glm::mat4(1.0);
		matrixModelTableB = glm::translate(matrixModelTableB, glm::vec3(13.0, -5.0, -4.0));
		matrixModelTableB = glm::rotate(matrixModelTableB, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//matrixModelTableB = glm::rotate(matrixModelTableB, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTableB = glm::scale(matrixModelTableB, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTableB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTVB = glm::mat4(1.0);
		matrixModelTVB = glm::translate(matrixModelTVB, glm::vec3(18.5, -3.0, -8.7));
		matrixModelTVB = glm::rotate(matrixModelTVB, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTVB = glm::scale(matrixModelTVB, glm::vec3(0.3, 0.3, 0.3));
		modelTV.render(matrixModelTVB);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable2B = glm::mat4(1.0);
		matrixModelTable2B = glm::translate(matrixModelTable2B, glm::vec3(18.5, -5.0, 2.0));
		matrixModelTable2B = glm::rotate(matrixModelTable2B, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTable2B = glm::scale(matrixModelTable2B, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable2B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToilet2B = glm::mat4(1.0);
		matrixModelToilet2B = glm::translate(matrixModelToilet2B, glm::vec3(29.5, 0.0, 8.0));
		matrixModelToilet2B = glm::rotate(matrixModelToilet2B, glm::radians(180.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelToilet2B = glm::scale(matrixModelToilet2B, glm::vec3(0.002, 0.002, 0.002));
		modelBed4.render(matrixModelToilet2B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable3B = glm::mat4(1.0);
		matrixModelTable3B = glm::translate(matrixModelTable3B, glm::vec3(27.5, -5.0, 4.5));
		matrixModelTable3B = glm::rotate(matrixModelTable3B, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//matrixModelTable3B = glm::rotate(matrixModelTable3B, glm::radians(9.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTable3B = glm::scale(matrixModelTable3B, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable3B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair3B = glm::mat4(1.0);
		matrixModelChair3B = glm::translate(matrixModelChair3B, glm::vec3(27.5, -5.0, 2.0));
		matrixModelChair3B = glm::scale(matrixModelChair3B, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair3B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair4B = glm::mat4(1.0);
		matrixModelChair4B = glm::translate(matrixModelChair4B, glm::vec3(27.5, -5.0, 7.0));
		matrixModelChair4B = glm::rotate(matrixModelChair4B,glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		matrixModelChair4B = glm::scale(matrixModelChair4B, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair4B);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTree = glm::mat4(1.0);
		matrixModelTree = glm::translate(matrixModelTree, glm::vec3(13.5, -5.0, 4.0));
		matrixModelTree = glm::rotate(matrixModelTree, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTree = glm::scale(matrixModelTree, glm::vec3(0.028, 0.028, 0.028));
		modelChristmasTree.render(matrixModelTree);
		glActiveTexture(GL_TEXTURE0);


		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Terreno<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 modelGround = glm::mat4(1.0);
		modelGround = glm::translate(modelGround, glm::vec3(0.0, -5.2, 0.0));
		modelGround = glm::rotate(modelGround, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelGround = glm::scale(modelGround, glm::vec3(100.0, 100.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		techo.render(modelGround);
		glBindTexture(GL_TEXTURE_2D, 0);

/*------------------------------------------------------------------------------------------------------------------------
														CASA TERROR
--------------------------------------------------------------------------------------------------------------------------*/
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 1 - Pared derecha de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1ModelA = glm::mat4(1.0);
		pared1ModelA = glm::translate(pared1ModelA, glm::vec3(-15.0, -2.5, -5.4));
		pared1ModelA = glm::rotate(pared1ModelA, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1ModelA = glm::scale(pared1ModelA, glm::vec3(7.2, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(pared1ModelA);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 2 - Pared izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model2A = glm::mat4(1.0);
		pared1Model2A = glm::translate(pared1Model2A, glm::vec3(-8.0, 0.0, 0.0));
		pared1Model2A = glm::rotate(pared1Model2A, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model2A = glm::scale(pared1Model2A, glm::vec3(18.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model2A);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 3 - Parte trasera de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model3A = glm::mat4(1.0);
		pared1Model3A = glm::translate(pared1Model3A, glm::vec3(-20.5, 0.0, -9.0));
		pared1Model3A = glm::scale(pared1Model3A, glm::vec3(25.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model3A);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 4 - Parte izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model45A = glm::mat4(1.0);
		pared1Model45A = glm::translate(pared1Model45A, glm::vec3(-33.0, 0.0, 0.0));
		pared1Model45A = glm::rotate(pared1Model45A, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model45A = glm::scale(pared1Model45A, glm::vec3(18.0, 10.0, 0.01));
		paredGoldenrod.render(pared1Model45A);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 5 - Parte frontal de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model44A = glm::mat4(1.0);
		pared1Model44A = glm::translate(pared1Model44A, glm::vec3(-24.0, -2.5, 9.0));
		pared1Model44A = glm::scale(pared1Model44A, glm::vec3(18.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,pared1Model44A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,pared1Model44A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model4A = glm::mat4(1.0);
		pared1Model4A = glm::translate(pared1Model4A, glm::vec3(-20.5, 5.0, 0.0));
		pared1Model4A = glm::rotate(pared1Model4A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model4A = glm::rotate(pared1Model4A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model4A = glm::scale(pared1Model4A, glm::vec3(18.0, 25.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		techo.render(pared1Model4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Piso planta baja<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 piso1 = glm::mat4(1.0);
		piso1 = glm::translate(piso1, glm::vec3(-20.5, -5.0, 0.0));
		piso1 = glm::rotate(piso1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		piso1 = glm::rotate(piso1, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		piso1 = glm::scale(piso1, glm::vec3(18.0, 25.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID27);
		paredGoldenrod.render(piso1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo 1er Piso<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model6A = glm::mat4(1.0);
		pared1Model6A = glm::translate(pared1Model6A, glm::vec3(-11.5, 0.0, -5.4));
		pared1Model6A = glm::rotate(pared1Model6A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model6A = glm::rotate(pared1Model6A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model6A = glm::scale(pared1Model6A, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model6A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model7A = glm::mat4(1.0);
		pared1Model7A = glm::translate(pared1Model7A, glm::vec3(-11.5, 0.0, 5.4));
		pared1Model7A = glm::rotate(pared1Model7A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model7A = glm::rotate(pared1Model7A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model7A = glm::scale(pared1Model7A, glm::vec3(7.2, 7.0, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model7A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model8A = glm::mat4(1.0);
		pared1Model8A = glm::translate(pared1Model8A, glm::vec3(-9.5, 0.0, 0.0));
		pared1Model8A = glm::rotate(pared1Model8A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		pared1Model8A = glm::rotate(pared1Model8A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pared1Model8A = glm::scale(pared1Model8A, glm::vec3(3.6, 3.0, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(pared1Model8A);

		glm::mat4 techo2 = glm::mat4(1.0);
		techo2 = glm::translate(techo2, glm::vec3(-24.0, 0.0, 0.0));
		techo2 = glm::rotate(techo2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		techo2 = glm::rotate(techo2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		techo2 = glm::scale(techo2, glm::vec3(18.0, 18.0, 0.01));
		paredGoldenrod.render(techo2);

		glm::mat4 techo3 = glm::mat4(1.0);
		techo3 = glm::translate(techo3, glm::vec3(-13.0, 0.0, 0.9));
		techo3 = glm::rotate(techo3, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		techo3 = glm::rotate(techo3, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		techo3 = glm::scale(techo3, glm::vec3(1.8, 4.0, 0.01));
		paredGoldenrod.render(techo3);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Paredes de las habitaciones<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredHA = glm::mat4(1.0);
		paredHA = glm::translate(paredHA, glm::vec3(-13.0, 0.0, -1.8));
		paredHA = glm::scale(paredHA, glm::vec3(4.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(paredHA);

		glm::mat4 pared1HA = glm::mat4(1.0);
		pared1HA = glm::translate(paredHA, glm::vec3(-13.0, 2.5, 1.8));
		pared1HA = glm::scale(pared1HA, glm::vec3(4.0, 5.0, 0.01));
		paredGoldenrod.render(pared1HA);

		glm::mat4 paredH2A = glm::mat4(1.0);
		paredH2A = glm::translate(paredH2A, glm::vec3(-22.0, 2.5, -5.4));
		paredH2A = glm::rotate(paredH2A, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		paredH2A = glm::scale(paredH2A, glm::vec3(7.5, 5.0, 0.01));
		paredGoldenrod.render(paredH2A);

		glm::mat4 paredH3A = glm::mat4(1.0);
		paredH3A = glm::translate(paredH3A, glm::vec3(-22.0, 2.5, 5.4));
		paredH3A = glm::rotate(paredH3A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredH3A = glm::scale(paredH3A, glm::vec3(7.2, 5.0, 0.01));
		paredGoldenrod.render(paredH3A);

		glm::mat4 paredH4A = glm::mat4(1.0);
		paredH4A = glm::translate(paredH4A, glm::vec3(-26.0, 2.5, 4.5));
		paredH4A = glm::rotate(paredH4A, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredH4A = glm::scale(paredH4A, glm::vec3(9.0, 5.0, 0.01));
		techo.render(paredH4A);

		glm::mat4 paredH5A = glm::mat4(1.0);
		paredH5A = glm::translate(paredH5A, glm::vec3(-18.5, 2.5, -1.8));
		paredH5A = glm::scale(paredH5A, glm::vec3(7.0, 5.0, 0.01));
		paredGoldenrod.render(paredH5A);

		glm::mat4 paredH6A = glm::mat4(1.0);
		paredH6A = glm::translate(paredH6A, glm::vec3(-16.5, 2.5, 1.8));
		paredH6A = glm::scale(paredH6A, glm::vec3(11.0, 5.0, 0.01));
		paredGoldenrod.render(paredH6A);

		glm::mat4 paredH7A = glm::mat4(1.0);
		paredH7A = glm::translate(paredH7A, glm::vec3(-30.5, 2.5, 0.0));
		paredH7A = glm::scale(paredH7A, glm::vec3(5.0, 5.0, 0.01));
		paredGoldenrod.render(paredH7A);

		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredH8A = glm::mat4(1.0);
		paredH8A = glm::translate(paredH8A, glm::vec3(-29.5, 2.5, 9.0));
		paredH8A = glm::scale(paredH8A, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,paredH8A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredH8A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredH9A = glm::mat4(1.0);
		paredH9A = glm::translate(paredH9A, glm::vec3(-18.5, 2.5, 9.0));
		paredH9A = glm::scale(paredH9A, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,paredH9A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredH9A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model11A = glm::mat4(1.0);
		pared1Model11A = glm::translate(pared1Model11A, glm::vec3(-9.5, 4.5, 1.8));
		pared1Model11A = glm::scale(pared1Model11A, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		techo.render(pared1Model11A);

		glm::mat4 pared1Model12A = glm::mat4(1.0);
		pared1Model12A = glm::translate(pared1Model12A, glm::vec3(-8.25, 2.0, 1.8));
		pared1Model12A = glm::scale(pared1Model12A, glm::vec3(0.5, 4.0, 0.01));
		paredGoldenrod.render(pared1Model12A);

		glm::mat4 pared1Model13A = glm::mat4(1.0);
		pared1Model13A = glm::translate(pared1Model13A, glm::vec3(-10.75, 2.0, 1.8));
		pared1Model13A = glm::scale(pared1Model13A, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model13A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta1A = glm::mat4(1.0);
		puerta1A = glm::translate(puerta1A, glm::vec3(-9.5, 2.0, 1.8));
		puerta1A = glm::scale(puerta1A, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		techo.render(puerta1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model14A = glm::mat4(1.0);
		pared1Model14A = glm::translate(pared1Model14A, glm::vec3(-9.5, 4.5, -1.8));
		pared1Model14A = glm::scale(pared1Model14A, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(pared1Model14A);

		glm::mat4 pared1Model15A = glm::mat4(1.0);
		pared1Model15A = glm::translate(pared1Model15A, glm::vec3(-8.25, 2.0, -1.8));
		pared1Model15A = glm::scale(pared1Model15A, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model15A);

		glm::mat4 pared1Model16A = glm::mat4(1.0);
		pared1Model16A = glm::translate(pared1Model16A, glm::vec3(-10.75, 2.0, -1.8));
		pared1Model16A = glm::scale(pared1Model16A, glm::vec3(0.5, 4.0, 0.01));
		techo.render(pared1Model16A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta2A = glm::mat4(1.0);
		puerta2A = glm::translate(puerta2A, glm::vec3(-9.5, 2.0, -1.8));
		puerta2A = glm::scale(puerta2A, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID28);
		techo.render(puerta2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta3<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 puerta3 = glm::mat4(1.0);
		puerta3 = glm::translate(puerta3, glm::vec3(-11.0, -3.0, -0.9));
		puerta3 = glm::scale(puerta3, glm::vec3(0.01, 4.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		techo.render(puerta3);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta4<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 marco4 = glm::mat4(1.0);
		marco4 = glm::translate(marco4, glm::vec3(-27.0, 4.5, 0.0));
		marco4 = glm::scale(marco4, glm::vec3(2.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		techo.render(marco4);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 puerta4 = glm::mat4(1.0);
		puerta4 = glm::translate(puerta4, glm::vec3(-27.0, 2.0, 0.0));
		puerta4 = glm::scale(puerta4, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID30);
		techo.render(puerta4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 escaleraModel1A = glm::mat4(1.0);
		escaleraModel1A = glm::translate(escaleraModel1A, glm::vec3(-14.375, -3.75, 0.0));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel1A = glm::scale(escaleraModel1A, glm::vec3(1.25, 2.5, 3.6));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel1A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel2A = glm::mat4(1.0);
		escaleraModel2A = glm::translate(escaleraModel2A, glm::vec3(-13.4, -4.0, 0.9));
		//escaleraModel1 = glm::rotate(escaleraModel1, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		escaleraModel2A = glm::scale(escaleraModel2A, glm::vec3(0.7, 2.0, 1.8));
		//glBindTexture(GL_TEXTURE_2D, textureID9);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		techo.render(escaleraModel2A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel3A = glm::mat4(1.0);
		escaleraModel3A = glm::translate(escaleraModel3A, glm::vec3(-12.7, -4.25, 0.9));
		escaleraModel3A = glm::scale(escaleraModel3A, glm::vec3(0.7, 1.5, 1.8));
		techo.render(escaleraModel3A);

		glm::mat4 escaleraModel4A = glm::mat4(1.0);
		escaleraModel4A = glm::translate(escaleraModel4A, glm::vec3(-12.0, -4.5, 0.9));
		escaleraModel4A = glm::scale(escaleraModel4A, glm::vec3(0.7, 1.0, 1.8));
		techo.render(escaleraModel4A);

		glm::mat4 escaleraModel5A = glm::mat4(1.0);
		escaleraModel5A = glm::translate(escaleraModel5A, glm::vec3(-11.35, -4.75, 0.9));
		escaleraModel5A = glm::scale(escaleraModel5A, glm::vec3(0.7, 0.5, 1.8));
		techo.render(escaleraModel5A);

		glm::mat4 escaleraModel6A = glm::mat4(1.0);
		escaleraModel6A = glm::translate(escaleraModel6A, glm::vec3(-13.4, -2.1875, -0.9));
		escaleraModel6A = glm::scale(escaleraModel6A, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel6A);

		glm::mat4 escaleraModel7A = glm::mat4(1.0);
		escaleraModel7A = glm::translate(escaleraModel7A, glm::vec3(-12.7, -1.5625, -0.9));
		escaleraModel7A = glm::scale(escaleraModel7A, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel7A);

		glm::mat4 escaleraModel8A = glm::mat4(1.0);
		escaleraModel8A = glm::translate(escaleraModel8A, glm::vec3(-12.0, -0.9375, -0.9));
		escaleraModel8A = glm::scale(escaleraModel8A, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel8A);

		glm::mat4 escaleraModel9A = glm::mat4(1.0);
		escaleraModel9A = glm::translate(escaleraModel9A, glm::vec3(-11.35, -0.3125, -0.9));
		escaleraModel9A = glm::scale(escaleraModel9A, glm::vec3(0.7, 0.625, 1.8));
		techo.render(escaleraModel9A);

		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared de las escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredEscaleraModel1A = glm::mat4(1.0);
		paredEscaleraModel1A = glm::translate(paredEscaleraModel1A, glm::vec3(-11.35, -2.8125, 0.0));
		paredEscaleraModel1A = glm::scale(paredEscaleraModel1A, glm::vec3(0.7, 4.375, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel1A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel2A = glm::mat4(1.0);
		paredEscaleraModel2A = glm::translate(paredEscaleraModel2A, glm::vec3(-12.05, -3.125, 0.0));
		paredEscaleraModel2A = glm::scale(paredEscaleraModel2A, glm::vec3(0.7, 3.75, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel2A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel3A = glm::mat4(1.0);
		paredEscaleraModel3A = glm::translate(paredEscaleraModel3A, glm::vec3(-12.75, -3.4375, 0.0));
		paredEscaleraModel3A = glm::scale(paredEscaleraModel3A, glm::vec3(0.7, 3.125, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel3A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel4A = glm::mat4(1.0);
		paredEscaleraModel4A = glm::translate(paredEscaleraModel4A, glm::vec3(-13.45, -3.75, 0.0));
		paredEscaleraModel4A = glm::scale(paredEscaleraModel4A, glm::vec3(0.7, 2.5, 0.01));
		//glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV",glm::value_ptr(glm::vec2(2.0, 1.0)));
		paredGoldenrod.render(paredEscaleraModel4A);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell15A = glm::mat4(1.0);
		paredEscaleraModell15A = glm::translate(paredEscaleraModell15A, glm::vec3(-11.0, -0.5, -0.9));
		paredEscaleraModell15A = glm::scale(paredEscaleraModell15A, glm::vec3(0.01, 1.0, 1.8));
		paredGoldenrod.render(paredEscaleraModell15A);

		glm::mat4 paredEscaleraModell16A = glm::mat4(1.0);
		paredEscaleraModell16A = glm::translate(paredEscaleraModell16A, glm::vec3(-15.0, -2.5, 0.0));
		paredEscaleraModell16A = glm::rotate(paredEscaleraModell16A, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		paredEscaleraModell16A = glm::scale(paredEscaleraModell16A, glm::vec3(3.6, 5.0, 0.01));
		techo.render(paredEscaleraModell16A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared frontal superior<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredFrontalModel1A = glm::mat4(1.0);
		paredFrontalModel1A = glm::translate(paredFrontalModel1A, glm::vec3(-11.5, 4.25, 9.0));
		paredFrontalModel1A = glm::scale(paredFrontalModel1A, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,paredFrontalModel1A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredFrontalModel1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel2A = glm::mat4(1.0);
		paredFrontalModel2A = glm::translate(paredFrontalModel2A, glm::vec3(-11.5, 0.75, 9.0));
		paredFrontalModel2A = glm::scale(paredFrontalModel2A, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		paredGoldenrod.render(0,8,paredFrontalModel2A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredFrontalModel2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel3A = glm::mat4(1.0);
		paredFrontalModel3A = glm::translate(paredFrontalModel3A, glm::vec3(-14.25, 2.5, 9.0));
		paredFrontalModel3A = glm::scale(paredFrontalModel3A, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,paredFrontalModel3A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredFrontalModel3A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredFrontalModel4A = glm::mat4(1.0);
		paredFrontalModel4A = glm::translate(paredFrontalModel4A, glm::vec3(-9.5, 2.5, 9.0));
		paredFrontalModel4A = glm::scale(paredFrontalModel4A, glm::vec3(3.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		paredGoldenrod.render(0,8,paredFrontalModel4A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,paredFrontalModel4A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model17A = glm::mat4(1.0);
		pared1Model17A = glm::translate(pared1Model17A, glm::vec3(-11.5, -2.5, 9.0));
		pared1Model17A = glm::scale(pared1Model17A, glm::vec3(7.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID22);
		paredGoldenrod.render(0,8,pared1Model17A);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID24);
		paredGoldenrod.render(9,20,pared1Model17A);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 puertaPrincipalA = glm::mat4(1.0);
		puertaPrincipalA = glm::translate(puertaPrincipalA, glm::vec3(-11.5, -3.25, 9.0));
		puertaPrincipalA = glm::scale(puertaPrincipalA, glm::vec3(2.0, 3.5, 0.1));
		glBindTexture(GL_TEXTURE_2D, textureID29);
		paredGoldenrod.render(puertaPrincipalA);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ventanas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 window1A = glm::mat4(1.0);
		window1A = glm::translate(window1A, glm::vec3(-12.25, 2.5, 9.0));
		window1A = glm::scale(window1A, glm::vec3(2.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window1A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window2A = glm::mat4(1.0);
		window2A = glm::translate(window2A, glm::vec3(-19.5, -2.5, 0.0));
		window2A = glm::scale(window2A, glm::vec3(9.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window2A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window3A = glm::mat4(1.0);
		window3A = glm::translate(window3A, glm::vec3(-24.0, 2.5, 9.0));
		window3A = glm::scale(window3A, glm::vec3(4.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(window3A);
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

		glm::mat4 modelBaseLamp5A = glm::mat4(1.0);
		modelBaseLamp5A = glm::translate(modelBaseLamp5A, glm::vec3(-18.5, 4.95, 0.0));
		modelBaseLamp5A = glm::scale(modelBaseLamp5A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp5A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp5A = glm::mat4(1.0);
		modelLamp5A = glm::translate(modelLamp5A, glm::vec3(-18.5, 4.9, 0.0));
		modelLamp5A = glm::scale(modelLamp5A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp5A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp6A = glm::mat4(1.0);
		modelBaseLamp6A = glm::translate(modelBaseLamp6A, glm::vec3(-17.0, 4.95, 5.4));
		modelBaseLamp6A = glm::scale(modelBaseLamp6A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp6A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp6A = glm::mat4(1.0);
		modelLamp6A = glm::translate(modelLamp6A, glm::vec3(-17.0, 4.9, 5.4));
		modelLamp6A = glm::scale(modelLamp6A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp6A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp7A = glm::mat4(1.0);
		modelBaseLamp7A = glm::translate(modelBaseLamp7A, glm::vec3(-17.0, 4.95, -5.4));
		modelBaseLamp7A = glm::scale(modelBaseLamp7A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp7A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp7A = glm::mat4(1.0);
		modelLamp7A = glm::translate(modelLamp7A, glm::vec3(-17.0, 4.9, -5.4));
		modelLamp7A = glm::scale(modelLamp7A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp7A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp8A = glm::mat4(1.0);
		modelBaseLamp8A = glm::translate(modelBaseLamp8A, glm::vec3(-26.5, 4.95, -5.0));
		modelBaseLamp8A = glm::scale(modelBaseLamp8A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp8A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp8A = glm::mat4(1.0);
		modelLamp8A = glm::translate(modelLamp8A, glm::vec3(-26.5, 4.9, -5.0));
		modelLamp8A = glm::scale(modelLamp8A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp8A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp9A = glm::mat4(1.0);
		modelBaseLamp9A = glm::translate(modelBaseLamp9A, glm::vec3(-29.0, 4.95, 4.5));
		modelBaseLamp9A = glm::scale(modelBaseLamp9A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp9A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp9A = glm::mat4(1.0);
		modelLamp9A = glm::translate(modelLamp9A, glm::vec3(-29.0, 4.9, 4.5));
		modelLamp9A = glm::scale(modelLamp9A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp9A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp10A = glm::mat4(1.0);
		modelBaseLamp10A = glm::translate(modelBaseLamp10A, glm::vec3(-11.0, 4.95, 0.0));
		modelBaseLamp10A = glm::scale(modelBaseLamp10A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp10A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp10A = glm::mat4(1.0);
		modelLamp10A = glm::translate(modelLamp10A, glm::vec3(-11.0, 4.9, 0.0));
		modelLamp10A = glm::scale(modelLamp10A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp10A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp11A = glm::mat4(1.0);
		modelBaseLamp11A = glm::translate(modelBaseLamp11A, glm::vec3(-20.0, -0.1, 5.4));
		modelBaseLamp11A = glm::scale(modelBaseLamp11A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp11A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp11A = glm::mat4(1.0);
		modelLamp11A = glm::translate(modelLamp11A, glm::vec3(-20.0, -0.15, 5.4));
		modelLamp11A = glm::scale(modelLamp11A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp11A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp12A = glm::mat4(1.0);
		modelBaseLamp12A = glm::translate(modelBaseLamp12A, glm::vec3(-20.0, -0.1, -5.4));
		modelBaseLamp12A = glm::scale(modelBaseLamp12A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp12A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp12A = glm::mat4(1.0);
		modelLamp12A = glm::translate(modelLamp12A, glm::vec3(-20.0, -0.15, -5.4));
		modelLamp12A = glm::scale(modelLamp12A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp12A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp13A = glm::mat4(1.0);
		modelBaseLamp13A = glm::translate(modelBaseLamp13A, glm::vec3(-28.0, -0.1, 5.4));
		modelBaseLamp13A = glm::scale(modelBaseLamp13A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp13A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp13A = glm::mat4(1.0);
		modelLamp13A = glm::translate(modelLamp13A, glm::vec3(-28.0, -0.15, 5.4));
		modelLamp13A = glm::scale(modelLamp13A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp13A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp14A = glm::mat4(1.0);
		modelBaseLamp14A = glm::translate(modelBaseLamp14A, glm::vec3(-28.0, -0.1, -5.4));
		modelBaseLamp14A = glm::scale(modelBaseLamp14A, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinderBaseLamp.render(modelBaseLamp14A);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp14A = glm::mat4(1.0);
		modelLamp14A = glm::translate(modelLamp14A, glm::vec3(-28.0, -0.15, -5.4));
		modelLamp14A = glm::scale(modelLamp14A, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphereLamp2.render(modelLamp14A);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Muebles<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 buro1 = glm::mat4(1.0);
		buro1 = glm::translate(buro1, glm::vec3(-21.0, 1.0, 2.8));
		buro1 = glm::scale(buro1, glm::vec3(2.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		techo.render(buro1);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 ropero1 = glm::mat4(1.0);
		ropero1 = glm::translate(ropero1, glm::vec3(-21.0, 2.0, 6.3));
		ropero1 = glm::scale(ropero1, glm::vec3(2.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		techo.render(ropero1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 buro2 = glm::mat4(1.0);
		buro2 = glm::translate(buro2, glm::vec3(-21.0, 1.0, -2.8));
		buro2 = glm::scale(buro2, glm::vec3(2.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		techo.render(buro2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 ropero2 = glm::mat4(1.0);
		ropero2 = glm::translate(ropero2, glm::vec3(-21.0, 2.0, -6.3));
		ropero2 = glm::scale(ropero2, glm::vec3(2.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		techo.render(ropero2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cancel1 = glm::mat4(1.0);
		cancel1 = glm::translate(cancel1, glm::vec3(-30.0, 2.5, 1.5));
		cancel1 = glm::rotate(cancel1, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		cancel1 = glm::scale(cancel1, glm::vec3(3.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID47);
		techo.render(cancel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cancel2 = glm::mat4(1.0);
		cancel2 = glm::translate(cancel2, glm::vec3(-31.5, 2.5, 3.0));
		cancel2 = glm::scale(cancel2, glm::vec3(3.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID47);
		techo.render(cancel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 maderaAlacena = glm::mat4(1.0);
		maderaAlacena = glm::translate(maderaAlacena, glm::vec3(-32.0, -3.0, -4.0));
		maderaAlacena = glm::scale(maderaAlacena, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID46);
		techo.render(maderaAlacena);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 maderaAlacena2 = glm::mat4(1.0);
		maderaAlacena2 = glm::translate(maderaAlacena2, glm::vec3(-32.0, -1.0, -7.0));
		maderaAlacena2 = glm::rotate(maderaAlacena2, glm::radians(90.0f),glm::vec3(0.0, 0.0, 1.0));
		maderaAlacena2 = glm::rotate(maderaAlacena2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		maderaAlacena2 = glm::scale(maderaAlacena2, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID46);
		techo.render(maderaAlacena2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertasAlacena = glm::mat4(1.0);
		puertasAlacena = glm::translate(puertasAlacena, glm::vec3(-31.0, -4.0, -6.5));
		puertasAlacena = glm::rotate(puertasAlacena, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		puertasAlacena = glm::scale(puertasAlacena, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID45);
		techo.render(puertasAlacena);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puertasAlacenaVidrio = glm::mat4(1.0);
		puertasAlacenaVidrio = glm::translate(puertasAlacenaVidrio, glm::vec3(-31.0, -2.0, -6.5));
		puertasAlacenaVidrio = glm::rotate(puertasAlacenaVidrio, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		puertasAlacenaVidrio = glm::scale(puertasAlacenaVidrio, glm::vec3(5.0, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		techo.render(puertasAlacenaVidrio);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Cuadros<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 cuadro1 = glm::mat4(1.0);
		cuadro1 = glm::translate(cuadro1, glm::vec3(-24.0, -2.5, -8.9));
		cuadro1 = glm::scale(cuadro1, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID31);
		techo.render(cuadro1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro2 = glm::mat4(1.0);
		cuadro2 = glm::translate(cuadro2, glm::vec3(-24.0, 2.5, -8.9));
		cuadro2 = glm::scale(cuadro2, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID32);
		techo.render(cuadro2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro3 = glm::mat4(1.0);
		cuadro3 = glm::translate(cuadro3, glm::vec3(-27.0, -2.5, 8.9));
		cuadro3 = glm::scale(cuadro3, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID33);
		techo.render(cuadro3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro4 = glm::mat4(1.0);
		cuadro4 = glm::translate(cuadro4, glm::vec3(-21.0, -2.5, 8.9));
		cuadro4 = glm::scale(cuadro4, glm::vec3(1.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID34);
		techo.render(cuadro4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadroIronMan = glm::mat4(1.0);
		cuadroIronMan = glm::translate(cuadroIronMan, glm::vec3(-18.5, -2.7, 1.1));
		cuadroIronMan = glm::scale(cuadroIronMan, glm::vec3(1.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID42);
		techo.render(cuadroIronMan);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 cuadroFantasma = glm::mat4(1.0);
		cuadroFantasma = glm::translate(cuadroFantasma, glm::vec3(-32.9, 2.0, 4.5));
		cuadroFantasma = glm::rotate(cuadroFantasma,glm::radians(90.0f) ,glm::vec3(0.0, 1.0, 0.0));
		cuadroFantasma = glm::scale(cuadroFantasma, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID43);
		techo.render(cuadroFantasma);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glm::mat4 cuadroMuerte = glm::mat4(1.0);
		cuadroMuerte = glm::translate(cuadroMuerte, glm::vec3(-28.0, 2.0, -8.9));
		cuadroMuerte = glm::scale(cuadroMuerte, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID44);
		techo.render(cuadroMuerte);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 cuadro5 = glm::mat4(1.0);
		cuadro5 = glm::translate(cuadro5, glm::vec3(-32.9, -2.5, 5.0));
		cuadro5 = glm::rotate(cuadro5, glm::radians(90.0f),glm::vec3(0.0, 1.0,0.0));
		cuadro5 = glm::scale(cuadro5, glm::vec3(2.0, 3.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID32);
		techo.render(cuadro5);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Papel Picado<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 papelpicado1 = glm::mat4(1.0);
		papelpicado1 = glm::translate(papelpicado1, glm::vec3(-19.5, -4.0, 3.2));
		papelpicado1 = glm::scale(papelpicado1, glm::vec3(1.5,1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID35);
		techo.render(papelpicado1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 papelpicado2 = glm::mat4(1.0);
		papelpicado2 = glm::translate(papelpicado2, glm::vec3(-17.5, -4.0, 3.2));
		papelpicado2 = glm::scale(papelpicado2, glm::vec3(1.5, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID36);
		techo.render(papelpicado2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 papelpicado3 = glm::mat4(1.0);
		papelpicado3 = glm::translate(papelpicado3, glm::vec3(-19.5, -4.0, 0.9));
		papelpicado3 = glm::scale(papelpicado3, glm::vec3(1.5, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID37);
		techo.render(papelpicado3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 papelpicado4 = glm::mat4(1.0);
		papelpicado4 = glm::translate(papelpicado4, glm::vec3(-17.5, -4.0, 0.9));
		papelpicado4 = glm::scale(papelpicado4, glm::vec3(1.5, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID38);
		techo.render(papelpicado4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 papelpicado5 = glm::mat4(1.0);
		papelpicado5 = glm::translate(papelpicado5, glm::vec3(-16.8, -4.0, 2.0));
		papelpicado5 = glm::rotate(papelpicado5, glm::radians(90.0f),glm::vec3(0.0, 1.0, 0.0));
		papelpicado5 = glm::scale(papelpicado5, glm::vec3(1.5, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID39);
		techo.render(papelpicado5);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 papelpicado6 = glm::mat4(1.0);
		papelpicado6 = glm::translate(papelpicado6, glm::vec3(-20.2, -4.0, 2.0));
		papelpicado6 = glm::rotate(papelpicado6, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		papelpicado6 = glm::scale(papelpicado6, glm::vec3(1.5, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID39);
		techo.render(papelpicado6);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pan Muerto<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pan1 = glm::mat4(1.0);
		pan1 = glm::translate(pan1, glm::vec3(-19.5, -3.5, 2.0));
		pan1 = glm::scale(pan1, glm::vec3(0.5, 0.1, 0.1));
		glBindTexture(GL_TEXTURE_2D, textureID40);
		sphere3.render(pan1);

		glm::mat4 pan11 = glm::mat4(1.0);
		pan11 = glm::translate(pan11, glm::vec3(-19.5, -3.5, 2.0));
		pan11 = glm::rotate(pan11, glm::radians(45.0f),glm::vec3(0.0, 1.0, 0.0));
		pan11 = glm::scale(pan11, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan11);

		glm::mat4 pan12 = glm::mat4(1.0);
		pan12 = glm::translate(pan12, glm::vec3(-19.5, -3.5, 2.0));
		pan12 = glm::rotate(pan12, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pan12 = glm::scale(pan12, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan12);

		glm::mat4 pan13 = glm::mat4(1.0);
		pan13 = glm::translate(pan13, glm::vec3(-19.5, -3.5, 2.0));
		pan13 = glm::rotate(pan13, glm::radians(135.0f), glm::vec3(0.0, 1.0, 0.0));
		pan13 = glm::scale(pan13, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan13);

		glm::mat4 pan14 = glm::mat4(1.0);
		pan14 = glm::translate(pan14, glm::vec3(-19.5, -3.5, 2.0));
		pan14 = glm::rotate(pan14, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		pan14 = glm::scale(pan14, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan14);

		glm::mat4 basePan1 = glm::mat4(1.0);
		basePan1 = glm::translate(basePan1, glm::vec3(-19.5, -3.51, 2.0));
		basePan1 = glm::scale(basePan1, glm::vec3(0.44, 0.1, 0.44));
		sphere3.render(basePan1);

		glm::mat4 pan2 = glm::mat4(1.0);
		pan2 = glm::translate(pan2, glm::vec3(-19.0, -3.5, 2.0));
		pan2 = glm::scale(pan2, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan2);

		glm::mat4 pan21 = glm::mat4(1.0);
		pan21 = glm::translate(pan21, glm::vec3(-19.0, -3.5, 2.0));
		pan21 = glm::rotate(pan21, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
		pan21 = glm::scale(pan21, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan21);

		glm::mat4 pan22 = glm::mat4(1.0);
		pan22 = glm::translate(pan22, glm::vec3(-19.0, -3.5, 2.0));
		pan22 = glm::rotate(pan22, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pan22 = glm::scale(pan22, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan22);

		glm::mat4 pan23 = glm::mat4(1.0);
		pan23 = glm::translate(pan23, glm::vec3(-19.0, -3.5, 2.0));
		pan23 = glm::rotate(pan23, glm::radians(135.0f), glm::vec3(0.0, 1.0, 0.0));
		pan23 = glm::scale(pan23, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan23);

		glm::mat4 pan24 = glm::mat4(1.0);
		pan24 = glm::translate(pan24, glm::vec3(-19.0, -3.5, 2.0));
		pan24 = glm::rotate(pan24, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		pan24 = glm::scale(pan24, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan24);

		glm::mat4 basePan2 = glm::mat4(1.0);
		basePan2 = glm::translate(basePan2, glm::vec3(-19.0, -3.51, 2.0));
		basePan2 = glm::scale(basePan2, glm::vec3(0.44, 0.1, 0.44));
		sphere3.render(basePan2);

		glm::mat4 pan3 = glm::mat4(1.0);
		pan3 = glm::translate(pan3, glm::vec3(-18.0, -3.5, 2.0));
		pan3 = glm::scale(pan3, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan3);

		glm::mat4 pan31 = glm::mat4(1.0);
		pan31 = glm::translate(pan31, glm::vec3(-18.0, -3.5, 2.0));
		pan31 = glm::rotate(pan31, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
		pan31 = glm::scale(pan31, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan11);

		glm::mat4 pan32 = glm::mat4(1.0);
		pan32 = glm::translate(pan32, glm::vec3(-18.0, -3.5, 2.0));
		pan32 = glm::rotate(pan32, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pan32 = glm::scale(pan32, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan12);

		glm::mat4 pan33 = glm::mat4(1.0);
		pan33 = glm::translate(pan33, glm::vec3(-18.0, -3.5, 2.0));
		pan33 = glm::rotate(pan33, glm::radians(135.0f), glm::vec3(0.0, 1.0, 0.0));
		pan33 = glm::scale(pan33, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan33);

		glm::mat4 pan34 = glm::mat4(1.0);
		pan34 = glm::translate(pan34, glm::vec3(-18.0, -3.5, 2.0));
		pan34 = glm::rotate(pan34, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		pan34 = glm::scale(pan34, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan34);

		glm::mat4 basePan3 = glm::mat4(1.0);
		basePan3 = glm::translate(basePan3, glm::vec3(-18.0, -3.51, 2.0));
		basePan3 = glm::scale(basePan3, glm::vec3(0.44, 0.1, 0.44));
		sphere3.render(basePan3);

		glm::mat4 pan4 = glm::mat4(1.0);
		pan4 = glm::translate(pan4, glm::vec3(-17.5, -3.5, 2.0));
		pan4 = glm::scale(pan4, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan4);

		glm::mat4 pan41 = glm::mat4(1.0);
		pan41 = glm::translate(pan41, glm::vec3(-17.5, -3.5, 2.0));
		pan41 = glm::rotate(pan41, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
		pan41 = glm::scale(pan41, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan11);

		glm::mat4 pan42 = glm::mat4(1.0);
		pan42 = glm::translate(pan42, glm::vec3(-17.5, -3.5, 2.0));
		pan42 = glm::rotate(pan42, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		pan42 = glm::scale(pan42, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan42);

		glm::mat4 pan43 = glm::mat4(1.0);
		pan43 = glm::translate(pan43, glm::vec3(-17.5, -3.5, 2.0));
		pan43 = glm::rotate(pan43, glm::radians(135.0f), glm::vec3(0.0, 1.0, 0.0));
		pan43 = glm::scale(pan43, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan43);

		glm::mat4 pan44 = glm::mat4(1.0);
		pan44 = glm::translate(pan44, glm::vec3(-17.5, -3.5, 2.0));
		pan44 = glm::rotate(pan44, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		pan44 = glm::scale(pan44, glm::vec3(0.5, 0.1, 0.1));
		sphere3.render(pan44);

		glm::mat4 basePan4 = glm::mat4(1.0);
		basePan4 = glm::translate(basePan4, glm::vec3(-17.5, -3.51, 2.0));
		basePan4 = glm::scale(basePan4, glm::vec3(0.44, 0.1, 0.44));
		sphere3.render(basePan4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Velas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 vela1 = glm::mat4(1.0);
		vela1 = glm::translate(vela1, glm::vec3(-19.5, -3.2, 1.5));
		vela1 = glm::scale(vela1, glm::vec3(0.1, 0.5, 0.1));
		glBindTexture(GL_TEXTURE_2D, textureID41);
		cylinder2.render(vela1);

		glm::mat4 vela2 = glm::mat4(1.0);
		vela2 = glm::translate(vela2, glm::vec3(-19.0, -3.2, 1.5));
		vela2 = glm::scale(vela2, glm::vec3(0.1, 0.5, 0.1));
		cylinder2.render(vela2);

		glm::mat4 vela3 = glm::mat4(1.0);
		vela3 = glm::translate(vela3, glm::vec3(-18.0, -3.2, 1.5));
		vela3 = glm::scale(vela3, glm::vec3(0.1, 0.5, 0.1));
		cylinder2.render(vela3);

		glm::mat4 vela4 = glm::mat4(1.0);
		vela4 = glm::translate(vela4, glm::vec3(-17.5, -3.2, 1.5));
		vela4 = glm::scale(vela4, glm::vec3(0.1, 0.5, 0.1));
		cylinder2.render(vela4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//----------------------------------------------Modelos----------------------------------------------------
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Camas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 matrixModelBed = glm::mat4(1.0);
		matrixModelBed = glm::translate(matrixModelBed, glm::vec3(-13.2, 0.0, 5.0));
		matrixModelBed = glm::rotate(matrixModelBed, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBed = glm::scale(matrixModelBed, glm::vec3(0.2, 0.2, 0.2));
		modelBed.render(matrixModelBed);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelBed2 = glm::mat4(1.0);
		matrixModelBed2 = glm::translate(matrixModelBed2, glm::vec3(-14.9, 0.0, -5.0));
		matrixModelBed2 = glm::rotate(matrixModelBed2, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelBed2 = glm::scale(matrixModelBed2, glm::vec3(0.02, 0.02, 0.02));
		modelBed2.render(matrixModelBed2);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToilet = glm::mat4(1.0);
		matrixModelToilet = glm::translate(matrixModelToilet, glm::vec3(-13.0, -5.0, -0.9));
		matrixModelToilet = glm::rotate(matrixModelToilet, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelToilet = glm::scale(matrixModelToilet, glm::vec3(0.002, 0.002, 0.002));
		modelBed4.render(matrixModelToilet);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair = glm::mat4(1.0);
		matrixModelChair = glm::translate(matrixModelChair, glm::vec3(-13.0, -5.0, -6.0));
		matrixModelChair = glm::scale(matrixModelChair, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelSofa = glm::mat4(1.0);
		matrixModelSofa = glm::translate(matrixModelSofa, glm::vec3(-20.5, -5.0, -2.0));
		matrixModelSofa = glm::rotate(matrixModelSofa, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		matrixModelSofa = glm::scale(matrixModelSofa, glm::vec3(0.0022, 0.0022, 0.0022));
		modelBed6.render(matrixModelSofa);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable = glm::mat4(1.0);
		matrixModelTable = glm::translate(matrixModelTable, glm::vec3(13.0, -5.0, -4.0));
		matrixModelTable = glm::rotate(matrixModelTable, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//matrixModelTable = glm::rotate(matrixModelTable, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTable = glm::scale(matrixModelTable, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable);
		//Forzar a habilitar a la unidad de textura 0-----------------IMPORTANTE
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTV = glm::mat4(1.0);
		matrixModelTV = glm::translate(matrixModelTV, glm::vec3(-18.5, -3.0, -8.7));
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTV = glm::scale(matrixModelTV, glm::vec3(0.3, 0.3, 0.3));
		modelTV.render(matrixModelTV);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable2 = glm::mat4(1.0);
		matrixModelTable2 = glm::translate(matrixModelTable2, glm::vec3(-18.5, -5.0, 2.0));
		matrixModelTable2 = glm::rotate(matrixModelTable2, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTable2 = glm::scale(matrixModelTable2, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable2);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelToilet2 = glm::mat4(1.0);
		matrixModelToilet2 = glm::translate(matrixModelToilet2, glm::vec3(-29.5, 0.0, 8.0));
		matrixModelToilet2 = glm::rotate(matrixModelToilet2, glm::radians(180.0f), glm::vec3(0.0, -1.0, 0.0));
		matrixModelToilet2 = glm::scale(matrixModelToilet2, glm::vec3(0.002, 0.002, 0.002));
		modelBed4.render(matrixModelToilet2);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTable3 = glm::mat4(1.0);
		matrixModelTable3 = glm::translate(matrixModelTable3, glm::vec3(-27.5, -5.0, 4.5));
		matrixModelTable3 = glm::rotate(matrixModelTable3, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//matrixModelTable3 = glm::rotate(matrixModelTable3, glm::radians(9.0f), glm::vec3(0.0, 0.0, 1.0));
		matrixModelTable3 = glm::scale(matrixModelTable3, glm::vec3(0.003, 0.003, 0.003));
		modelBed7.render(matrixModelTable3);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair3 = glm::mat4(1.0);
		matrixModelChair3 = glm::translate(matrixModelChair3, glm::vec3(-27.5, -5.0, 2.0));
		matrixModelChair3 = glm::scale(matrixModelChair3, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair3);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair4 = glm::mat4(1.0);
		matrixModelChair4 = glm::translate(matrixModelChair4, glm::vec3(-27.5, -5.0, 7.0));
		matrixModelChair4 = glm::rotate(matrixModelChair4,glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		matrixModelChair4 = glm::scale(matrixModelChair4, glm::vec3(0.25, 0.25, 0.25));
		modelBed5.render(matrixModelChair4);
		glActiveTexture(GL_TEXTURE0);

//---------------------------------------------------------------------------------------------------------------------


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

		modelPickup.render(matrixModelPickup);
		glActiveTexture(GL_TEXTURE0);

				//Maquina de estados mover la Pickup
		switch (statePickup)
		{
		case 0:
			//std::cout << "Advance Pickup:" << offsetPickupAdvance << std::endl;
			matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(0.0, 0.0, 0.3));
			offsetPickupAdvance += 0.1;
			if (offsetPickupAdvance > 65.0) {
				offsetPickupAdvance = 0.0;
				statePickup = 1;
			}
			break;
		case 1:
			//std::cout << "Turn Pickup" << offsetPickupRot << std::endl;
			matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(0.0, 0.0, 0.3));
			matrixModelPickup = glm::rotate(matrixModelPickup, glm::radians(0.5f), glm::vec3(0, 1, 0));
			offsetPickupRot += 0.5;
			if (offsetPickupRot > 90) {
				offsetPickupRot = 0.0;
				statePickup = 0;
			}
		default:
			break;
		}

				//Maquina para para prender y apagar las luces
		switch (onoff)
		{
		case 0:
			//std::cout << "Advance:" << std::endl;
			contador += 0.1;
			if (contador > 1.0) {
				onoff = 1;
				lightSpecular = 0.6;
				color = 1.0;
				contador = 0.0;
			}
			break;
		case 1:
			//std::cout << "Turn" << std::endl;
			contador += 0.1;
			if (contador > 1.0) {
				onoff = 0;
				lightSpecular = 0.006;
				color = 0.0;
				contador = 0.0;
			}
		default:
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
