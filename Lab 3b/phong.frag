#version 150

// Simplified Phong: No materials, only one, hard coded light source
// and no ambient

out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exSurface; // Phong (specular)
in vec2 exTexCoord;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates! You will usually give light in world coordinates.
	float diffuse, specular, shade;
	
	// Diffuse
	diffuse = dot(normalize(exNormal), light);
	diffuse = max(0.0, diffuse); // No negative light
	
	// Specular
	vec3 r = reflect(-light, normalize(exNormal));
	vec3 v = normalize(-exSurface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);
	shade = 0.7*diffuse + 1.0*specular + 0.3;
	
	vec4 color = vec4(exTexCoord.x, exTexCoord.y, exTexCoord.y, 1.0);
	
	outColor = vec4(shade, shade, shade, 1.0) * color;
}
