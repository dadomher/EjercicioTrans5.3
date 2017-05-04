#include "Camera.h"

Camera::Camera(){

}

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) {
	this->cameraPos = position;
	this->cameraFront = direction;
	vec3 up = vec3(0.0, 1.0, 0.0);
	this->cameraRight = normalize(cross(direction, up));
	this->cameraUp = cross(cameraRight,direction);
	this->Lastframe = 0;
	this->LastMx = 400;
	this->LastMy = 300;
	this->Sensitivity = sensitivity;
	this->firstMouse = true;
	this->PITCH = 0.0f;
	this->YAW -90.0f;
	this->FOV = fov;
}

//funcion que actualizaremos la camara segun que tecla apretemos
void Camera::DoMovement(GLFWwindow* window) {
	//Detectamos el estado 
	int Status_W = glfwGetKey(window, GLFW_KEY_W);
	int Status_S = glfwGetKey(window, GLFW_KEY_S);
	int Status_A = glfwGetKey(window, GLFW_KEY_A);
	int Status_D = glfwGetKey(window, GLFW_KEY_D);

	GLfloat speed = 0.01, actualFrame = glfwGetTime();
	Deltatime = actualFrame - Lastframe;
	Lastframe = actualFrame;

	GLfloat cSpeed = Deltatime - speed;

	//calculamos la posicion de la camara segun el estado que detectamos previamente de las teclas
	if (Status_W == GLFW_PRESS || Status_W == GLFW_REPEAT) cameraPos += cSpeed * cameraFront;
	else if (Status_S == GLFW_PRESS || Status_S == GLFW_REPEAT) cameraPos -= cSpeed * cameraFront;
	else if (Status_A == GLFW_PRESS || Status_A == GLFW_REPEAT) cameraPos -= glm::normalize(cross(cameraFront, cameraUp)) * cSpeed;
	else if (Status_D == GLFW_PRESS || Status_D == GLFW_REPEAT) cameraPos += glm::normalize(cross(cameraFront, cameraUp)) * cSpeed;
}

void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}

	//calculamos el offset desde el frame anterior al actual
	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;

	//actualizamos frame anterior para la siguiente iteracion
	LastMx = xpos;
	LastMy = ypos;

	//multiplicamos el valor de offset para que el movimiento del raton sea mucho mas llevadero
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	//2 de los 3 angulos que utilizamos en los objetos 3d. Determina el valor de estos angulos.
	YAW += xoffset;
	PITCH += yoffset;

	PITCH = clamp(PITCH, -89.0f, 89.0f);
	YAW = mod(YAW, 360.0f);

	//calculamos la camara frontal
	vec3 front;
	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
}

void Camera::MouseScroll(GLFWwindow* window, double yScroll) {
	if (FOV <= 1.0f) FOV = 1.0f;
	else if (FOV >= 45.0f) FOV = 45.0f;

	if (FOV >= 1.0f && FOV <= 45.0f) {
		FOV -= yScroll;
	}
}

mat4 Camera::LookAt() {
	mat4 lookat;
	mat4 A, B;
	A[0][0] = cameraRight.x;
	A[1][0] = cameraRight.y;
	A[2][0] = cameraRight.z;

	A[0][1] = cameraUp.x;
	A[1][1] = cameraUp.y;
	A[2][1] = cameraUp.z;

	A[0][2] = cameraFront.x;
	A[1][2] = cameraFront.y;
	A[2][2] = cameraFront.z;

	B[3][0] = -cameraPos.x;
	B[3][1] = -cameraPos.y;
	B[3][2] = -cameraPos.z;

	lookat = A*B;
	return lookat;
}

GLfloat Camera::GetFOV() {
	return FOV;
}