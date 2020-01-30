#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "glm/glm.hpp"

// Printing
void printRM(std::string name, glm::mat4 &M);
void printRM(std::string name, glm::mat3 &M);

// File IO
std::string loadEntireFile(std::string filepath);

#endif
