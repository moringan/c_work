#include "./emu.h"
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MODIFIER 10;

// Window size
int display_width = SCREEN_WIDTH * MODIFIER;
int display_height = SCREEN_HEIGHT * MODIFIER;
 
/* Initialize OpenGL Graphics 
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(25.0f);
}
*/

void setupTexture()
{
	// Clear screen
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
			data[y][x][0] = data[y][x][1] = data[y][x][2] = 0;

	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void updateTexture()
{	
	// Update pixels
/*	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
			if(gfx[y][x] == 0)
				data[y][x][0] = data[y][x][1] = data[y][x][2] = 0;	
			else 
				data[y][x][0] = data[y][x][1] = data[y][x][2] = 255;  */
		
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
//	int delay = 3000;
	for(;;) {
		emulate_cycle();
//		usleep(delay);
		if(draw_flag) {
/*			for(int i = 0; i < 32; i++) {
				for(int j = 0; j < 64; j++)
					if(gfx[i][j] != 0x0) {
						data[i][j][0] = 255;
						data[i][j][1] = 255;
						data[i][j][2] = 255;
					}
					else {
						data[i][j][0] = 0;
						data[i][j][1] = 0;
						data[i][j][2] = 0;
					}
			
			}
			glDrawPixels(64, 32, GL_RGB, GL_UNSIGNED_BYTE, data);*/
			draw_flag = 0;
			updateTexture();
			glutSwapBuffers();
		}
	}
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);        
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}

int main(int argc, char** argv) {
	int delay = 1000;
	if(argc != 2) {
		printf("need a filename\n");
		exit(1);
	}

	initialize();
	load_game(argv[1]);
	for(;;) {
		emulate_cycle();
		usleep(delay);
		if(draw_flag) {
			debug_render();
			draw_flag = 0;
		}
	}




/*	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(display_width, display_height);
	glutCreateWindow("Mike's Chip-8 Emulator");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape_window); 
	glutKeyboardFunc(keyboard_down);
	glutKeyboardUpFunc(keyboard_up); 
	setupTexture();


	glutMainLoop();                 // Enter the event-processing loop
*/

	return 0;
}

void keyboard_down(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		keys[0x1] = 1;
	else if(key == '2')	keys[0x2] = 1;
	else if(key == '3')	keys[0x3] = 1;
	else if(key == '4')	keys[0xC] = 1;

	else if(key == 'q')	keys[0x4] = 1;
	else if(key == 'w')	keys[0x5] = 1;
	else if(key == 'e')	keys[0x6] = 1;
	else if(key == 'r')	keys[0xD] = 1;

	else if(key == 'a')	keys[0x7] = 1;
	else if(key == 's')	keys[0x8] = 1;
	else if(key == 'd')	keys[0x9] = 1;
	else if(key == 'f')	keys[0xE] = 1;

	else if(key == 'z')	keys[0xA] = 1;
	else if(key == 'x')	keys[0x0] = 1;
	else if(key == 'c')	keys[0xB] = 1;
	else if(key == 'v')	keys[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboard_up(unsigned char key, int x, int y)
{
	if(key == '1')		keys[0x1] = 0;
	else if(key == '2')	keys[0x2] = 0;
	else if(key == '3')	keys[0x3] = 0;
	else if(key == '4')	keys[0xC] = 0;

	else if(key == 'q')	keys[0x4] = 0;
	else if(key == 'w')	keys[0x5] = 0;
	else if(key == 'e')	keys[0x6] = 0;
	else if(key == 'r')	keys[0xD] = 0;

	else if(key == 'a')	keys[0x7] = 0;
	else if(key == 's')	keys[0x8] = 0;
	else if(key == 'd')	keys[0x9] = 0;
	else if(key == 'f')	keys[0xE] = 0;

	else if(key == 'z')	keys[0xA] = 0;
	else if(key == 'x')	keys[0x0] = 0;
	else if(key == 'c')	keys[0xB] = 0;
	else if(key == 'v')	keys[0xF] = 0;
}

