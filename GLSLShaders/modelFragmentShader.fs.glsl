#version 450
uniform vec4 ModelColor;

in vec3 normal;
in vec3 lightDir;

out vec4 fragColor;

void main()
{
    fragColor = ModelColor * dot(lightDir, normal);
}