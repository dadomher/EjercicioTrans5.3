#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

class Shader {
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	GLuint Program;

	const GLchar* vertexPath;
	const GLchar* fragmentPath;



	void USE() {
		glUseProgram(Program);
	}

private:

};


#endif
