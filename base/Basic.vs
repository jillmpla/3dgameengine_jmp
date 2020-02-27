#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 vertexColor;
out vec4 viewPos;
out vec3 viewNormal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 normMat;

void main()
{		
	// Get position of vertex (object space)
	vec4 objPos = vec4(position, 1.0);

	// pass vertex position
	gl_Position = projMat*viewMat*modelMat*objPos;

	// Output per-vertex color
	vertexColor = color;
	
	//Output position and normal
	viewPos = viewMat*modelMat*objPos;
	viewNormal = normMat*normal;
}
