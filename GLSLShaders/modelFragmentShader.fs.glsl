#version 450
uniform vec3 ModelColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(ModelColor, 1.0);
}