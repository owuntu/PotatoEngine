#version 450
in vec3 vertColor;

out vec4 fragColor;

void main()
{
    //fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	fragColor = vec4(vertColor, 1.0);
}