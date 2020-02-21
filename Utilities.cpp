#include "Utilities.h"
#include <iostream>
#include <fstream>


// Print matrix in row-major form
void printRM(std::string name, glm::mat4 &M) {
	std::cout << name << ":" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << M[j][i] << ", ";
		}
		std::cout << std::endl;
	}
}

// Print matrix in row-major form
void printRM(std::string name, glm::mat3 &M) {
	std::cout << name << ":" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << M[j][i] << ", ";
		}
		std::cout << std::endl;
	}
}

// Read the entire contents of an ASCII file as a string
// Heavily inspired by: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string loadEntireFile(std::string filepath) {
	
    // Create a string to hold the contents of the file
    std::string contents;
	std::ifstream infile;

	try {
		// Open the file as a BINARY file
		infile = std::ifstream(filepath, std::ios::in | std::ios::binary);

		// Find out the size of the file and resize the string accordingly.
		// seekg --> go to that spot in the file
		// ios::end --> end of the file
		// ios::beg --> beginning of the file
		// tellg --> get current position in file
		infile.seekg(0, std::ios::end);
		contents.resize(infile.tellg());

		// Go back to the beginning of the file
		infile.seekg(0, std::ios::beg);

		// Read in entire contents of the file
		// NOTE: &contents[0] --> gives you starting address of string
		infile.read(&contents[0], contents.size());

		// Close the file
		infile.close();
	}
	catch (std::exception& e) {
		if (infile.is_open()) infile.close();
    	std::cout << e.what() << std::endl;
		throw e;		
	}

	return contents;
}
