#version 330 core

layout(location = 0) in vec3 position;

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
}
