/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

*/

#include "jello.h"
#include "showCube.h"

void showBoundingBox()
{	
  glBegin(GL_QUADS);
	
  for (int i=-10; i<10; i++) {
	  for (int j=10; j>-10; j--) {
		  if ((i+j)%2) {
			  glColor4f(0.0,0.0,0.0,0);
			  glVertex3f(i,-2,j);
			  glVertex3f(i+1,-2,j);
			  glVertex3f(i+1,-2,j-1);
			  glVertex3f(i,-2,j);			  
		  }else {
			  glColor4f(1.0,1.0,1.0,0);
			  glVertex3f(i,-2,j);
			  glVertex3f(i+1,-2,j);
			  glVertex3f(i+1,-2,j-1);
			  glVertex3f(i,-2,j);	
		  }
	  }
  }	
  glEnd();

  return;
}

void showCircle()
{
	float r = 0.5, num_segments = 300;
    glColor4f(0.0,0.0,1.0,0);
	glBegin(GL_LINE_LOOP); 
	for(int i = 0; i < num_segments; i++) 
	{ 
		
		float theta = 2.0f * pi * float(i) / float(num_segments);//get the current angle 
		
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		
		glVertex3f(x, y, 0);//output vertex 
	} 
	glEnd(); 	
	
	//draw the stick of circle
	glBegin(GL_LINES);
	glVertex3f(0, -2, 0);
	glVertex3f(0, -0.5, 0);
	glEnd();
	
	return;
}
	
void showChain(struct world * jello)
{
	
	glPointSize(5);
	glLineWidth(1);
	glDisable(GL_LIGHTING);
	for (int i = 0; i < particleNum ; i++) {
		glBegin(GL_POINTS); // draw point
		glColor4f(1,0,0,0);  
		glVertex3f(jello->p[i].x, jello->p[i].y, jello->p[i].z);
		//printf("%d.x:%f  %d.y:%f  %d.z:%f\n",i,jello->p[i][0][0].x,i,jello->p[i][0][0].y,i,jello->p[i][0][0].z);
		glEnd();
	}
 
	glColor4f(0,1,0,0);
	glBegin(GL_LINE_STRIP); 
	for (int i = 0; i < particleNum; i++) {
		glVertex3f(jello->p[i].x, jello->p[i].y, jello->p[i].z);
	}
	glEnd();
	 
	glEnable(GL_LIGHTING);
	return;
}

void showText(struct world * jello)
{
	printf("Force x:%g, y:%g, z:0\nAlpha:%g\nBeta:%g\n", jello->Force.x, jello->Force.y, alpha,beta);
	if (PNoise == 1) {
		printf("PerlinNoise:ON\n");
	}else {
		printf("PerlinNoise:OFF\n");			
	}
	
	if (CRING == 1) {
		printf("Last Constrain:ON\n\n\n");
	}else {
		printf("Last Constrain:OFF\n\n\n");
	}
	
}
