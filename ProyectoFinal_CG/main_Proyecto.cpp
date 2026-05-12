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
#include "modelosGrandes.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 10.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);

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

//Arrays vertex para el cubo, la luz y la cara del cubo que se ve desde el frente
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

float verticesLuz[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

float verticesCajaVolad[] = {
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
	1.0000f, 1.0000f, -1.0000f,   1.0000f, -0.0000f, -0.0000f,   1.0000f, 1.0000f,
	1.0000f, 1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.0000f, 1.0000f,
	1.0000f, -1.0000f, 1.0000f,   1.0000f, -0.0000f, -0.0000f,   0.0000f, 0.0000f,
	-1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   1.0000f, 1.0000f,
	1.0000f, 1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.0000f, 1.0000f,
	1.0000f, -1.0000f, -1.0000f,   -0.0000f, -0.0000f, -1.0000f,   0.0000f, 0.0000f,
};

glm::vec3 Light1 = glm::vec3(0);


/////////////////////////////////Animations

//Anim Moneda
bool AnimMoneda = false;
float rotMoneda = 0.0f;

// Animacion Plataforma
bool AnimPlataforma = false;
float platPos = 18.728f; // Limite inferior
bool platSube = true; 

// Animacion Bandera (Viento)
bool AnimBandera = false;
float tiempoViento = 0.0f;

// Animacion Koopa Shell
bool AnimShell = false;
float shellX = 9.72329f; // Posicion inicial
float shellRot = 0.0f;   // Angulo de rotacion
bool shellAvanza = true; // Controla si va hacia la izquierda o derecha

// Animacion Cajita Voladora (Maquina de Estados)
bool AnimCaja = false;
float cajaPosX = 10.048f, cajaPosY = 10.8093f, cajaPosZ = 6.78313f; // Posicion 1
float cajaRotY = 0.0f;   // Controla hacia donde voltea
float cajaInclin = 0.0f;  // Controla la inclinacion hacia adelante
float heliceRot = 0.0f;  // Rotacion de las helices
float tiempoVuelo = 0.0f;
int estadoCaja = 0;      // Define que lado del cuadro esta recorriendo

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions TAMAÑO DE VENTANA DE ACUERDO A PANTALLA
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Pruebas Finales", primaryMonitor, nullptr);

	/*GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height - 60, "Pruebas Finales", nullptr, nullptr);*/

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

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	////////////////////////////////////// Load models
	Model Mario((char*)"Models/Mario.obj");
	Model Escenario((char*)"Models/escMod.obj");
	Model shell((char*)"Models/shell.obj");
	Model cuboRomp((char*)"Models/bloqLad.obj");
	Model cuboRompConj((char*)"Models/conjLad.obj");
	Model cuboDec((char*)"Models/cubDec.obj");
	Model tree((char*)"Models/treeobj.obj");
	Model goal((char*)"Models/banderaGoalobj.obj");
	Model piedra((char*)"Models/piedrita.obj");
	Model puente((char*)"Models/puente.obj");


	//// CUBO NORMAL FUENTES DE LUZ
	// First, set the container's VAO (and VBO)
	GLuint VBO_Luz, VAO_Luz;
	glGenVertexArrays(1, &VAO_Luz);
	glGenBuffers(1, &VBO_Luz);
	glBindVertexArray(VAO_Luz);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Luz);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLuz), verticesLuz, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);
	
	
	//// CUBO NORMAL
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


	//// CUBO CARA DE FRENTE
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


	//// CUBO SIN CARA INFERIOR
	GLuint VBO_SinInferior, VAO_SinInferior;
	glGenVertexArrays(1, &VAO_SinInferior);
	glGenBuffers(1, &VBO_SinInferior);
	glBindVertexArray(VAO_SinInferior);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_SinInferior);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCajaVolad), verticesCajaVolad, GL_STATIC_DRAW);

	// Position attribute (X, Y, Z) - Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (Nx, Ny, Nz) - Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute (U, V) - Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	//TUBERIA--------------------------------------------------------------------------
	GLuint VBO_Tuberia, VAO_Tuberia;
	glGenVertexArrays(1, &VAO_Tuberia);
	glGenBuffers(1, &VBO_Tuberia);
	glBindVertexArray(VAO_Tuberia);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tuberia);
	glBufferData(GL_ARRAY_BUFFER, sizeTuberia, verticesTuberia, GL_STATIC_DRAW);

	// Position attribute (X, Y, Z) - Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (Nx, Ny, Nz) - Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute (U, V) - Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	
	//Moneda--------------------------------------------------------------------------
	
	GLuint VBO_Moneda, VAO_Moneda;
	glGenVertexArrays(1, &VAO_Moneda);
	glGenBuffers(1, &VBO_Moneda);
	glBindVertexArray(VAO_Moneda);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Moneda);
	glBufferData(GL_ARRAY_BUFFER, sizeMoneda, verticesMoneda, GL_STATIC_DRAW);

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


	///////////CARGA E INICIALIZACION DE TEXTURAS
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


	GLuint textureNaranja;
	glGenTextures(1, &textureNaranja);
	glBindTexture(GL_TEXTURE_2D, textureNaranja);
	int textureWidth4, textureHeight4, nrChannels4;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* naranja;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	naranja = stbi_load("images/textCajitaVolad.png", &textureWidth4, &textureHeight4, &nrChannels4, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth4, textureHeight4, 0, GL_RGBA, GL_UNSIGNED_BYTE, naranja);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (naranja)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth4, textureHeight4, 0, GL_RGBA, GL_UNSIGNED_BYTE, naranja);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(naranja);

	GLuint texturePlat;
	glGenTextures(1, &texturePlat);
	glBindTexture(GL_TEXTURE_2D, texturePlat);
	int textureWidthPlat5, textureHeightPlat5, nrChannelsPlat5;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* platExt;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	platExt = stbi_load("images/platText.png", &textureWidthPlat5, &textureHeightPlat5, &nrChannelsPlat5, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlat5, textureHeightPlat5, 0, GL_RGB, GL_UNSIGNED_BYTE, platExt);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (platExt)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlat5, textureHeightPlat5, 0, GL_RGB, GL_UNSIGNED_BYTE, platExt);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(platExt);


	GLuint texturePlatInt;
	glGenTextures(1, &texturePlatInt);
	glBindTexture(GL_TEXTURE_2D, texturePlatInt);
	int textureWidthPlatInt6, textureHeightPlatInt6, nrChannelsPlatInt6;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* platInt;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	platInt = stbi_load("images/platDemasText.png", &textureWidthPlatInt6, &textureHeightPlatInt6, &nrChannelsPlatInt6, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlatInt6, textureHeightPlatInt6, 0, GL_RGB, GL_UNSIGNED_BYTE, platInt);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (platInt)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlatInt6, textureHeightPlatInt6, 0, GL_RGB, GL_UNSIGNED_BYTE, platInt);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(platInt);

	GLuint texturePlatAdor;
	glGenTextures(1, &texturePlatAdor);
	glBindTexture(GL_TEXTURE_2D, texturePlatAdor);
	int textureWidthPlatAdor7, textureHeightPlatAdor7, nrChannelsPlatAdor7;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* platAd;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	platAd = stbi_load("images/platAdorn.png", &textureWidthPlatAdor7, &textureHeightPlatAdor7, &nrChannelsPlatAdor7, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlatAdor7, textureHeightPlatAdor7, 0, GL_RGB, GL_UNSIGNED_BYTE, platAd);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (platAd)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthPlatAdor7, textureHeightPlatAdor7, 0, GL_RGB, GL_UNSIGNED_BYTE, platAd);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(platAd);

	
	GLuint textureCubeUsed;
	glGenTextures(1, &textureCubeUsed);
	glBindTexture(GL_TEXTURE_2D, textureCubeUsed);
	int textureWidthCubeUsed7, textureHeightCubeUsed7, nrChannelsCubeUsed7;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* cubeUsed;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	cubeUsed = stbi_load("images/textBlockUsed.png", &textureWidthCubeUsed7, &textureHeightCubeUsed7, &nrChannelsCubeUsed7, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidthCubeUsed7, textureHeightCubeUsed7, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeUsed);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (cubeUsed)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidthCubeUsed7, textureHeightCubeUsed7, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeUsed);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(cubeUsed);



	GLuint textureBaseInfPole;
	glGenTextures(1, &textureBaseInfPole);
	glBindTexture(GL_TEXTURE_2D, textureBaseInfPole);
	int textureWidthBaseInfPole, textureHeightBaseInfPole, nrChannelsBaseInfPole;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* baseInfPole;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	baseInfPole = stbi_load("images/textBaseSup.png", &textureWidthBaseInfPole, &textureHeightBaseInfPole, &nrChannelsBaseInfPole, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthBaseInfPole, textureHeightBaseInfPole, 0, GL_RGB, GL_UNSIGNED_BYTE, baseInfPole);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (baseInfPole)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthBaseInfPole, textureHeightBaseInfPole, 0, GL_RGB, GL_UNSIGNED_BYTE, baseInfPole);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(baseInfPole);

	GLuint textureBaseIntPole;
	glGenTextures(1, &textureBaseIntPole);
	glBindTexture(GL_TEXTURE_2D, textureBaseIntPole);
	int textureWidthBaseIntPole, textureHeightBaseIntPole, nrChannelsBaseIntPole;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* baseIntPole;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	baseIntPole = stbi_load("images/textBaseInt.png", &textureWidthBaseIntPole, &textureHeightBaseIntPole, &nrChannelsBaseIntPole, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthBaseIntPole, textureHeightBaseIntPole, 0, GL_RGB, GL_UNSIGNED_BYTE, baseIntPole);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (baseIntPole)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthBaseIntPole, textureHeightBaseIntPole, 0, GL_RGB, GL_UNSIGNED_BYTE, baseIntPole);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(baseIntPole);

	GLuint textureMangoPole;
	glGenTextures(1, &textureMangoPole);
	glBindTexture(GL_TEXTURE_2D, textureMangoPole);
	int textureWidthMangoPole, textureHeightMangoPole, nrChannelsMangoPole;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* mangoPole;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	mangoPole = stbi_load("images/textMango.png", &textureWidthMangoPole, &textureHeightMangoPole, &nrChannelsMangoPole, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthMangoPole, textureHeightMangoPole, 0, GL_RGB, GL_UNSIGNED_BYTE, mangoPole);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (mangoPole)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthMangoPole, textureHeightMangoPole, 0, GL_RGB, GL_UNSIGNED_BYTE, mangoPole);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(mangoPole);

	GLuint textureMoneda;
	glGenTextures(1, &textureMoneda);
	glBindTexture(GL_TEXTURE_2D, textureMoneda);
	int textureWidthMon, textureHeightMon, nrChannelsMon;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageMoneda;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	imageMoneda = stbi_load("images/mon.png", &textureWidthMon, &textureHeightMon, &nrChannelsMon, 0);
	glBindTexture(GL_TEXTURE_2D, textureMoneda);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthMon, textureHeightMon, 0, GL_RGB, GL_UNSIGNED_BYTE, imageMoneda);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (imageMoneda)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthMon, textureHeightMon, 0, GL_RGB, GL_UNSIGNED_BYTE, imageMoneda);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageMoneda);


	//Carga de textura para tuberia
	GLuint textureTuber;
	glGenTextures(1, &textureTuber);
	glBindTexture(GL_TEXTURE_2D, textureTuber);
	int textureWidthTube, textureHeightTube, nrChannelsTube;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageTuberia;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	imageTuberia = stbi_load("images/tuberiaT.png", &textureWidthTube, &textureHeightTube, &nrChannelsTube, 0);
	glBindTexture(GL_TEXTURE_2D, textureTuber);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthTube, textureHeightTube, 0, GL_RGB, GL_UNSIGNED_BYTE, imageTuberia);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (imageTuberia)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidthTube, textureHeightTube, 0, GL_RGB, GL_UNSIGNED_BYTE, imageTuberia);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageTuberia);


//////////////////////////////////////////////////////////////////////////////////

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
		Animation();

		// Clear the colorbuffer
		glClearColor(0.08f, 0.31f, 0.51f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -71.9288, 2.21836, 3.17808);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);


		//// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 0.802f, 0.088f); // Luz base
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.0f, 0.802f, 0.088f); // Luz principal brillante
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f); // Brillos


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, 0.5f, 0.5f); //canal de colores, primeros dos hace verdoso
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

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


		//////////CARGA DE MODELOS

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Escenario.Draw(lightingShader);

		//CARGA DE SHELL DE KOOPA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(shellX, 1.5481f, 20.6984f));
		model = glm::rotate(model, glm::radians(shellRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		shell.Draw(lightingShader);

		//CARGA DE CUBO LADRILLO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuboRomp.Draw(lightingShader);
		
		//CARGA CONJUNTO CUBO LADRILLO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuboRompConj.Draw(lightingShader);

		//CARGA CUBOS DE DECORACION
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuboDec.Draw(lightingShader);

		//CARGA ARBOLES DE DECORACION
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		//CARGA GOAL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		goal.Draw(lightingShader);

		//CARGA GOAL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piedra.Draw(lightingShader);

		//CARGA PUENTE	
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puente.Draw(lightingShader);

		//-----------------------------------------------------------------------------------DIBUJADO DE CAJITA VOLADORA
		//Base CUBO sin cara
		//MODELADO BASE CUBO SIN CARA INFERIOR PADRE DE LA JERARQUIA DE LA CAJITA VOLADORA
		
		// PADRE ABSOLUTO CUBO SIN CARA INFERIOR
		glm::mat4 modelCaja = glm::mat4(1.0f);
		modelCaja = glm::translate(modelCaja, glm::vec3(cajaPosX, cajaPosY, cajaPosZ));
		modelCaja = glm::rotate(modelCaja, glm::radians(cajaRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCaja = glm::rotate(modelCaja, glm::radians(cajaInclin), glm::vec3(1.0f, 0.0f, 0.0f));
		model = modelCaja;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureNaranja);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO_SinInferior);
		glDrawArrays(GL_TRIANGLES, 0, 30);
		glBindVertexArray(0);

		// MODELADO OJITOS DE LA CAJITA VOLADORA DER 
		model = modelCaja;
		model = glm::translate(model, glm::vec3(0.367371f, 0.000376f, 1.06267f));
		model = glm::scale(model, glm::vec3(0.111f, 0.383f, 0.070f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO OJITOS DE LA CAJITA VOLADORA IZQ 
		model = modelCaja;
		model = glm::translate(model, glm::vec3(-0.367371f, 0.000376f, 1.06267f));
		model = glm::scale(model, glm::vec3(0.111f, 0.383f, 0.070f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO PARTE BASE DE LA HELICE INFERIOR 
		model = modelCaja;
		model = glm::translate(model, glm::vec3(0.0f, 1.072636f, 0.0f));
		model = glm::scale(model, glm::vec3(0.262f, 0.072f, 0.251f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureNaranja);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO PARTE BASE DE LA HELICE SUPERIOR 
		glm::mat4 modelBaseSup = modelCaja; 
		modelBaseSup = glm::translate(modelBaseSup, glm::vec3(0.0f, 1.422516f, 0.0f));

		model = modelBaseSup;
		model = glm::scale(model, glm::vec3(0.262f, 0.279f, 0.251f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO PARTE HELICE (Hijo de Base Superior, y padre de los extras)
		glm::mat4 modelHelice = modelBaseSup; 
		modelHelice = glm::translate(modelHelice, glm::vec3(0.0f, -0.007f, 0.0f));
		modelHelice = glm::rotate(modelHelice, glm::radians(heliceRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = modelHelice;
		model = glm::scale(model, glm::vec3(1.428f, 0.057f, 0.251f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO PARTE EXTRA HELICE IZQUIERDA (Hijo de la helice principal)
		model = modelHelice;
		model = glm::translate(model, glm::vec3(0.839388f, 0.00169f, 0.0f));
		model = glm::rotate(model, 1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.461f, 0.057f, 0.251f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// MODELADO PARTE EXTRA HELICE DERECHA (Hijo de la helice principal)
		model = modelHelice;
		model = glm::translate(model, glm::vec3(-0.839388f, 0.00169f, 0.0f));
		model = glm::rotate(model, 1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.461f, 0.057f, 0.251f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		//-----------------------------------------------------------------------------------DIBUJADO DE PLATAFORMA
		//Base 
		// PADRE ABSOLUTO: BASE SUPERIOR
		glm::mat4 modelPlataforma = glm::mat4(1.0f);
		modelPlataforma = glm::translate(modelPlataforma, glm::vec3(-3.01135f, platPos, 36.63f));
		modelPlataforma = glm::scale(modelPlataforma, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::scale(model, glm::vec3(4.20231f, 0.196867f, 4.20231f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePlat);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);

		// Dibujamos la Base Superior (Padre)
		model = modelPlataforma;
		model = glm::scale(model, glm::vec3(4.202f, 0.197f, 4.202f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INFERIOR 
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(0.0f, -0.79588f, 0.0f));
		model = glm::scale(model, glm::vec3(4.200f, 0.187f, 4.200f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INTERIOR
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(0.0f, -0.39794f, 0.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePlatAdor);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		
		model = glm::scale(model, glm::vec3(3.544f, 0.237f, 3.544f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePlatInt);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);

		// MODELADO BASE ESQUINAS INFERIOR 

		// INFERIOR IZQUIERDA
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-3.85684f, -0.40549f, 3.87845f));
		model = glm::scale(model, glm::vec3(0.34944f, 0.209f, 0.320f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// INFERIOR DERECHA
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-3.85684f, -0.40549f, -3.87845f));
		model = glm::scale(model, glm::vec3(0.34944f, 0.209f, 0.320f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// SUPERIOR DERECHA
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(3.85684f, -0.40549f, -3.87845f));
		model = glm::scale(model, glm::vec3(0.34944f, 0.209f, 0.320f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// SUPERIOR IZQUIERDA
		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(3.85684f, -0.40549f, 3.87845f));
		model = glm::scale(model, glm::vec3(0.349f, 0.209f, 0.320f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ADORNOS INTERIORES INFERIOR 

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-3.74468f, -0.39152f, 0.0f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-3.74468f, -0.39152f, 1.93662f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-3.74468f, -0.39152f, -1.93662f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ADORNOS INTERIORES INFERIOR IZQUIERDA 

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(0.0f, -0.39152f, 3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(1.93662f, -0.39152f, 3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-1.93662f, -0.39152f, 3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ADORNOS INTERIORES INFERIOR DERECHA 

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(0.0f, -0.39152f, -3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(-1.93662f, -0.39152f, -3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(1.93662f, -0.39152f, -3.74468f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ADORNOS INTERIORES SUPERIOR 

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(3.74468f, -0.39152f, 0.0f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(3.74468f, -0.39152f, -1.93662f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = modelPlataforma;
		model = glm::translate(model, glm::vec3(3.74468f, -0.39152f, 1.93662f));
		model = glm::scale(model, glm::vec3(0.220f, 0.220f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		//-----------------------------------------------------------------------------------DIBUJADO DE CHECPOINT FLAG

		//DEFINICION DEL PADRE (EL MANGO)
		glm::mat4 modelMango = glm::mat4(1.0f);
		modelMango = glm::translate(modelMango, glm::vec3(0.0f, 2.91483f, 13.998f));
		modelMango = glm::rotate(modelMango, 3.141598f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMango = glm::scale(modelMango, glm::vec3(0.8030f, 0.8030f, 0.8030f));

		model = modelMango;
		model = glm::scale(model, glm::vec3(0.198f, -1.407f, 0.199f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//CARGA DE TEXTURA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// DIBUJO DE LOS HIJOS 

		//Adorno bandera
		model = modelMango; 
		model = glm::translate(model, glm::vec3(0.0f, 1.53924f, 0.0f));
		model = glm::scale(model, glm::vec3(0.309f, 0.309f, 0.309f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Bandera como tal
		model = modelMango;
		model = glm::translate(model, glm::vec3(1.19395f, 0.34149f, 0.0f));
		model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 1.0f, 0.0f)); 
		
		//Parte animacion de ondeo de bandera (manipulacion de 3 ejes)
		// Solo se mueven si tiempoViento avanza. Uso de sin() y cos() para crear turbulencia
		float fuerzaX = sin(tiempoViento * 1.5f) * 8.0f;  // Aleteo vertical
		float fuerzaY = sin(tiempoViento * 2.0f) * 15.0f; // Tirones hacia los lados (mas fuerte)
		float fuerzaZ = cos(tiempoViento * 1.1f) * 5.0f;  // Ligera torsion

		model = glm::rotate(model, glm::radians(fuerzaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(fuerzaY), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(fuerzaZ), glm::vec3(0.0f, 1.0f, 0.0f));


		model = glm::scale(model, glm::vec3(0.010f, 0.847f, -1.007));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureFrente);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO_Frente);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Tercera Plataforma (Base superior) 
		model = modelMango;
		model = glm::translate(model, glm::vec3(0.0f, -1.521971f, 0.0f));
		model = glm::scale(model, glm::vec3(0.257f, -0.122f, 0.256f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Segunda Plataforma (Base intermedia)

		model = modelMango;
		model = glm::translate(model, glm::vec3(0.0f, -1.730216f, 0.0f));
		model = glm::scale(model, glm::vec3(0.716f, -0.086f, 0.698f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Base de bandera (Base inferior) 
		model = modelMango;
		model = glm::translate(model, glm::vec3(0.0f, -2.060867f, 0.0f));
		model = glm::scale(model, glm::vec3(0.845f, -0.244f, 0.840f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		
		//-----------------------------------------------------------------------------------DIBUJADO DE POLE
		//Base CUBO 

		// PADRE ABSOLUTO: MANGO DEL POLE
		glm::mat4 modelPole = glm::mat4(1.0f);
		modelPole = glm::translate(modelPole, glm::vec3(-6.2866f, 11.8751f, 35.9193f));
		modelPole = glm::scale(modelPole, glm::vec3(0.8025f, 0.8025f, 0.8025f));
		glBindVertexArray(VAO);
		
		// Dibujamos el Mango (Padre) 
		model = modelPole;
		model = glm::scale(model, glm::vec3(0.260f, 3.500f, 0.260f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMangoPole);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE CUBO USED SUPER MARIO 
		model = modelPole;
		model = glm::translate(model, glm::vec3(0.0f, -5.222761f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCubeUsed);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INFERIOR DEL POLE 
		model = modelPole;
		model = glm::translate(model, glm::vec3(0.0f, -4.01449f, 0.0f));
		model = glm::scale(model, glm::vec3(0.654f, 0.219f, 0.654f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBaseInfPole);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INTERIOR DEL POLE 
		model = modelPole;
		model = glm::translate(model, glm::vec3(0.0f, -3.65088f, 0.0f));
		model = glm::scale(model, glm::vec3(0.412f, 0.138f, 0.412f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBaseIntPole);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INTERIOR SUPERIOR DEL POLE 
		model = modelPole;
		model = glm::translate(model, glm::vec3(0.0f, 3.55519f, 0.0f));
		model = glm::scale(model, glm::vec3(0.412f, 0.138f, 0.412f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBaseIntPole);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// MODELADO BASE INFERIOR SUPERIOR DEL POLE 
		model = modelPole;
		model = glm::translate(model, glm::vec3(0.0f, 3.92333f, 0.0f));
		model = glm::scale(model, glm::vec3(0.654f, 0.219f, 0.654f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBaseInfPole);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//-----------------------------------------------------------------------------------DIBUJADO DE CUBO DE INTERROGACION
		//Base CUBO 

		//MODELADO BASE CUBO USED SUPER MARIO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.43391f, 11.213f, 31.1363f));
		model = glm::scale(model, glm::vec3(0.7688621f, 0.768862f, 0.768862f));
		model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.39813f, 6.36189f, 31.1363f));
		model = glm::scale(model, glm::vec3(0.7688621f, 0.768862f, 0.768862f));
		model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); // Asegura que el shader lea la textura 0

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//-----------------------------------------------------------------------------------DIBUJADO DE TUBERIA

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.09684f, 3.69183f));
		//model = glm::scale(model, glm::vec3(1.67002f, 1.67002f, 1.67002f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureTuber);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); 

		glBindVertexArray(VAO_Tuberia);
		glDrawArrays(GL_TRIANGLES, 0, 2292);
		glBindVertexArray(0);


		//-----------------------------------------------------------------------------------DIBUJADO DE Monedas

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.3432f, 14.4035f, 33.0905f));
		model = glm::scale(model, glm::vec3(0.6364f, 0.6364f, 0.6364f));
		model = glm::rotate(model, glm::radians(rotMoneda), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMoneda);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0); 

		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.8);*/

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.0f, 0.0f, 0.0f); //canal de colores, primeros dos hace verdoso
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material_shininess"), 0.0f);

		glBindVertexArray(VAO_Moneda);
		glDrawArrays(GL_TRIANGLES, 0, 3924);
		glBindVertexArray(0);
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.85912f, 14.4035f, 33.0905f));
		model = glm::scale(model, glm::vec3(0.6364f, 0.6364f, 0.6364f));
		model = glm::rotate(model, glm::radians(rotMoneda), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMoneda);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);

		/*glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.8);*/

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.0f, 0.0f, 0.0f); //canal de colores, primeros dos hace verdoso
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material_shininess"), 0.0f);

		glBindVertexArray(VAO_Moneda);
		glDrawArrays(GL_TRIANGLES, 0, 3924);
		glBindVertexArray(0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.40302f, 14.4035f, 33.0905f));
		model = glm::scale(model, glm::vec3(0.6364f, 0.6364f, 0.6364f));
		model = glm::rotate(model, glm::radians(rotMoneda), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMoneda);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);

		/*glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.8);*/

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.0f, 0.0f, 0.0f); //canal de colores, primeros dos hace verdoso
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material_shininess"), 0.0f);

		glBindVertexArray(VAO_Moneda);
		glDrawArrays(GL_TRIANGLES, 0, 3924);
		glBindVertexArray(0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.2644f, 1.78688f, 20.7837f));
		model = glm::scale(model, glm::vec3(0.768862f, 0.768862f, 0.768862f));
		//model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCubeUsed);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.26968f, 1.78688f, 20.7837f));
		model = glm::scale(model, glm::vec3(0.768862f, 0.768862f, 0.768862f));
		//model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCubeUsed);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		////////////////////////////////////////////////////////////////////////////////////////// PARTE DE FUENTES DE LUZ

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

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

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
	float moreSpeedCamera = 2.0f;

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime * moreSpeedCamera);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime * moreSpeedCamera);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime * moreSpeedCamera);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime * moreSpeedCamera);


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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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


	if (keys[GLFW_KEY_P])
	{
		AnimPlataforma = !AnimPlataforma; 
		keys[GLFW_KEY_P] = false;         
	}

	if (keys[GLFW_KEY_C])
	{
		AnimMoneda = !AnimMoneda;
		keys[GLFW_KEY_C] = false; 
	}

	if (keys[GLFW_KEY_B])
	{
		AnimBandera = !AnimBandera;
		keys[GLFW_KEY_B] = false;
	}

	if (keys[GLFW_KEY_K])
	{
		AnimShell = !AnimShell;
		keys[GLFW_KEY_K] = false; // Evita el parpadeo de la animacion
	}

	if (keys[GLFW_KEY_V])
	{
		AnimCaja = !AnimCaja;
		keys[GLFW_KEY_V] = false;
	}

}
void Animation() {

	if (AnimMoneda)
	{
		//rotMoneda += 1.2f; // Velocidad de rotacion de la moneda
		rotMoneda += 85.0f * deltaTime;
		if (rotMoneda >= 360.0f) {
			rotMoneda -= 360.0f; // Mantiene el angulo en un rango manejable
		}
	}

	// Animacion de la plataforma 
	if (AnimPlataforma) {
		float velocPlat = 1.8f * deltaTime;

		if (platSube) {
			platPos += velocPlat;
			if (platPos >= 26.7383f) {
				platPos = 26.7383f;
				platSube = false;
			}
		}
		else {
			platPos -= velocPlat;
			if (platPos <= 18.728f) {
				platPos = 18.728f;
				platSube = true;
			}
		}
	}

	if (AnimBandera) {
		// Incremento del tiempo, multiplicacion por deltaTime asegura 
		// que el viento sople igual sin importar los frames del equipo

		tiempoViento += deltaTime * 5.0f; // Velocidad del viento, ajustable
	}

	if (AnimShell) {
		
		//Rotacion constante su propie eje
		shellRot += 900.0f * deltaTime; // Velocidad de rotacion
		if (shellRot >= 360.0f) {
			shellRot -= 360.0f; // Mantiene el angulo en un rango manejable
		}

		// Traslacion de lado a lado
		float velocShell = 6.0f * deltaTime; // Velocidad de movimiento, ajustable

		if (shellAvanza) {
			shellX -= velocShell; // Se dirige hacia -1.68063 (derecha, empieza en 9 en x)
			if (shellX <= -1.68063f) {
				shellX = -1.68063f; // Limite derecho
				shellAvanza = false; // Cambiamos de dirección
			}
		}
		else {
			shellX += velocShell; // Regresa hacia 9 (izquierda)
			if (shellX >= 9.72329f) {
				shellX = 9.72329f; // Limite izquierdo
				shellAvanza = true; // Cambiamos de direccion
			}
		}
	}


	// Animación Cajita Voladora
	if (AnimCaja) {
		
		tiempoVuelo += deltaTime * 2.0f;

		// Las helices giran constantemente sin importar el estado
		
		heliceRot += 1500.0f * deltaTime;

		if (heliceRot >= 360.0f) {
			
			heliceRot -= 360.0f;
		} 

		// Efecto de flotacion constante en el eje y o z para simular vuelo
		cajaPosY = 10.8093f + sin(tiempoVuelo) * 0.4f;

		// Velocidades ajustables
		float velCaja = 4.8f * deltaTime;
		float velRot = 150.0f * deltaTime;

		// Maquina de estados cajita

		switch (estadoCaja) {
		
		case 0: // Avanzar de Posicion 1 a 2 (+Z)
			cajaInclin = 15.0f; // Se inclina para avanzar
			cajaPosZ += velCaja;
			if (cajaPosZ >= 20.916f) {
				cajaPosZ = 20.916f; // Asegura limite
				cajaInclin = 0.0f;    // Se endereza
				estadoCaja = 1;      // Pasa a estado de giro
			}
			break;

		case 1: // Rotar a la derecha (-90 grados en Y)
			cajaRotY -= velRot;
			if (cajaRotY <= -90.0f) {
				cajaRotY = -90.0f;
				estadoCaja = 2; // Pasa a estado de avance
			}
			break;

		case 2: // Avanzar de Posicion 2 a 3 (-X)
			cajaInclin = 15.0f;
			cajaPosX -= velCaja;
			if (cajaPosX <= -13.6973f) {
				cajaPosX = -13.6973f;
				cajaInclin = 0.0f;
				estadoCaja = 3;
			}
			break;

		case 3: // Rotar a la derecha (-180 grados en Y)
			cajaRotY -= velRot;
			if (cajaRotY <= -180.0f) {
				cajaRotY = -180.0f;
				estadoCaja = 4;
			}
			break;

		case 4: // Avanzar de Posicion 3 a 4 (-Z)
			cajaInclin = 15.0f;
			cajaPosZ -= velCaja;
			if (cajaPosZ <= 6.78313f) {
				cajaPosZ = 6.78313f;
				cajaInclin = 0.0f;
				estadoCaja = 5;
			}
			break;

		case 5: // Rotar a la derecha (-270 grados en Y)
			cajaRotY -= velRot;
			if (cajaRotY <= -270.0f) {
				cajaRotY = -270.0f;
				estadoCaja = 6;
			}
			break;

		case 6: // Avanzar de Posicion 4 a 1 (+X)
			cajaInclin = 15.0f;
			cajaPosX += velCaja;
			if (cajaPosX >= 10.048f) {
				cajaPosX = 10.048f;
				cajaInclin = 0.0f;
				estadoCaja = 7;
			}
			break;

		case 7: // Rotar a la derecha (-360 grados) y reiniciar
			cajaRotY -= velRot;
			if (cajaRotY <= -360.0f) {
				cajaRotY = 0.0f; // Reinicia el contador de angulos
				estadoCaja = 0;  // Vuelve al primer estado
			}
			break;
		}
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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