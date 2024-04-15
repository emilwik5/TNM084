#version 150

out vec4 outColor;

in vec2 texCoord;
uniform sampler2D tex;
in vec3 exNormal;

void main(void)
{
	vec4 t = texture(tex, texCoord);
	vec3 n = normalize(exNormal);
//	if (t.a < 0.01) discard;
//	else
	float shade = n.z;
		outColor = t * (shade+0.7);
	outColor.a = 1.0;
	
//	outColor = vec4(exNormal, 1.0);
}
