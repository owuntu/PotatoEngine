#version 300 es

precision mediump float;

uniform vec4 ModelColor;
uniform int bUseLighting;

in vec3 normal;
in vec3 lightDir;

out vec4 fragColor;

void main()
{
    float cosTheta = (bUseLighting == 1) ? dot(lightDir, normal) : 1.0;
    fragColor = ModelColor * cosTheta;
}