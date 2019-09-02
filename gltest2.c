

#include <GL/glut.h>

void displayCB(void)		/* function called whenever redisplay needed */
{
  glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
//glPointSize(20.0f);
// glBegin(GL_POINTS);
//	glColor3f(1,1,1);
//	glVertex2i(100,100);
//  glEnd();
//  glColor3f(1.0, 1.0, 1.0);		/* set current color to white */
//  glBegin(GL_POLYGON);			/* draw filled triangle */
 // glVertex2i(200,125);			/* specify each vertex of triangle */
  //glVertex2i(100,375);
  //glVertex2i(300,375);
  //glEnd();	
   glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
      glColor3f(0.0f, 0.0f, 1.0f); // Blue
      glVertex2f(0.1f, -0.6f);
      glVertex2f(0.7f, -0.6f);
      glVertex2f(0.4f, -0.1f);
 
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(0.3f, -0.4f);
      glColor3f(0.0f, 1.0f, 0.0f); // Green
      glVertex2f(0.9f, -0.4f);
      glColor3f(0.0f, 0.0f, 1.0f); // Blue
      glVertex2f(0.6f, -0.9f);
   glEnd();
 
  glFlush();				/* Complete any pending operations */
}

void keyCB(unsigned char key, int x, int y)	/* called on key press */
{
  if( key == 'q' ) exit(0);
}


int main(int argc, char *argv[])
{
  int win;

  glutInit(&argc, argv);		/* initialize GLUT system */

  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(400,500);		/* width=400pixels height=500pixels */
  win = glutCreateWindow("Triangle");	/* create window */

  /* from this point on the current window is win */

  glClearColor(0.0,0.0,0.0,0.0);	/* set background to black */
  gluOrtho2D(0,400,0,500);		/* how object is mapped to window */
  glutDisplayFunc(displayCB);		/* set window's display callback */
  glutKeyboardFunc(keyCB);		/* set window's key callback */

  glutMainLoop();			/* start processing events... */

  /* execution never reaches this point */

  return 0;
}
