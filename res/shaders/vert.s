#version 330 core

layout (location = 0) in vec3 aPos;

out float COL;

void main()
{
	gl_Position = vec4(aPos.xy, 0, 1);
	COL = aPos.z;
}
