// Includes

#ifdef WIN32																// If We're Under MSVC
#include <windows.h>														// We Need The Windows Header
#else																		// Otherwhise
#include <stdio.h>															// We're Including The Standard IO Header
#include <stdlib.h>															// And The Standard Lib Header
#include <string.h>															// And The String Lib Header
#include <iostream>
#endif

#include <GL/gl.h>															// We're Including The OpenGL Header
#include <math.h>												// We Require The Math Lib For Sin and Cos
#include <GL/glu.h>		
#include "SDL/SDL.h"

#include "cylinder.h"
#include "camera.h"
#include "plane.h"
#include "helper.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 786
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
  glClearColor(0.0f,0.0f,0.0f,0.0f);							// Black Background
  glClearDepth(1.0f);											// Depth Buffer Setup
  glDepthFunc(GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
  glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
  glShadeModel(GL_SMOOTH);									// Select Smooth Shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
  GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel (GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_SPECULAR, black);
  glMaterialfv(GL_FRONT, GL_SHININESS, black);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT0, GL_AMBIENT, black);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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

Cylinder **mkCylinders(int cw, int ch, float seed) {
  int c_count = cw * ch;
  Vec3 p1(0.1, 0.05, 0.1);
  Cylinder **cs = (Cylinder**)malloc(c_count*sizeof(Cylinder*));
  float radius = 1.0 / cw;
  for (int i = 0; i < ch; ++i) {
    for (int t = 0; t < cw; ++t) {
      float height = (cos((3.17*i)/cw + (0.003 * seed)) * sin((3.17*t)/cw + (0.0023 * seed)))* 0.2 + 0.3;
      p1[0] = -0.5 + (1.0/ch)*i + radius * 0.5;
      p1[1] = height * 0.5;
      p1[2] = -0.5 + (1.0/cw) * t + radius * 0.5;
      cs[i*cw+t] = new Cylinder(p1, height, radius * 0.7, 2, 16 ); //(p, 0.1f, (1.0/cw), 2, 10 );
     }
  } 
  return cs;
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
  float ani_u = 0.0;
  float ani_v = 0.0;
  Camera cam;

  int cw = 17;
  int ch = 17;
  int c_count = cw * ch;
  Cylinder **cs = mkCylinders(cw,ch, ani_u);
  const Vec3 origin(0.0);
  Plane plane(1.0, 1.0, origin, 70, 70);
  bool running = true;
  std::cout << "starting main loop" << std::endl;
  //int frame = 0;
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  bool anim = false;
  Helper helper;
  while(running) {
    if(SDL_PollEvent(&E)) {
      switch(E.type) {
        case SDL_KEYDOWN:	
          switch ( E.key.keysym.sym ) {
            case SDLK_ESCAPE:
              running = false;
              break;
            default:
              anim = !anim;
          }
      }
    }
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
    glLoadIdentity();											// Reset The Modelview Matrix
    //std::cout << "frame" << (frame++) << std::endl;
    cam.setLookAt();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    plane.draw();
    //cylinder.draw();
    for (int i = 0; i < c_count; ++i) {
      delete cs[i];
    }
    cs = mkCylinders(cw,ch, ani_u);
    for (int i = 0; i < ch; ++i) {
      for (int t = 0; t < cw; ++t) {
        cs[i*cw+t]->draw();
      }
    }
    helper.draw();
    //ccc->draw();
    if (anim) {
      ani_u += 1.0;
      ani_v += 0.00214;
      cam.animate(0.01);
    }
    glFlush();													// Flush The GL Rendering Pipelines
    SDL_GL_SwapBuffers();										// And Swap The Buffers (We're Double-Buffering, Remember?)
  }
  return 0;
}


