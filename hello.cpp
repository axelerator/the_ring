// Includes

#ifdef WIN32																// If We're Under MSVC
#include <windows.h>														// We Need The Windows Header
#else																		// Otherwhise
#include <stdio.h>															// We're Including The Standard IO Header
#include <stdlib.h>															// And The Standard Lib Header
#include <string.h>															// And The String Lib Header
#endif

#include <GL/gl.h>															// We're Including The OpenGL Header
#include <math.h>												// We Require The Math Lib For Sin and Cos
#include <GL/glu.h>		
#include "SDL/SDL.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 8

SDL_Surface *Screen;

float		angle = 0.0;												// Used To Rotate The Triangles
float		cnt1 = 0.0 , cnt2 = 0.0;											// Used To Move The Object On The Screen
float		Lx = 0.0, Ly = 0.0;												// We use all this variables to hold
Sint16		Ix = 0, Iy = 0;												// Values used for moving/rotating figures
int			rot1 = 0, rot2 = 0;											// Counter Variablesk
bool createWindowGL(int W, int H, int B, Uint32 F) {
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );								// In order to use SDL_OPENGLBLIT we have to
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );							// set GL attributes first
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );							// colors and doublebuffering

	if(!(Screen = SDL_SetVideoMode(W, H, B, F)))							// We're Using SDL_SetVideoMode To Create The Window
	{
		return false;														// If It Fails, We're Returning False
	}

	SDL_FillRect(Screen, NULL, SDL_MapRGBA(Screen->format,0,0,0,0));		// A key event! God knows all the time I've spent just to
																			// figure out this basic thing. We have to open the Screen Alpha Channel!

	//ReshapeGL(SCREEN_W, SCREEN_H);											// We're Calling Reshape As The Window Is Created

	return true;															// Return TRUE (Initialization Successful)

}
bool initGL(SDL_Surface *S)										// Any OpenGL Initialization Code Goes Here
{
	glClearColor(0.0f,0.0f,0.0f,0.5f);							// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel(GL_SMOOTH);									// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	return true;												// Return TRUE (Initialization Successful)
}


void reshapeGL(int width, int height)										// Reshape The Window When It's Moved Or Resized
{
	glViewport(0,0,(GLsizei)(width),(GLsizei)(height));						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
	glLoadIdentity();														// Reset The Projection Matrix */

	gluPerspective(45.0f,(GLfloat)(width)/(GLfloat)(height),1.0f,100.0f);	// Calculate The Aspect Ratio Of The Window
	glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
	glLoadIdentity();														// Reset The Modelview Matrix

	return;																	// Always Return, We're Standard :)
}

int main(int argc, char *argv[]) {
     SDL_Surface *screen;
     Uint8       *p;
     int         x = 10; //x coordinate of our pixel
     int         y = 20; //y coordinate of our pixel
     
     /* Initialize SDL */
     SDL_Init(SDL_INIT_VIDEO);
     
     /* Initialize the screen / window */
     screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
     /* Make p point to the place we want to draw the pixel */
     p = (Uint8 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
     
     /* Draw the pixel! */
     //*p=0xff;
     
     /* update the screen (aka double buffering) */
     //SDL_Flip(screen);

		Uint32 vflags = SDL_HWSURFACE|SDL_OPENGLBLIT;
		//SDL_Surface *screen;

		createWindowGL(SCREEN_WIDTH, SCREEN_HEIGHT, 16, vflags);
		reshapeGL(SCREEN_WIDTH, SCREEN_WIDTH);
		initGL(screen);
		SDL_Event	E;															// And Event Used In The Polling Process
		bool running = true;
     while(running) {
				if(SDL_PollEvent(&E)) {
					switch(E.type) {
						case SDL_KEYDOWN:	
							running = false;
					}
				}
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix
	glTranslatef(float(cos(cnt1)), float(sin(cnt2)), -6.0f);	// Translate 6 Units Into The Screen And Use cnt1 And cnt2 To Move The Object
	glRotatef(angle, 0.0f, 1.0f, 0.0f);							// Rotate On The Y-Axis By angle

	for (rot1=0; rot1<2; rot1++)								// 2 Passes
	{
		glRotatef(90.0f,0.0f,1.0f,0.0f);						// Rotate 90 Degrees On The Y-Axis
		glRotatef(180.0f,1.0f,0.0f,0.0f);						// Rotate 180 Degrees On The X-Axis

		for (rot2=0; rot2<2; rot2++)							// 2 Passes
		{
			glRotatef(180.0f,0.0f,1.0f,0.0f);					// Rotate 180 Degrees On The Y-Axis
			glBegin(GL_TRIANGLES);								// Begin Drawing Triangles
				glColor3f (1.f, 0.f, 0.f);						// 1st Color (Red) ...
					glVertex3f( 0.0f, 1.0f, 0.0f);				// ...For The 1st Vertex
				glColor3f (0.f, 1.f, 0.f);						// 2nd Color (Green) ...
					glVertex3f(-1.0f,-1.0f, 1.0f);				// ...For The 2nd Vertex
				glColor3f (0.f, 0.f, 1.f);						// And 3rd Color (Blue) ...
					glVertex3f( 1.0f,-1.0f, 1.0f);				// ...For The 3rd Vertex
			glEnd();											// Done Drawing Triangles
		}
	}
	cnt1 += 0.0001;
	glFlush();													// Flush The GL Rendering Pipelines


				SDL_GL_SwapBuffers();										// And Swap The Buffers (We're Double-Buffering, Remember?)
		}
}


