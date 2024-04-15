// TNM084 Lab 1
// Procedural textures
// Draws concentric circles on CPU and GPU
// Make something more interesting!

// For the CPU part, you should primarily work in maketexture() below.

//uses framework Cocoa
//uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include <stdio.h>
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"

// Lab 1 specific
// Code for you to access to make interesting patterns.
// This is for CPU. For GPU, I provide separate files
// with functions that you can add to your fragment shaders.
#include "noise1234.h"
#include "simplexnoise1234.h"
#include "cellular.h"

#define kTextureSize 512
GLubyte ptex[kTextureSize][kTextureSize][3];
const float ringDensity = 20.0;


// Example: Radial pattern.
void maketexture()
{
	int x, y;
	float fx, fy, fxo, fyo;
	char val;

	for (x = 0; x < kTextureSize; x++)
	for (y = 0; y < kTextureSize; y++)
	{
		fx = (float)(x-kTextureSize/2.)/kTextureSize*2.;
		fy = (float)(y-kTextureSize/2.)/kTextureSize*2.;

		fxo = sqrt(fx*fx+fy*fy); //Changed to multiplication
		fyo = sqrt(fx*fx+fy*fy);
		fxo = sin(fxo * 100 * noise1(fx)*2);
		fyo = cos(fxo * 50 * sin(noise2(y, fy)*2*3.14152617));

		if (fxo > 1.0) fxo = 1;
		if (fxo < -1.0) fxo = -1.0;
		if (fyo > 1.0) fyo = 1.0;
		if (fyo < -1.0) fyo = -1.0;

		ptex[x][y][0] = fxo * 127 + 127;
		ptex[x][y][1] = fyo * 127;
		ptex[x][y][2] = 127; //ändrar B kanalen i RGB.
	}
}

// Globals
// Data would normally be read from files
vec3 vertices[] = { {-1.0f,-1.0f,0.0f},
					{1.0f,-1.0f,0.0f},
					{1.0f,1.0f,0.0f},
					{-1.0f,1.0f,0.0f},
					{1.0f,1.0f,0.0f},
					{1.0f,-1.0f,0.0f} };
vec2 texCoords[] = {{0.0f,0.0f},
					{1.0f,0.0f},
					{1.0f,1.0f},
					{0.0f,1.0f},
					{1.0f,1.0f},
					{1.0f,0.0f} };
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

// vertex array object
unsigned int vertexArrayObjID;
// Texture reference
GLuint texid;
// Switch between CPU and shader generation
int displayGPUversion = 1;
	// Reference to shader program
	GLuint program;
	Model* quad;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int texBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1.vert", "lab1.frag");
	glUseProgram(program);
	printError("init shader");

	// Upload geometry to the GPU:
	quad = LoadDataToModel(vertices,NULL,texCoords,NULL,indices,4,6);

	// Texture unit
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

// Constants common to CPU and GPU
	glUniform1i(glGetUniformLocation(program, "displayGPUversion"), 0); // shader generation off
	glUniform1f(glGetUniformLocation(program, "ringDensity"), ringDensity);

	maketexture();

// Upload texture
	glGenTextures(1, &texid); // texture id
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kTextureSize, kTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, ptex);

	// End of upload of geometry
	printError("init arrays");
}

// Switch on any key
void key(unsigned char key, int x, int y)
{
	displayGPUversion = !displayGPUversion;
	glUniform1i(glGetUniformLocation(program, "displayGPUversion"), displayGPUversion); // shader generation off
	printf("Changed flag to %d\n", displayGPUversion);
	glutPostRedisplay();
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float etime = 0.001 * glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "time"), etime);

	DrawModel(quad, program, "in_Position", NULL, "in_TexCoord");

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(kTextureSize, kTextureSize);
	glutCreateWindow ("Lab 1");
	glutDisplayFunc(display);
	glutRepeatingTimer(5);
	glutKeyboardFunc(key);
	init ();
	glutMainLoop();
}
