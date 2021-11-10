#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMat;

out vec3 vertColor;

void main()
{
	mat4 mvp = projection * view * modelMat;
	gl_Position = mvp * vec4(aPos, 1.0);
	vertColor = aColor;
}
