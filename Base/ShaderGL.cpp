#include "ShaderGL.hpp"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <algorithm>


// Check for error messages when compiling shaders.
// Returns GL_TRUE if compile was successful; GL_FALSE otherwise.
GLint checkErrorFromShaderCompile(GLuint shaderID) {

	GLint resultGL = GL_FALSE;
	int infoLogLength;

	// Get the compilation status
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &resultGL);

	// Get how long the info message is
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	// Make a "string" that has the right length
	std::vector<char> shaderErrorMessage(std::max(infoLogLength, int(1)));

	// Get the actual shader message
	glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);

	// Print error message
	std::cout << &shaderErrorMessage[0] << std::endl;

	// Return OpenGL error
	return resultGL;
}

// Check for error messages when linking shaders.
// Returns GL_TRUE if compile was successful; GL_FALSE otherwise.
GLint checkErrorFromShaderLink(GLuint programID) {

	GLint resultGL = GL_FALSE;
	int infoLogLength;

	// Get linking status
	glGetProgramiv(programID, GL_LINK_STATUS, &resultGL);

	// Get how long the info message is
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	// Make a "string" that has the right length
	std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));

	// Get the actual linker message
	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);

	// Print the error message
	std::cout << &programErrorMessage[0] << std::endl;

	// Return OpenGL error
	return resultGL;
}

// Creates and compiles shader from code string; returns shader ID
GLuint createAndCompileShader(const char *shaderCode, GLenum shaderType) {
	// Create the shader ID
	GLuint shaderID = glCreateShader(shaderType);

	// Compile the vertex shader...
	std::cout << "Compiling shader..." << std::endl;
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	// Checking result of compilation...
	GLint compileOK = checkErrorFromShaderCompile(shaderID);
	if (!compileOK || shaderID == 0) {
		glDeleteShader(shaderID);		
		std::cout << "Error compiling shader." << std::endl;
		throw std::runtime_error("Error compiling shader.");
	}

	// Return shader ID
	return shaderID;
}

// Given a list of compiled shaders, create and link a shader program (ID returned).
GLuint createAndLinkShaderProgram(std::vector<GLuint> allShaderIDs) {

	// Create program ID and attach shaders
	std::cout << "Linking program..." << std::endl;
	GLuint programID = glCreateProgram();
	for (GLuint &shaderID : allShaderIDs) {
		glAttachShader(programID, shaderID);
	}

	// Actually link the program
	glLinkProgram(programID);

	// Detach the shaders (program already linked, successful or not)
	for (GLuint &shaderID : allShaderIDs) {
		glDetachShader(programID, shaderID);
	}

	// Check linking
	GLint linkOK = checkErrorFromShaderLink(programID);
	if (!linkOK || programID == 0) {
		glDeleteProgram(programID);		
		std::cout << "Error linking shaders." << std::endl;
		throw std::runtime_error("Error linking shaders.");
	}

	// Return program ID
	return programID;
}

// Does the following:
// - Creates and compiles vertex and fragment shaders (from provided code strings)
// - Creates and links shader program
// - Deletes vertex and fragment shaders
GLuint initShaderProgramFromSource(std::string vertexShaderCode, std::string fragmentShaderCode) {

	// Print out the shader code
	std::cout << "VERTEX SHADER CODE:" << std::endl;
	std::cout << vertexShaderCode << std::endl;
	std::cout << "END VERTEX SHADER CODE" << std::endl;

	std::cout << "FRAGMENT SHADER CODE:" << std::endl;
	std::cout << fragmentShaderCode << std::endl;
	std::cout << "END FRAGMENT SHADER CODE" << std::endl;

	GLuint vertID = 0;
	GLuint fragID = 0;
	GLuint programID = 0;

	try {
		// Create and compile shaders
		std::cout << "Vertex shader: ";
		vertID = createAndCompileShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
		std::cout << "Fragment shader: ";
		fragID = createAndCompileShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

		// Create and link program
		programID = createAndLinkShaderProgram({ vertID, fragID });

		// We don't need the individual shaders anymore, so delete them
		glDeleteShader(vertID);
		glDeleteShader(fragID);

		std::cout << "Program successfully compiled and linked!" << std::endl;
	}
	catch (std::exception e) {
		// Cleanup shaders and shader program, just in case
		if (vertID) glDeleteShader(vertID);
		if (fragID) glDeleteShader(fragID);
		if (programID) glDeleteProgram(programID);
		// Rethrow exception
		throw e;
	}

	return programID;
}

// Does the following:
// - Loads vertex and fragment shader code from files
// - Creates and compiles vertex and fragment shaders
// - Creates and links shader program
// - Deletes vertex and fragment shaders
GLuint initShaderProgramFromFiles(std::string vertexFilePath, std::string fragmentFilePath) {

	std::cout << "Vertex shader path: " << vertexFilePath << std::endl;
	std::cout << "Fragment shader path: " << fragmentFilePath << std::endl;

	// Load the contents of the files
	std::string vertexShaderCode = loadEntireFile(vertexFilePath);
	std::string fragmentShaderCode = loadEntireFile(fragmentFilePath);

	// Create shader program!
	GLuint programID = initShaderProgramFromSource(vertexShaderCode, fragmentShaderCode);

	// Return program ID
	return programID;
}

// Base Shader class
ShaderGL::ShaderGL(std::string vertex, std::string fragment, bool isFilepath) {
	if (isFilepath) {
		std::cout << "Creating shader from files..." << std::endl;
		programID = initShaderProgramFromFiles(vertex, fragment);
	}
	else {
		std::cout << "Creating shader from source..." << std::endl;
		programID = initShaderProgramFromSource(vertex, fragment);
	}
}

ShaderGL::~ShaderGL() {
	glUseProgram(0);			// Good idea to make sure not active
	glDeleteProgram(programID);
}

void ShaderGL::activate() {
	glUseProgram(programID);
}

void ShaderGL::deactivate() {
	glUseProgram(0);
}




