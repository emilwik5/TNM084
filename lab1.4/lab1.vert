// Lab 1 vertex shader. You do not need to make changes here.

#version 150

in  vec3 in_Position;
in vec2 in_TexCoord;
out vec2 texCoord;

void main(void)
{
	texCoord = in_TexCoord;
	gl_Position = vec4(in_Position, 1.0);
}
