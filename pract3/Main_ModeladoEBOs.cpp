#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f; 
float movY=0.0f;
float movZ=-5.0f;
float rotY = 0.0f;
float rotX = 0.0f;
float rotZ = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
    float vertices[] = {
        // X, Y, Z, R, G, B
        0.91f, 0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        0.91f, 1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.91f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.94f, -1.00f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.94f, -0.99f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.91f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.94f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.96f, -0.97f, 0.7f, 0.7f, 0.7f,
        0.94f, 1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.98f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, 1.00f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.99f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.98f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.97f, -0.96f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.91f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.91f, -0.96f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.94f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.99f, 0.94f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.94f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.96f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.96f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.97f, 0.96f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.91f, -1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.91f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, -1.00f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, -1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.99f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.98f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.98f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.97f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.91f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.94f, -1.00f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.94f, -0.99f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.94f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.96f, -0.97f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.94f, -0.91f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.96f, -0.91f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.91f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.99f, -0.94f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.96f, -0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.91f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.94f, -0.96f, 0.7f, 0.7f, 0.7f,
        0.97f, -0.96f, -0.96f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.91f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, 1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.94f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.96f, 0.91f, 0.7f, 0.7f, 0.7f,
        1.00f, 0.91f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.99f, 0.94f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.96f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.91f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.98f, 0.94f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.97f, 0.96f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.91f, 1.00f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, 1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.99f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.98f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.98f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.97f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.91f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.94f, 1.00f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.94f, 0.99f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.94f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.91f, 0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.94f, 0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, 0.96f, 0.97f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.91f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        0.91f, -1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.91f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.91f, 0.96f, 0.7f, 0.7f, 0.7f,
        1.00f, -0.94f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.99f, -0.94f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.94f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.96f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.98f, -0.96f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.97f, -0.96f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.94f, 1.00f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.94f, 0.99f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.91f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.94f, 0.98f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.96f, 0.97f, 0.7f, 0.7f, 0.7f,
        0.94f, -1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.98f, 0.91f, 0.7f, 0.7f, 0.7f,
        0.91f, -1.00f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.99f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.98f, 0.94f, 0.7f, 0.7f, 0.7f,
        0.91f, -0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.94f, -0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        0.96f, -0.97f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.91f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, 1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.91f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.94f, -1.00f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.94f, -0.99f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.94f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.96f, -0.97f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.94f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.96f, -0.91f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.91f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.99f, 0.94f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.96f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.91f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.94f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.97f, 0.96f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.91f, 1.00f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.94f, 1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.99f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.98f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.98f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.97f, -0.96f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.91f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        -0.91f, -1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.91f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.91f, -0.96f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.94f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.99f, -0.94f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.94f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.96f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.96f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.97f, -0.96f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.94f, -1.00f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.91f, -1.00f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.94f, -0.99f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.96f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.91f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.94f, -0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.96f, -0.97f, 0.7f, 0.7f, 0.7f,
        -0.94f, -1.00f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.98f, -0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, -1.00f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.99f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.98f, -0.94f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.98f, -0.96f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.97f, -0.96f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.91f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        -0.91f, 1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.91f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.91f, 0.96f, 0.7f, 0.7f, 0.7f,
        -1.00f, 0.94f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.99f, 0.94f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.94f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.96f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.98f, 0.96f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.97f, 0.96f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.94f, 1.00f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.94f, 0.99f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.91f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.94f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.96f, 0.97f, 0.7f, 0.7f, 0.7f,
        -0.94f, 1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.98f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, 1.00f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.99f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.98f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.91f, 0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.94f, 0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.96f, 0.97f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.91f, -1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.91f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.91f, -1.00f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.94f, -1.00f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.99f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.98f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.98f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.98f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.97f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.91f, 1.00f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.91f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.94f, 1.00f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.94f, 0.99f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.94f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.91f, -0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.94f, -0.96f, 0.98f, 0.7f, 0.7f, 0.7f,
        -0.96f, -0.96f, 0.97f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.94f, 0.91f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.96f, 0.91f, 0.7f, 0.7f, 0.7f,
        -1.00f, -0.91f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.99f, -0.94f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.96f, 0.94f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.91f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.98f, -0.94f, 0.96f, 0.7f, 0.7f, 0.7f,
        -0.97f, -0.96f, 0.96f, 0.7f, 0.7f, 0.7f,
    };

    GLuint indices[] = {
    3, 5, 0,
    4, 6, 3,
    16, 7, 4,
    6, 8, 5,
    6, 10, 9,
    17, 10, 7,
    11, 13, 1,
    12, 14, 11,
    24, 15, 12,
    14, 16, 13,
    14, 18, 17,
    25, 18, 15,
    19, 21, 2,
    20, 22, 19,
    8, 23, 20,
    22, 24, 21,
    22, 26, 25,
    9, 26, 23,
    10, 18, 26,
    30, 32, 27,
    31, 33, 30,
    43, 34, 31,
    33, 35, 32,
    33, 37, 36,
    44, 37, 34,
    38, 40, 28,
    39, 41, 38,
    51, 42, 39,
    41, 43, 40,
    41, 45, 44,
    52, 45, 42,
    46, 48, 29,
    47, 49, 46,
    35, 50, 47,
    49, 51, 48,
    49, 53, 52,
    36, 53, 50,
    37, 45, 53,
    57, 59, 54,
    58, 60, 57,
    70, 61, 58,
    60, 62, 59,
    60, 64, 63,
    71, 64, 61,
    65, 67, 55,
    66, 68, 65,
    78, 69, 66,
    68, 70, 67,
    68, 72, 71,
    79, 72, 69,
    73, 75, 56,
    74, 76, 73,
    62, 77, 74,
    76, 78, 75,
    76, 80, 79,
    63, 80, 77,
    64, 72, 80,
    84, 86, 81,
    85, 87, 84,
    97, 88, 85,
    87, 89, 86,
    87, 91, 90,
    98, 91, 88,
    92, 94, 82,
    93, 95, 92,
    105, 96, 93,
    95, 97, 94,
    95, 99, 98,
    106, 99, 96,
    100, 102, 83,
    101, 103, 100,
    89, 104, 101,
    103, 105, 102,
    103, 107, 106,
    90, 107, 104,
    91, 99, 107,
    111, 113, 108,
    112, 114, 111,
    124, 115, 112,
    114, 116, 113,
    114, 118, 117,
    125, 118, 115,
    119, 121, 109,
    120, 122, 119,
    132, 123, 120,
    122, 124, 121,
    122, 126, 125,
    133, 126, 123,
    127, 129, 110,
    128, 130, 127,
    116, 131, 128,
    130, 132, 129,
    130, 134, 133,
    117, 134, 131,
    118, 126, 134,
    138, 140, 135,
    139, 141, 138,
    151, 142, 139,
    141, 143, 140,
    141, 145, 144,
    152, 145, 142,
    146, 148, 136,
    147, 149, 146,
    159, 150, 147,
    149, 151, 148,
    149, 153, 152,
    160, 153, 150,
    154, 156, 137,
    155, 157, 154,
    143, 158, 155,
    157, 159, 156,
    157, 161, 160,
    144, 161, 158,
    145, 153, 161,
    165, 167, 162,
    166, 168, 165,
    178, 169, 166,
    168, 170, 167,
    168, 172, 171,
    179, 172, 169,
    173, 175, 163,
    174, 176, 173,
    186, 177, 174,
    176, 178, 175,
    176, 180, 179,
    187, 180, 177,
    181, 183, 164,
    182, 184, 181,
    170, 185, 182,
    184, 186, 183,
    184, 188, 187,
    171, 188, 185,
    172, 180, 188,
    192, 194, 189,
    193, 195, 192,
    205, 196, 193,
    195, 197, 194,
    195, 199, 198,
    206, 199, 196,
    200, 202, 190,
    201, 203, 200,
    213, 204, 201,
    203, 205, 202,
    203, 207, 206,
    214, 207, 204,
    208, 210, 191,
    209, 211, 208,
    197, 212, 209,
    211, 213, 210,
    211, 215, 214,
    198, 215, 212,
    199, 207, 215,
    189, 154, 137,
    194, 155, 154,
    197, 143, 155,
    209, 140, 143,
    208, 135, 140,
    137, 30, 27,
    156, 31, 30,
    159, 43, 31,
    147, 40, 43,
    146, 28, 40,
    29, 19, 2,
    48, 20, 19,
    51, 8, 20,
    39, 5, 8,
    38, 0, 5,
    163, 200, 190,
    175, 201, 200,
    178, 213, 201,
    166, 210, 213,
    165, 191, 210,
    82, 73, 56,
    94, 74, 73,
    97, 62, 74,
    85, 59, 62,
    84, 54, 59,
    136, 111, 108,
    148, 112, 111,
    151, 124, 112,
    139, 121, 124,
    138, 109, 121,
    164, 65, 55,
    183, 66, 65,
    186, 78, 66,
    174, 75, 78,
    173, 56, 75,
    55, 11, 1,
    67, 12, 11,
    70, 24, 12,
    58, 21, 24,
    57, 2, 21,
    83, 192, 189,
    102, 193, 192,
    105, 205, 193,
    93, 202, 205,
    92, 190, 202,
    110, 181, 164,
    129, 182, 181,
    132, 170, 182,
    120, 167, 170,
    119, 162, 167,
    1, 127, 110,
    13, 128, 127,
    16, 116, 128,
    4, 113, 116,
    3, 108, 113,
    27, 100, 83,
    32, 101, 100,
    35, 89, 101,
    47, 86, 89,
    46, 81, 86,
    108, 28, 136,
    2, 81, 29,
    110, 55, 1,
    27, 189, 137,
    56, 190, 82,
    162, 135, 191,
    3, 6, 5,
    4, 7, 6,
    16, 17, 7,
    6, 9, 8,
    6, 7, 10,
    17, 18, 10,
    11, 14, 13,
    12, 15, 14,
    24, 25, 15,
    14, 17, 16,
    14, 15, 18,
    25, 26, 18,
    19, 22, 21,
    20, 23, 22,
    8, 9, 23,
    22, 25, 24,
    22, 23, 26,
    9, 10, 26,
    30, 33, 32,
    31, 34, 33,
    43, 44, 34,
    33, 36, 35,
    33, 34, 37,
    44, 45, 37,
    38, 41, 40,
    39, 42, 41,
    51, 52, 42,
    41, 44, 43,
    41, 42, 45,
    52, 53, 45,
    46, 49, 48,
    47, 50, 49,
    35, 36, 50,
    49, 52, 51,
    49, 50, 53,
    36, 37, 53,
    57, 60, 59,
    58, 61, 60,
    70, 71, 61,
    60, 63, 62,
    60, 61, 64,
    71, 72, 64,
    65, 68, 67,
    66, 69, 68,
    78, 79, 69,
    68, 71, 70,
    68, 69, 72,
    79, 80, 72,
    73, 76, 75,
    74, 77, 76,
    62, 63, 77,
    76, 79, 78,
    76, 77, 80,
    63, 64, 80,
    84, 87, 86,
    85, 88, 87,
    97, 98, 88,
    87, 90, 89,
    87, 88, 91,
    98, 99, 91,
    92, 95, 94,
    93, 96, 95,
    105, 106, 96,
    95, 98, 97,
    95, 96, 99,
    106, 107, 99,
    100, 103, 102,
    101, 104, 103,
    89, 90, 104,
    103, 106, 105,
    103, 104, 107,
    90, 91, 107,
    111, 114, 113,
    112, 115, 114,
    124, 125, 115,
    114, 117, 116,
    114, 115, 118,
    125, 126, 118,
    119, 122, 121,
    120, 123, 122,
    132, 133, 123,
    122, 125, 124,
    122, 123, 126,
    133, 134, 126,
    127, 130, 129,
    128, 131, 130,
    116, 117, 131,
    130, 133, 132,
    130, 131, 134,
    117, 118, 134,
    138, 141, 140,
    139, 142, 141,
    151, 152, 142,
    141, 144, 143,
    141, 142, 145,
    152, 153, 145,
    146, 149, 148,
    147, 150, 149,
    159, 160, 150,
    149, 152, 151,
    149, 150, 153,
    160, 161, 153,
    154, 157, 156,
    155, 158, 157,
    143, 144, 158,
    157, 160, 159,
    157, 158, 161,
    144, 145, 161,
    165, 168, 167,
    166, 169, 168,
    178, 179, 169,
    168, 171, 170,
    168, 169, 172,
    179, 180, 172,
    173, 176, 175,
    174, 177, 176,
    186, 187, 177,
    176, 179, 178,
    176, 177, 180,
    187, 188, 180,
    181, 184, 183,
    182, 185, 184,
    170, 171, 185,
    184, 187, 186,
    184, 185, 188,
    171, 172, 188,
    192, 195, 194,
    193, 196, 195,
    205, 206, 196,
    195, 198, 197,
    195, 196, 199,
    206, 207, 199,
    200, 203, 202,
    201, 204, 203,
    213, 214, 204,
    203, 206, 205,
    203, 204, 207,
    214, 215, 207,
    208, 211, 210,
    209, 212, 211,
    197, 198, 212,
    211, 214, 213,
    211, 212, 215,
    198, 199, 215,
    189, 194, 154,
    194, 197, 155,
    197, 209, 143,
    209, 208, 140,
    208, 191, 135,
    137, 156, 30,
    156, 159, 31,
    159, 147, 43,
    147, 146, 40,
    146, 136, 28,
    29, 48, 19,
    48, 51, 20,
    51, 39, 8,
    39, 38, 5,
    38, 28, 0,
    163, 175, 200,
    175, 178, 201,
    178, 166, 213,
    166, 165, 210,
    165, 162, 191,
    82, 94, 73,
    94, 97, 74,
    97, 85, 62,
    85, 84, 59,
    84, 81, 54,
    136, 148, 111,
    148, 151, 112,
    151, 139, 124,
    139, 138, 121,
    138, 135, 109,
    164, 183, 65,
    183, 186, 66,
    186, 174, 78,
    174, 173, 75,
    173, 163, 56,
    55, 67, 11,
    67, 70, 12,
    70, 58, 24,
    58, 57, 21,
    57, 54, 2,
    83, 102, 192,
    102, 105, 193,
    105, 93, 205,
    93, 92, 202,
    92, 82, 190,
    110, 129, 181,
    129, 132, 182,
    132, 120, 170,
    120, 119, 167,
    119, 109, 162,
    1, 13, 127,
    13, 16, 128,
    16, 4, 116,
    4, 3, 113,
    3, 0, 108,
    27, 32, 100,
    32, 35, 101,
    35, 47, 89,
    47, 46, 86,
    46, 29, 81,
    108, 0, 28,
    2, 54, 81,
    110, 164, 55,
    27, 83, 189,
    56, 163, 190,
    162, 109, 135,

    };


	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO); //DIBUJO DE CUBO, HACER TABLA DE MESA
	
	 //   model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(5.0f, 0.1f, 2.3f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));


		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//

		////creamos una nueva pat (la 1)
		//
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.9f, 0.1f));
		//model = glm::translate(model, glm::vec3(-24.5f, 0.5f, 10.9f));

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//

		////creamos una nueva pat (la 2)

		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.9f, 0.1f));
		//model = glm::translate(model, glm::vec3(24.5f, 0.5f, 10.9f));

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//
		////creamos una nueva pat (la 3)

		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.9f, 0.1f));
		//model = glm::translate(model, glm::vec3(24.5f, 0.5f, -10.9f));

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		////creamos una nueva pat (la 4)

		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.9f, 0.1f));
		//model = glm::translate(model, glm::vec3(-24.5f, 0.5f, -10.9f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		//glDrawArrays(GL_TRIANGLES, 0, 1284);
		
		glDrawElements(GL_TRIANGLES, 1284, GL_UNSIGNED_INT, 0); 
        ///CAMBIAR SIEMPRE EL NUMERO DE INDICES A DIBUJAR, EN ESTE CASO 36 PORQUE SON 12 TRIANGULOS, PERO SI SE QUISIERA DIBUJAR 
        //TODO EL MODELO SE DEBERIA PONER 1284 QUE ES EL NUMERO TOTAL DE INDICES

		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotX += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotX -= 0.4f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotY += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotY -= 0.4f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		rotZ += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		rotZ -= 0.4f;

}


