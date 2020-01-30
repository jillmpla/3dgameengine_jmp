#ifndef SHADER_GL_H
#define SHADER_GL_H

#include <GL/glew.h>
#include <string>
#include <vector>

// Shader utility functions
GLint checkErrorFromShaderCompile(GLuint shaderID);
GLint checkErrorFromShaderLink(GLuint programID);
GLuint createAndCompileShader(const char *shaderCode, GLenum shaderType);
GLuint createAndLinkShaderProgram(std::vector<GLuint> allShaderIDs);

// Creates shader programs
GLuint initShaderProgramFromSource(std::string vertexShaderCode, std::string fragmentShaderCode);
GLuint initShaderProgramFromFiles(std::string vertexFilePath, std::string fragmentFilePath);

// Base Shader class
class ShaderGL {
protected:
	GLuint programID = 0;

public:
	ShaderGL(std::string vertex, std::string fragment, bool isFilepath);
	virtual ~ShaderGL();
	virtual void activate();
	virtual void deactivate();
};

#endif
