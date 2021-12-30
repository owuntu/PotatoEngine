#version 450
uniform vec4 ModelColor;

out vec4 fragColor;

void main()
{
    fragColor = ModelColor;
}