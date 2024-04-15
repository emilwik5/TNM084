#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out float worldHeight;
out vec3 exNormal;
out vec2 texCoord;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	texCoord = inTexCoord;
	exNormal = inNormal;
	

	gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0);

	worldHeight = inPosition.y;
}
