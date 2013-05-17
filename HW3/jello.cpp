/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

  Your name:
  <write your name here>

*/

#include "jello.h"
#include "showCube.h"
#include "input.h"
#include "physics.h"
#include "perlinNoise.h"
#include <stdlib.h>
#include <time.h>

// camera parameters
double Theta = pi / 6;
double Phi = pi / 6;
double R = 6;

// mouse control
int g_iMenuId;
int g_vMousePos[2];
int g_iLeftMouseButton,g_iMiddleMouseButton,g_iRightMouseButton;

// number of images saved to disk so far
int sprite=0;

//value of particles
int particleNum;
double particleMass;
double particleLength;
char *Integration;

double constraintsError = 0;

bool CRING = 0;

double alpha, beta;

// these variables control what is displayed on screen
int shear=0, bend=0, structural=1, pause=0, viewingMode=0, saveScreenToFile=0, Fx=0, Fy=0, Fa=0, Fb=0, PNoise = 0;

struct world jello;

int windowWidth, windowHeight;

void myinit()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0,1.0,0.01,1000.0);

  // set background color to grey
  glClearColor(0.5, 0.5, 0.5, 0.0);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  return; 
}

void reshape(int w, int h) 
{
  // Prevent a divide by zero, when h is zero.
  // You can't make a window of zero height.
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Set the perspective
  double aspectRatio = 1.0 * w / h;
  gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 

  windowWidth = w;
  windowHeight = h;

  glutPostRedisplay();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // camera parameters are Phi, Theta, R
  gluLookAt(R * cos(Phi) * cos (Theta), R * sin (Theta), R * sin(Phi) * cos (Theta),
	        0.0,0.0,0.0, 0.0,1.0,0.0);
	

  /* Lighting */
  /* You are encouraged to change lighting parameters or make improvements/modifications
     to the lighting model . 
     This way, you will personalize your assignment and your assignment will stick out. 
  */

  // global ambient light
  GLfloat aGa[] = { 0.0, 0.0, 0.0, 0.0 };
  
  // light 's ambient, diffuse, specular
  GLfloat lKa0[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd0[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lKs0[] = { 1.0, 1.0, 1.0, 1.0 };

  GLfloat lKa1[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd1[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat lKs1[] = { 1.0, 0.0, 0.0, 1.0 };

  GLfloat lKa2[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd2[] = { 1.0, 1.0, 0.0, 1.0 };
  GLfloat lKs2[] = { 1.0, 1.0, 0.0, 1.0 };

  GLfloat lKa3[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd3[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat lKs3[] = { 0.0, 1.0, 1.0, 1.0 };

  GLfloat lKa4[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd4[] = { 0.0, 0.0, 1.0, 1.0 };
  GLfloat lKs4[] = { 0.0, 0.0, 1.0, 1.0 };

  GLfloat lKa5[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd5[] = { 1.0, 0.0, 1.0, 1.0 };
  GLfloat lKs5[] = { 1.0, 0.0, 1.0, 1.0 };

  GLfloat lKa6[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd6[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lKs6[] = { 1.0, 1.0, 1.0, 1.0 };

  GLfloat lKa7[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd7[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat lKs7[] = { 0.0, 1.0, 1.0, 1.0 };

  // light positions and directions
  GLfloat lP0[] = { -1.999, -1.999, -1.999, 1.0 };
  GLfloat lP1[] = { 1.999, -1.999, -1.999, 1.0 };
  GLfloat lP2[] = { 1.999, 1.999, -1.999, 1.0 };
  GLfloat lP3[] = { -1.999, 1.999, -1.999, 1.0 };
  GLfloat lP4[] = { -1.999, -1.999, 1.999, 1.0 };
  GLfloat lP5[] = { 1.999, -1.999, 1.999, 1.0 };
  GLfloat lP6[] = { 1.999, 1.999, 1.999, 1.0 };
  GLfloat lP7[] = { -1.999, 1.999, 1.999, 1.0 };
  
  // jelly material color

  GLfloat mKa[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mKd[] = { 0.3, 0.3, 0.3, 1.0 };
  GLfloat mKs[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mKe[] = { 0.0, 0.0, 0.0, 1.0 };

  /* set up lighting */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // set up cube color
  glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
  glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
  glMaterialf(GL_FRONT, GL_SHININESS, 120);
    
  // macro to set up light i
  #define LIGHTSETUP(i)\
  glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
  glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
  glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
  glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
  glEnable(GL_LIGHT##i)
  
  LIGHTSETUP (0);
  LIGHTSETUP (1);
  LIGHTSETUP (2);
  LIGHTSETUP (3);
  LIGHTSETUP (4);
  LIGHTSETUP (5);
  LIGHTSETUP (6);
  LIGHTSETUP (7);

  // enable lighting
  glEnable(GL_LIGHTING);    
  glEnable(GL_DEPTH_TEST);

  // show the cube
  //showCube(&jello);

  glDisable(GL_LIGHTING);

  // show the bounding box
  showBoundingBox();

  // show Circle and Chain
  showCircle();
  showChain(&jello);
	
  showText(&jello);
 
  glutSwapBuffers();
}

void doIdle()
{
  char s[20]="picxxxx.ppm";
  int i;
  
  // save screen to file
  s[3] = 48 + (sprite / 1000);
  s[4] = 48 + (sprite % 1000) / 100;
  s[5] = 48 + (sprite % 100 ) / 10;
  s[6] = 48 + sprite % 10;

  if (saveScreenToFile==1)
  {
    saveScreenshot(windowWidth, windowHeight, s);
    //saveScreenToFile=0; // save only once, change this if you want continuos image generation (i.e. animation)
    sprite++;
  }

  if (sprite >= 300) // allow only 300 snapshots
  {
    exit(0);	
  }

  if (pause == 0)
  {
    // insert code which appropriately performs one step of the cube simulation:
	  if (Integration[0] == 'E') {
		   Euler(&jello);
	  }else if (Integration[0] == 'R') {
		  RK4(&jello);
	  }else {
		  exit(0);
	  }

  }
	
	if (Fx == 1) {
		jello.Force.x += 1;
		Fx = 0;
	}
	if (Fx == -1) {
		jello.Force.x -= 1;
		Fx = 0;
	}
	if (Fy == 1) {
		jello.Force.y += 1;
		Fy = 0;
	}
	if (Fy == -1) {
		jello.Force.y -= 1;
		Fy = 0;
	}
	if (Fa == 1) {
		alpha += 1;
		Fa = 0;
	}
	if (Fa == -1) {
		if (alpha != 0) {
			alpha -= 1;
			Fa = 0;
		}
	}
	if (Fb == 1) {
		beta += 0.1;
		Fb = 0;
	}
	if (Fb == -1) {
		beta -= 0.1;
		Fb = 0;
	}
	if (PNoise == -1) {
		jello.Force.x = 0;
		jello.Force.y = -1;
		PNoise = 0;
		for (int i=0; i<particleNum; i++) {
			jello.v[i].x = 0;
			jello.v[i].y = 0;
			jello.v[i].z = 0;
			jello.a[i].x = 0;
			jello.a[i].y = 0;
			jello.a[i].z = 0;
		}
	}	
	if (PNoise == 1) {
		srand(time(NULL));
		PerlinNoise P;
		if (alpha == 0) {
			jello.Force.x = P.PerlinNoise1D(rand()%100, particleNum , 0.3);
			jello.Force.y = P.PerlinNoise1D(rand()%100, particleNum , 0.3);
		}else {
			jello.Force.x = P.PerlinNoise1D(rand()%100, particleNum , 0.3) * 20;
			jello.Force.y = P.PerlinNoise1D(rand()%100, particleNum , 0.3) * 20;
		}
	}
		
	//reset everything
	if (Fx == -5) {
		for (int i=0; i<particleNum; i++) {
			jello.v[i].x = 0;
			jello.v[i].y = 0;
			jello.v[i].z = 0;
			jello.a[i].x = 0;
			jello.a[i].y = 0;
			jello.a[i].z = 0;
		}
		jello.Force.x = 0;
		jello.Force.y = -1;
		alpha = 0;
		beta = 0;
		PNoise = 0;
		Fx = 0;
	}
	

  glutPostRedisplay();
}

int main (int argc, char ** argv)
{
  if (argc<6)
  {  
    printf ("Oops! You didn't say the number of particles!\n");
    printf ("Usage: %s [Euler or RK4] [number of particles] [alpha value] [beta value] [Able CRING?]\n", argv[0]);
    exit(0);
  }

	Integration = argv[1];
	particleNum = atoi(argv[2]);
	alpha = atof(argv[3]);
	beta = atof(argv[4]);
	CRING = atoi(argv[5]);
	particleMass = 1.0/particleNum;
	particleLength = 1.0/(particleNum-1);
	jello.dt = 0.0003;
	jello.Force.x = 0;
	jello.Force.y = -1;
	jello.Force.z = 0;
	
	//input value check
	if (Integration[0] != 'R' && Integration[0] != 'E') {
		printf ("Oops! You should set Euler or RK4!\n");
		exit(0);
	}
	
	if (particleNum >= 500) {
		printf ("Oops! You set too many particles!\n");
		exit(0);
	}
	
	if (!CRING) {
		//horizontal position
		for (int i=0; i<particleNum; i++) {
			jello.p[i].x = float(i) * particleLength;
			jello.p[i].y = 0.5;
			jello.p[i].z = 0;
			jello.v[i].x = 0;
			jello.v[i].y = 0;
			jello.v[i].z = 0;
			jello.a[i].x = 0;
			jello.a[i].y = 0;
			jello.a[i].z = 0;
		}
	}else {
		//3 o'clock setting
		int middle = (particleNum-1)/2;
		for (int i=0 ; i<particleNum; i++) {
			if (particleNum%2) {
				if (i == middle) {
					jello.p[i].x = 0;
					jello.p[i].y = 0;
					jello.p[i].z = 0;
					jello.v[i].x = 0;
					jello.v[i].y = 0;
					jello.v[i].z = 0;
					jello.a[i].x = 0;
					jello.a[i].y = 0;
					jello.a[i].z = 0;
				}else if (i<middle) {
					jello.p[i].x = 0;
					jello.p[i].y = 0.5 - float(i) * particleLength;
					jello.p[i].z = 0;
					jello.v[i].x = 0;
					jello.v[i].y = 0;
					jello.v[i].z = 0;
					jello.a[i].x = 0;
					jello.a[i].y = 0;
					jello.a[i].z = 0;
				}else {
					jello.p[i].x = 0.5 - float(particleNum - i -1) * particleLength;
					jello.p[i].y = 0;
					jello.p[i].z = 0;
					jello.v[i].x = 0;
					jello.v[i].y = 0;
					jello.v[i].z = 0;
					jello.a[i].x = 0;
					jello.a[i].y = 0;
					jello.a[i].z = 0;
				}
			}else {
				if (i<=middle) {
					jello.p[i].x = 0;
					jello.p[i].y = 0.5 - float(i) * particleLength;
					jello.p[i].z = 0;
					jello.v[i].x = 0;
					jello.v[i].y = 0;
					jello.v[i].z = 0;
					jello.a[i].x = 0;
					jello.a[i].y = 0;
					jello.a[i].z = 0;
				}else {
					jello.p[i].x = 0.5 - float(particleNum - i -1) * particleLength;
					jello.p[i].y = 0;
					jello.p[i].z = 0;
					jello.v[i].x = 0;
					jello.v[i].y = 0;
					jello.v[i].z = 0;
					jello.a[i].x = 0;
					jello.a[i].y = 0;
					jello.a[i].z = 0;
				}			
			}
		}		
	}

	/*
	 //6 o'clock setting
	 for (int i=0 ; i<particleNum; i++) {
	 jello.p[i].x = 0;
	 jello.p[i].y = 0.5 - float(i) / (particleNum-1);
	 jello.p[i].z = 0;
	 jello.v[i].x = 0;
	 jello.v[i].y = 0;
	 jello.v[i].z = 0;
	 jello.a[i].x = 0;
	 jello.a[i].y = 0;
	 jello.a[i].z = 0;
	 }
	 */
	
	
	
  glutInit(&argc,argv);
  
  /* double buffered window, use depth testing, 640x480 */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  windowWidth = 640;
  windowHeight = 480;
  glutInitWindowSize (windowWidth, windowHeight);
  glutInitWindowPosition (0,0);
  glutCreateWindow ("Jello cube");

  /* tells glut to use a particular display function to redraw */
  glutDisplayFunc(display);

  /* replace with any animate code */
  glutIdleFunc(doIdle);

  /* callback for mouse drags */
  glutMotionFunc(mouseMotionDrag);

  /* callback for window size changes */
  glutReshapeFunc(reshape);

  /* callback for mouse movement */
  glutPassiveMotionFunc(mouseMotion);

  /* callback for mouse button changes */
  glutMouseFunc(mouseButton);

  /* register for keyboard events */
  glutKeyboardFunc(keyboardFunc);

  /* do initialization */
  myinit();

  /* forever sink in the black hole */
  glutMainLoop();

  return(0);
}

