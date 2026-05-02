#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	// Posiciones (X,Y,Z)       // Normales (Nx,Ny,Nz)      // UVs (U,V)
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   1.0000f, 1.0000f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0000f, 0.0000f,
	1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   1.0000f, 0.0000f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   1.0000f, 1.0000f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0000f, 0.0000f,
	1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   1.0000f, 0.0000f,
	-1.0000f, 1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   1.0000f, 1.0000f,
	-1.0000f, -1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.0000f, 0.0000f,
	-1.0000f, -1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   1.0000f, 0.0000f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   1.0000f, 1.0000f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.0000f, 0.0000f,
	-1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   1.0000f, 0.0000f,
	1.0000f, 1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   1.0000f, 1.0000f,
	1.0000f, -1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.0000f, 0.0000f,
	1.0000f, -1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   1.0000f, 0.0000f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   1.0000f, 1.0000f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.0000f, 0.0000f,
	-1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   1.0000f, 0.0000f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   1.0000f, 1.0000f,
	-1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0000f, 1.0000f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0000f, 0.0000f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   1.0000f, 1.0000f,
	-1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0000f, 1.0000f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0000f, 0.0000f,
	-1.0000f, 1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   1.0000f, 1.0000f,
	-1.0000f, 1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.0000f, 1.0000f,
	-1.0000f, -1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.0000f, 0.0000f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   1.0000f, 1.0000f,
	1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.0000f, 1.0000f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.0000f, 0.0000f,
	1.0000f, 1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   1.0000f, 1.0000f,
	1.0000f, 1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.0000f, 1.0000f,
	1.0000f, -1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.0000f, 0.0000f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   1.0000f, 1.0000f,
	1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.0000f, 1.0000f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.0000f, 0.0000f,
};

float verticesCaraFrente[] = {
	// Posiciones (X,Y,Z)       // Normales (Nx,Ny,Nz)      // UVs (U,V)
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.3155f, 0.3155f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0179f, 0.0179f,
	1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.3155f, 0.0179f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0179f, 0.6488f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.3155f, 0.3512f,
	1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.3155f, 0.6488f,
	-1.0000f, 1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.3512f, 0.3155f,
	-1.0000f, -1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.6488f, 0.0179f,
	-1.0000f, -1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.6488f, 0.3155f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.6488f, 0.6488f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.3512f, 0.3512f,
	-1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.6488f, 0.3512f,
	1.0000f, 1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.9795f, 0.6403f,
	1.0000f, -1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.6819f, 0.3427f,
	1.0000f, -1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.9795f, 0.3427f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.3512f, 0.9821f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.6488f, 0.6845f,
	-1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.6488f, 0.9821f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.3155f, 0.3155f,
	-1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0179f, 0.3155f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, 1.0000f, -0.0000f,   0.0179f, 0.0179f,
	1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0179f, 0.6488f,
	-1.0000f, 1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.0179f, 0.3512f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -0.0000f, 1.0000f,   0.3155f, 0.3512f,
	-1.0000f, 1.0000f, 1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.3512f, 0.3155f,
	-1.0000f, 1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.3512f, 0.0179f,
	-1.0000f, -1.0000f, -1.0000f,   -1.0000f, -0.0000f, -0.0000f,   0.6488f, 0.0179f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.6488f, 0.6488f,
	1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.3512f, 0.6488f,
	-1.0000f, -1.0000f, 1.0000f,   -0.0000f, -1.0000f, -0.0000f,   0.3512f, 0.3512f,
	1.0000f, 1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.9795f, 0.6403f,
	1.0000f, 1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.6819f, 0.6403f,
	1.0000f, -1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.6819f, 0.3427f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.3512f, 0.9821f,
	1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.3512f, 0.6845f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.6488f, 0.6845f,
};

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	Model Dog((char*)"Models/Mario.obj");
	Model Piso((char*)"Models/piso.obj");



	// First, set the container's VAO (and VBO)
	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute (X, Y, Z) - Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (Nx, Ny, Nz) - Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute (U, V) - Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	// First, set the container's VAO (and VBO) CARA FRENTE TEXTURAS
	// First, set the container's VAO (and VBO)
	GLuint VBO_Frente, VAO_Frente;
	glGenVertexArrays(1, &VAO_Frente);
	glGenBuffers(1, &VBO_Frente);
	glBindVertexArray(VAO_Frente);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Frente);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCaraFrente), verticesCaraFrente, GL_STATIC_DRAW);

	// Position attribute (X, Y, Z) - Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (Nx, Ny, Nz) - Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute (U, V) - Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	
	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	image = stbi_load("images/texInt.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int textureWidth1, textureHeight1, nrChannels1;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* blanco;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	blanco = stbi_load("images/textBlanca.png", &textureWidth1, &textureHeight1, &nrChannels1, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth1, textureHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, blanco);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (blanco)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth1, textureHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, blanco);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(blanco);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	int textureWidth2, textureHeight2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* amarFlag;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	amarFlag = stbi_load("images/textBaseFlag.png", &textureWidth2, &textureHeight2, &nrChannels2, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, amarFlag);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (amarFlag)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, amarFlag);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(amarFlag);

	GLuint textureFrente;
	glGenTextures(1, &textureFrente);
	glBindTexture(GL_TEXTURE_2D, textureFrente);
	int textureWidthFrente3, textureHeightFrente3, nrChannelsFrente3;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bandera;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	bandera = stbi_load("images/texBLab.png", &textureWidthFrente3, &textureHeightFrente3, &nrChannelsFrente3, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthFrente3, textureHeightFrente3, 0, GL_RGB, GL_UNSIGNED_BYTE, bandera);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (bandera)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthFrente3, textureHeightFrente3, 0, GL_RGB, GL_UNSIGNED_BYTE, bandera);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(bandera);



	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

                  glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.5f,0.5f,0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.0f, 0.0f, 0.0f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		// SpotLight Tipo Lampara depende mucho de la tarjeta grafica,
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	

		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		
	
		// Draw the loaded model MODELOS
		//BANDERA
		//MODELADO BASE CUBO SIN CARA INFERIOR
		//

		////Base de bandera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.620774f, 0.0f));
		//model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//Base de bandera segunda plataforma
		////////////////////////
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.72551f, 0.0f));
		//model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(0.974f, 1.335f, 0.240f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		
		////Tercera Plataforma de la bandera o mango que sostienne el mango
		///////////////////////
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.411419f, 0.0f));
		////model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		//model = glm::scale(model, glm::vec3(0.257f, -0.122f, 0.256f));
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		/////////////////////// MODELO QUE DEBE SER EL PADRE DE TODOS LOS DEMAS, ES EL MANGO

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 1.93339f, 0.0f));
		////model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		//model = glm::scale(model, glm::vec3(0.198f, -1.407f, 0.199f));
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		///////////////////////// Adorno de bandera

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 3.47263f, 0.0f));
		////model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		//model = glm::scale(model, glm::vec3(0.309f, 0.309f, 0.309f));
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);


		/////////////////////// Bandera como tal

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.19395f, 2.27488f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 0.847f, -0.010f));
		//model = glm::rotate(model, 1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		//

		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureFrente);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		//glBindVertexArray(VAO_Frente);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}