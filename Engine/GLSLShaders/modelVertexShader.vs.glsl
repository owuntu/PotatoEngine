#version 450

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMat;

out vec3 normal;
out vec3 lightDir;

void main()
{
	mat4 mvp = projection * view * modelMat;
	normal = mat3(transpose(inverse(modelMat))) * aNormal;
	normal = normalize(normal);

	// hard code fix light dir from camera
	lightDir = mat3(transpose(view)) * vec3(0, 0, 1);
	lightDir = normalize(lightDir);

	gl_Position = mvp * vec4(aPos, 1.0);
}
