#version 330 core

out vec4 FragColor;
in float COL;

void main()
{
	FragColor = vec4(COL, 0, 0, 1);
}
