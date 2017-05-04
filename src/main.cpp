//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <cmath>
#include <SOIL.h>
#include "shaderPractica.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h";
#include "Model.h"

using namespace std;
using namespace glm;

//ancho y alto pantalla
const GLint WIDTH = 800, HEIGHT = 600;
//para triangulizar la imagen o solo ver los vertices
bool WIDEFRAME = false;
// ancho y alto de las texturas
int width, height;
//variable con la que regulo la transparencia entre texturas
float transformacion = 0.5f;
//modo de pintar las texturas
float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
//Variable con las que controlo la rotacion de los cubos 
float xRot = 0.0f, yRot = 0.0f;
//
GLfloat fov = 45.0f;

Camera c1;
Model m1;
//variables donde calculamos y almacenamos los valores de posicion, direccion de la camara

vec3 cPos = glm::vec3(0.0f, 0.0f, 3.0f);
vec3 cFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cUp = glm::vec3(0.0f, 1.0f, 0.0f);

// velocidad de la posicion de la camara
GLfloat cSpeed = 0.01f; 
GLfloat sensivity = 0.5f;

int main();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void pintar_vao(GLuint VAO);
mat4 GenerateModelMatrix(vec3 rot, vec3 scal, vec3 cubepositions);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}

	//set functions when callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //escondemos y impedimos que el cursor pueda abandonar la pantalla
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport0
	int screenWithd, screenHeigtht;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeigtht);

	//origen de la camara, dimensiones de la ventana
	glViewport(0, 0, screenWithd, screenHeigtht);

	//fondo
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//cargamos los shader
	Shader sh = Shader("./src/perspectiveVertex.vertexshader", "./src/perspectiveFragment.fragmentshader");

	//VAO
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	// Crear los VBO, VAO y EBO
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Establecer el objeto
	glBindVertexArray(VAO);
	//Declarar el VBO y el EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Enlazar el buffer con openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubesPositionBuffer), CubesPositionBuffer, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0); //POSICION ARRAY, N� VALORES, LARGO TOTAL VERTICES, VALOR QUE EMPIEZA EN EL VECTOR
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	//TEXTURAS
	// Crear la textura
	GLuint texture;

	//Reservar memoria en openGL
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//parametros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//libera la imagen
	SOIL_free_image_data(image);

	// Crear la textura
	GLuint texture2;

	//Reservar memoria en openGL
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//parametros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	image = SOIL_load_image("./src/texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//libera la imagen
	SOIL_free_image_data(image);

	//libera el puntero de la textura
	glBindTexture(GL_TEXTURE_2D, 0);

	//liberar el buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//liberar el buffer de vertices
	glBindVertexArray(0);

	//CAMARA
	c1 = Camera(cPos, cFront, sensivity, fov);

	//Matriz de proyeccion
	mat4 proj;

	//Matriz del modelo
	mat4 model;

	//Matriz del view de la camara
	mat4 view;

	m1 = Model("./models/spider/spider.obj");
	//m1.loadModel("./models/spider/spider.obj");

	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		c1.DoMovement(window);

		//z Buffer
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Establecer el color de fondo
		glClearColor(1.0, 1.0, 1.0, 1.0);

		//uniform de las texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(sh.Program, "Texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(sh.Program, "Texture2"), 1);

		//
		sh.USE();

		//

		//calculamos a cada iteracion el view de la camara para aplicar a cada una de estas el valor de la posicion
		proj = perspective(radians(c1.GetFOV()), 800.0f / 600.0f, 1.0f, 100.0f);
		model = GenerateModelMatrix(vec3(0.25f), vec3(0.0f), vec3(0.0f));

		//valor de la transparencia, proyeccion y el view de la camara qude le pasamos al shader
		glUniform1f(glGetUniformLocation(sh.Program, "trans"), transformacion);
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "view"), 1, GL_FALSE, value_ptr(c1.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "final"), 1, GL_FALSE, glm::value_ptr(model));
		m1.Draw(sh, GL_FILL);

		/*model = GenerateModelMatrix(vec3(xRot, yRot, 0.0f), vec3(1.0f), CubesPositionBuffer[0]);
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "final"), 1, GL_FALSE, glm::value_ptr(model));
		pintar_vao(VAO);


		for (int i = 1; i < 10; i++) {
			model = GenerateModelMatrix(vec3(glfwGetTime() * 100, glfwGetTime() * 100, 0.0f), vec3(1.0f), CubesPositionBuffer[i]);
			glUniformMatrix4fv(glGetUniformLocation(sh.Program, "final"), 1, GL_FALSE, glm::value_ptr(model));
			pintar_vao(VAO);
		}*/

		//intercambia el framebuffer
		glfwSwapBuffers(window);
		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();
	}
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicaci�n y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) WIDEFRAME = !WIDEFRAME;

	//aumentar o disminuir la transparencia entre las texturas
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (transformacion <= 0.9f) transformacion += 0.1f;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (transformacion >= 0.1f) transformacion -= 0.1f;
	}

	//dar una rotacion al cubo
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) yRot += 1;
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) yRot -= 1;
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) xRot += 1;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) xRot -= 1;

	//dar una rotacion a la camara
	//if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) cPos += cameraSpeed * cFront;
	//if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) cPos -= cameraSpeed * cFront;
	//if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) cPos -= glm::normalize(cross(cFront, cUp)) * cameraSpeed;
	//if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) cPos += glm::normalize(cross(cFront, cUp)) * cameraSpeed;
}

void pintar_vao(GLuint VAO) {
	//pitar el VAO
	glBindVertexArray(VAO);

	if (WIDEFRAME) {
		//pintar con lineas
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else {
		//pintar con triangulos
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

//funcion para crear cubos
mat4 GenerateModelMatrix(vec3 rot, vec3 scal, vec3 cubepositions) {
	//variables funcion GenerateModelMatrix
	mat4 funcTransModel, funcRotModel, funcScalModel, funcFinalModel;

	funcTransModel = translate(funcTransModel, cubepositions);

	funcRotModel = glm::rotate(funcRotModel, radians(rot[0]), vec3(1.0f, 0.0f, 0.0f));
	funcRotModel = glm::rotate(funcRotModel, radians(rot[1]), vec3(0.0f, 1.0f, 0.0f));
	funcRotModel = glm::rotate(funcRotModel, radians(rot[2]), vec3(0.0f, 0.0f, 1.0f));

	funcScalModel = glm::scale(funcScalModel, scal);
	funcFinalModel = funcTransModel * funcRotModel * funcScalModel;

	return funcFinalModel;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	c1.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//utilizamos el fov para determinar cuanto podemos ver de la escena y establecemos unos limites
	c1.MouseScroll(window, yoffset);
}

