//
//  main.cpp
//	Assignment 5: Lighting
//
//  Created by Jacob Rail on 10/9/13.
//  Copyright (c) 2013 Jacob Rail. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Pieces.h"
#include "XWing.h"
#define GLGLEXR_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

// -------------------------------------------------------------------------- //
// ---------------------------- Global Variables ---------------------------- //
int th = 0;
int ph = 0;
double asp = 1;
bool mode = false;
double dim = 60;
double fov = 60;
bool displayAxis = true;
bool info = true;
char demo;
double WHITE = 0.6;
double BLACK = 0.1;
bool light = true;      //  Lighting

// Light values
int one       =   1;  // Unit value
double lightDistance  =   20;  // Light distance
/*
 int inc       =  10;  // Ball increment
 int smooth    =   1;  // Smooth/Flat shading
 int local     =   0;  // Local Viewer Model
 int emission  =   0;  // Emission intensity (%)
 */
double ambient   =  30;  // Ambient intensity (%)
double diffuse   = 100;  // Diffuse intensity (%)
double specular  =   0;  // Specular intensity (%)
/*
 int shininess =   0;  // Shininess (power of two)
 float shinyvec[1];    // Shininess (value)
 */
int zh        =  90;  // Light azimuth
float ylight  =   5;  // Elevation of light

// -------------------------------------------------------------------------- //
// ------------------------------- Prototypes ------------------------------- //
void display();
void key(unsigned char, int, int);
void special(int, int, int);
void Print(const char* formati, ...);
void DisplayAxes();
static void project();
void setUpBoard();
void printInfo();
void Demos();
void reshape(int width, int height);
void idle();
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

int main(int argc, char * argv[])
{
	// initialize GLUT and process user parameters
	glutInit(&argc, argv);
	
	//
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	// window title
	glutCreateWindow("Jacob R: Projections");
	
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	// initialize the screen
	project();
	glutMainLoop();
	
    return 0;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
	//  Elapsed time in seconds
	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	zh = fmod(90*t,360.0);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

// -------------------------------------------------------------------------- //

void reshape(int width, int height){
	
	//  Ratio of the width to the height of the window
	double w2h = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Orthogonal projection
	glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

// -------------------------------------------------------------------------- //
/*
 *  Draw vertex in polar coordinates with normal
 */
/*
 static void Vertex(double th,double ph)
 {
 double x = Sin(th)*Cos(ph);
 double y = Cos(th)*Cos(ph);
 double z =         Sin(ph);
 //  For a sphere at the origin, the position
 //  and normal vectors are the same
 glNormal3d(x,y,z);
 glVertex3d(x,y,z);
 }*/

static void ball(double x,double y,double z,double r)
{
	int th,ph;
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,static_cast<float>(0.01*emission),1.0};
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r,r,r);
	//  White ball
	glColor3f(1,1,1);
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	//  Bands of latitude
	for (ph=-90;ph<90;ph+=inc)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=2*inc)
		{
			Vertex(th,ph);
			Vertex(th,ph+inc);
		}
		glEnd();
	}
	//  Undo transofrmations
	glPopMatrix();
}



// -------------------------------------------------------------------------- //


void display(){
	// Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	// Undo previous transformation(s)
	glLoadIdentity();
	
	
	// Perspective - set eye position
	if (mode) {
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim*		   Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
	}
	// Orthogonal - set world orientation
	else {
		glRotated(ph, 1, 0, 0);
		glRotated(th, 0, 1, 0);
	}
	
	
	//  Flat or smooth shading
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
	
	
	if(light){
		//  Translate intensity to color vectors
        GLfloat Ambient[]   = {static_cast<GLfloat>(0.01*ambient) ,static_cast<GLfloat>(0.01*ambient) ,static_cast<GLfloat>(0.01*ambient) ,1.0};
        GLfloat Diffuse[]   = {static_cast<GLfloat>(0.01*diffuse) ,static_cast<GLfloat>(0.01*diffuse) ,static_cast<GLfloat>(0.01*diffuse) ,1.0};
        GLfloat Specular[]  = {static_cast<GLfloat>(0.01*specular),static_cast<GLfloat>(0.01*specular),static_cast<GLfloat>(0.01*specular),1.0};
		
		
		//  Light position
        //double Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        GLfloat Position[] = {static_cast<GLfloat>(lightDistance*Cos(zh)), ylight, static_cast<GLfloat>(lightDistance*Sin(zh)), 1.0};
		//  Draw light position as ball (still no lighting here)
        glColor3f(1,1,0);
        ball(Position[0],Position[1],Position[2] , 1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
		
        glLightfv(GL_LIGHT0, GL_AMBIENT , Ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE , Diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
        glLightfv(GL_LIGHT0, GL_POSITION, Position);
		
	}
	else
		glDisable(GL_LIGHTING);
	
	
	/*** Draw scene ***/
	/*** Draw chess board ***/
	
	
	 double tileWidth = 10;
	 double offsetWhite = 10;
	 double offsetBlack = 0;
	 double offsetTemp = 0;
	/*
	// front
	glColor3f(0, 1, 1);
	glNormal3f(0, 0, 5);
	glVertex3d(-5, 10, 5);
	glVertex3d(5, 10, 5);
	glVertex3d(5, 20, 5);
	glVertex3d(-5, 20, 5);
	// left
	glColor3f(1, 0, 0);
	glNormal3f(-5, 0, 0);
	glVertex3d(-5, 10, 5);
	glVertex3d(-5, 10, -5);
	glVertex3d(-5, 20, -5);
	glVertex3d(-5, 20, 5);
	// back
	glColor3f(0, 1, 0);
	glNormal3f(0, 0, -5);
	glVertex3d(-5, 10, -5);
	glVertex3d(5, 10, -5);
	glVertex3d(5, 20, -5);
	glVertex3d(-5, 20, -5);
	// right
	glColor3f(1, 1, 0);
	glNormal3f(5, 0, 0);
	glVertex3d(5, 10, -5);
	glVertex3d(5, 10, 5);
	glVertex3d(5, 20, 5);
	glVertex3d(5, 20, -5);
	// bottom
	glNormal3f(0, 10, 0);
	glColor3f(0, 0, 1);
	glVertex3d(-5, 10, 5);
	glVertex3d(5, 10, 5);
	glVertex3d(5, 10, -5);
	glVertex3d(-5, 10, -5);
	// top
	glColor3f(1, 0, 1);
	glNormal3f(0, 20, 0);
	glVertex3d(-5, 20, 5);
	glVertex3d(5, 20, 5);
	glVertex3d(5, 20, -5);
	glVertex3d(-5, 20, -5);
	*/
	
	// middle test square, centered at origin.
	/*
	 glColor3f(0, 1, 1);
	 glVertex3d(-5, 0.0001, 5);
	 glVertex3d(5, 0.0001, 5);
	 glVertex3d(5, 0.0001, -5);
	 glVertex3d(-5, 0.0001, -5);
	 */
	
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	
	for (int i = -40; i < 40; i += 10) {
		for (int j = -40; j < 40; j += 20) {
			glColor3f(1, 1, 1);
			glVertex3d(j + offsetBlack,				0, i);
			glVertex3d(j + tileWidth + offsetBlack, 0, i);
			glVertex3d(j + tileWidth + offsetBlack, 0, i + tileWidth);
			glVertex3d(j + offsetBlack,				0, i + tileWidth);
			
			glColor3f(0.1, 0.1, 0.1);
			glVertex3d(j + offsetWhite,				0, i);
			glVertex3d(j + tileWidth + offsetWhite, 0, i);
			glVertex3d(j + tileWidth + offsetWhite, 0, i + tileWidth);
			glVertex3d(j + offsetWhite,				0, i + tileWidth);
		}
			
		offsetTemp = offsetWhite;
		offsetWhite = offsetBlack;
		offsetBlack = offsetTemp;
	}
	
	glEnd();
	
	/*** Draw scene ***/
	setUpBoard();
	Demos();
	/*** End scene  ***/
	
	glDisable(GL_LIGHTING);
	// display axes
	DisplayAxes();
	
	// print onscreen info
	printInfo();
	
	glFlush();
	glutSwapBuffers();
}

// -------------------------------------------------------------------------- //

void key(unsigned char ch, int x, int y){
	// exit the program when the Esc Key is pressed
	if (ch == 27) {
		exit(0);
	}
	else if (ch == 'm' || ch == 'M')
		mode = !mode;
	else if (ch == 'w' || ch == 'W'){
		if (!mode) // ortho
			++dim;
		else
			++fov;
	}
	else if (ch == 'q' || ch == 'Q'){
		if (!mode)
			--dim;
		else
			--fov;
	}
	else if (ch == 'a' || ch == 'A')
		displayAxis = !displayAxis;
	else if (ch == 'x')
		demo = 'x';
	else if (ch == 'X')
		demo = 'X';
	else if (ch == '`' || ch == '~')
		demo = '~';
	else if (ch == 'p')
		demo = 'p';
	else if (ch == 'P')
		demo = 'P';
	else if (ch == 'r')
		demo = 'r';
	else if (ch == 'R')
		demo = 'R';
	else if (ch == 'k')
		demo = 'k';
	else if (ch == 'K')
		demo = 'K';
	else if (ch == '?')
		info = !info;
	
	// Reproject
	project();
	// tell GLUT to redisplay the scene
	glutPostRedisplay();
}

// -------------------------------------------------------------------------- //

void special(int key, int x, int y){
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	
	// Reproject
	project();
	
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

// -------------------------------------------------------------------------- //

static void project(){
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective transformation
	if (mode){
		gluPerspective(fov,asp,dim/4,4*dim);
	}
	//  Orthogonal projection
	else
		glOrtho(-dim, dim, -dim, +dim, -dim*3, +dim*3);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

// -------------------------------------------------------------------------- //

void DisplayAxes(){
	
	if (displayAxis) {
		// draw axes
		glColor3f(0, 1, 0);			// make axes green
		glBegin(GL_LINES);
		glVertex3d(0, 0, 0);
		glVertex3d(dim/2, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, dim/2, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, dim/2);
		
		
		glColor3f(1, 0, 0);			// make axes red
		glVertex3d(0, 0, 0);
		glVertex3d(-dim/2, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, -dim/2, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, -dim/2);
		glEnd();
		
		// Label axes
		glColor3f(0, 1, 0);			// lable + axes green
		glRasterPos3d(dim/2, 0, 0);
		Print("X");
		glRasterPos3d(0, dim/2, 0);
		Print("Y");
		glRasterPos3d(0, 0, dim/2);
		Print("Z");
		
		glColor3f(1, 0, 0);			// lable - axes red
		glRasterPos3d(-dim/2, 0, 0);
		Print("-x");
		glRasterPos3d(0, -dim/2, 0);
		Print("-y");
		glRasterPos3d(0, 0, -dim/2);
		Print("-z");
	}
}

// -------------------------------------------------------------------------- //

#define LEN 8192
void Print(const char* format, ...){
	char buf[LEN];
	char* ch = buf;
	va_list args;
	
	va_start(args, format);
	vsnprintf(buf, LEN, format, args);
	va_end(args);
	
	while (*ch) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
	}
}

// -------------------------------------------------------------------------- //

void setUpBoard(){
	
	Pawn pw(WHITE);
	Rook rw(WHITE);
	
	Pawn pb(BLACK);
	Rook rb(BLACK);
	
	King kw(WHITE);
	King kb(BLACK);
	
	int i = -35;
	for (; i <= 40; i += 10) {
		// draw white
		pw.draw(i, 25);
		// draw black
		pb.draw(i, -25);
	}
	
	// draw white
	rw.draw(-35, 35);
	rw.draw(35, 35);
	
	// draw black
	rb.draw(-35, -35);
	rb.draw(35, -35);
	
	XWing xww(WHITE);
	XWing xwb(BLACK);
	// draw white xwing fighters as bishops
	xww.draw(0.20, -15, -35, 'w');
	xww.draw(0.20, 15, -35, 'w');
	
	// draw black xwing fighters as bishops
	xwb.draw(0.20, -15, 35, 'b');
	xwb.draw(0.20, 15, 35, 'b');
	
	// draw kings
	kw.draw(5, 35);
	kb.draw(5, -35);
}

// -------------------------------------------------------------------------- //
void printInfo(){
	double scale;
	if(mode){
		scale = fov;
	}
	else
		scale = dim;
	glColor3f(1, 1, 1);
	if (info) {
		glWindowPos2d(5, 5);
		Print("Angle = %d, mode = %d, FOV = %d, Scale = %0.1f\n",
			  th, ph, mode, scale);
		glWindowPos2d(5, 25);
		Print("Use ArroKeys to rotate camera");
		glWindowPos2d(5, 45);
		Print("toggle orthoginal and perspective with m/M");
		glWindowPos2d(5, 65);
		Print("q/w zoom in/out");
		glWindowPos2d(5, 85);
		Print("toggle axis: a/A");
	}
	else{
		glWindowPos2d(5, 65);
		Print("To demo a piece in the center of the board\n");
		glWindowPos2d(5, 45);
		Print("hit 'p' for pawn, 'r' for rook, or 'x' for xwing.\n");
		glWindowPos2d(5, 25);
		Print("Lower case latter will spawn the piece as WHITE,\n");
		glWindowPos2d(5, 5);
		Print("uppercase will span as BLACK. Hit '~' to stop demos.");
	}
	glWindowPos2d(5, 105);
	Print("for additional info hit '?'");
	
}

// -------------------------------------------------------------------------- //
void Demos(){
	if (demo == 'x') {
		XWing demoWhite(WHITE);
		demoWhite.draw(0.5, 0, 0, 'w');
		return;
	}
	else if (demo == 'X'){
		XWing demoBlack (BLACK);
		demoBlack.draw(0.5, 0, 0, 'b');
		return;
	}
	
	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);
	if (demo == 'r'){
		Rook demoWhiteRook(WHITE);
		demoWhiteRook.draw(0, 0);
	}
	else if (demo == 'R'){
		Rook demoBlackRook(BLACK);
		demoBlackRook.draw(0, 0);
	}
	else if (demo == 'p'){
		Pawn demoWhitePawn(WHITE);
		demoWhitePawn.draw(0, 0);
	}
	else if (demo == 'P'){
		Pawn demoBlackPawn(BLACK);
		demoBlackPawn.draw(0, 0);
	}
	else if (demo == 'k'){
		King demoWhiteKing(WHITE);
		demoWhiteKing.draw(0, 0);
	}
	else if (demo == 'K'){
		King demoBlackKing(BLACK);
		demoBlackKing.draw(0,0);
	}
	else if (demo == '~')
		demo = '~';
	glPopMatrix();
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //











