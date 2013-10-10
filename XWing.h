//
//  XWing.h
//  Assignment 4: Projections
//
//  Created by Jacob Rail on 10/3/13.
//  Copyright (c) 2013 Jacob Rail. All rights reserved.
//

#ifndef Assignment_4__Projections_XWing_h
#define Assignment_4__Projections_XWing_h


//
//  main.c
//  Scene in 3D
//
//  Created by Jacob Rail on 9/24/13.
//  Copyright (c) 2013 Jacob Rail. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "Pieces.h"

#define GLGLEXR_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

// My Globals
// draw object
static int bodyWidth = 35;
static int bodyBackHight = 6;
static int bodyFrontHight = 4;
static int bodyDepthFront = 5;
static int bodyDepthBack = 5;

/*** Draw wings ***/
static int wingDepth = 20;
static int wingBackToTip = 3;
//static int wingFrontToTip = 0;
static int wingTipWidth = 7;
static int wingBaseWidth = 10; //wingBackToTip + wingTipWidth + wingFrontToTip
static int thrust = 0;
static int fire = 0;
static double laserDistance = 0;
//static double laser[4];
static int MAX_DIST = 400;

class XWing: Piece {
	
public:
	XWing (double c);
	void draw(double scale, double x, double z, char color);

private:
	/*** Private Variables ***/
	
	
	
	/*** Private Functions ***/
	
	// Prototypes
	void Print(const char* format, ...);
	void DrawCircle(double x, double y, double z, double radius, double red, double green, double blue, double normalXVal);
	void drawCylinderX(double x1, double x2, double y, double z, double radius);
	void drawCone(double x1, double x2, double y1, double y2, double z,
				  double radius);
	void wing(double x, double y, double z, double z1, double length);
	void drawWings();
	void display ();
	void key(unsigned char ch, int x, int y);
	void special(int key, int x, int y);
	void idle();
	void fireLaser(double, double, double, double);
	void timer(int v);

};
//----------------------------------------------------------------------------//

XWing::XWing (double c){
	colorBottom = c;
	colorTop = c + diff;
}

//----------------------------------------------------------------------------//

void XWing::draw(double scale, double x, double z, char color){

	int rotation = 0;
	double zoffset = 0;
	double xoffset = 0;
	
	if (color == 'w' || color == 'W') {
		rotation = 90;
		zoffset = 3.5;
		xoffset = 2.5;
	}
	else if (color == 'b' || color == 'B'){
		rotation = -90;
		zoffset = -3.5;
		xoffset = -2.5;
	}
	else
		EXIT_FAILURE;
	
	glPushMatrix();

	glTranslated(x, 5, zoffset - z);
	glScaled(scale, scale, scale);
	glTranslated(xoffset, 0, 0);
	glRotated(rotation, 0, 1, 0);

	display();
	
	glPopMatrix();
	
 }

//----------------------------------------------------------------------------//
void XWing::timer(int v){
	
	if (laserDistance < MAX_DIST) {
		++laserDistance;
	}
	else fire = 1-fire;
	/*
	if (fire)
		glutTimerFunc(5, timer, v);
	else{
		laserDistance = 0;
	}*/
}

//----------------------------------------------------------------------------//

void XWing::fireLaser(double x1, double x2, double y, double z){
	//	glutTimerFunc(0, timer, 1);
}


//----------------------------------------------------------------------------//
/**************************/
/*** CHECK FOR LIGHTING ***/
/**************************/
void XWing::DrawCircle(double x, double y, double z, double radius, double red, double green, double blue, double normalXVal){
	double x1, y1, z1;
	double angle;
	
	glPushMatrix();
	glTranslated(x, y, z);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(normalXVal, y, z);
	glColor3d(red, green, blue);
	for (angle = 1.0; angle <= 360; angle += 0.5) {
		x1 = 0;
		y1 = sin(angle)*radius;
		z1 = cos(angle)*radius;
		glVertex3f(x1,y1,z1);
	}
	glEnd();
	glPopMatrix();
}

//----------------------------------------------------------------------------//

void XWing::drawCylinderX(double x1, double x2, double y, double z, double radius){
	
	glPushMatrix();
	glTranslated(0, y, z);
	glBegin(GL_QUAD_STRIP);
	unsigned int a;
	double y1, z1;
	for (a = 0; a <= 360; a += 5){
		y1 = Sin(a)*radius;
		z1 = Cos(a)*radius;
		// color back
		glNormal3f(x1, y1, z1);
		glColor3f(colorBottom, colorBottom, colorBottom);
		glVertex3d(x1, y1, z1);
		// color front
		glColor3f(colorTop, colorTop, colorTop);
		glVertex3d(x2, y1, z1);
	}
	glEnd();
	
	// draw and color back
	DrawCircle(x1, 0, 0, radius, colorBottom, colorBottom, colorBottom, -1);
	// draw and color front
	DrawCircle(x2, 0, 0, radius, colorTop, colorTop, colorTop, + 1);
	glPopMatrix();
	
}
//----------------------------------------------------------------------------//

void XWing::wing(double x, double y, double z, double z1, double length){
	double angle = atan(length/(z1 - z));
	
	glBegin(GL_QUADS);
	// top
	glNormal3f(Cos(angle), Sin(angle), 0);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3d(0, y + 1, z1);
	glVertex3d(wingBackToTip, length, z);
	glVertex3d(wingBackToTip + wingTipWidth, length, z);
	glVertex3d(wingBaseWidth, y + 1, z1);
	
	// bottom
	glColor3f(colorBottom, colorBottom, colorBottom);
	glVertex3d(0, y, z1);
	glVertex3d(wingBackToTip, length, z);
	glVertex3d(wingBackToTip + wingTipWidth, length, z);
	glVertex3d(wingBaseWidth, y, z1);
	glEnd();
	
	// back
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(colorBottom, colorBottom, colorBottom);
	glVertex3f(0, y, z1);
	glVertex3f(wingBackToTip, length, z);
	glVertex3f(0, y+1, z1);
	glEnd();
	// front
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3f(wingBaseWidth, y, z1);
	glVertex3f(wingBaseWidth, length, z);
	glVertex3f(wingBaseWidth, y+1, z1);
	glEnd();
	
}

//----------------------------------------------------------------------------//

void XWing::drawCone(double x1, double x2, double y1, double y2, double z,
					 double radius){
	glBegin(GL_QUAD_STRIP);
	for (unsigned int j=0;j<=360;j+=5) {
		glColor3f(colorBottom, colorBottom, colorBottom);
		glVertex3f(x1, y1 + (Sin(j))*radius, (Cos(j)*radius) + z);
		glColor3f(colorTop, colorTop, colorTop);
		glVertex3f(x2, y2, z);
    }
    glEnd();
}

//----------------------------------------------------------------------------//

void XWing::drawWings(){
	
	// draw lasers
	int laserLength = 20;
	double laserRadius = 0.2;
	if (fire){
		drawCylinderX(25 + laserDistance, laserLength + laserDistance, 10, wingDepth, laserRadius);
		drawCylinderX(25 + laserDistance, laserLength + laserDistance, 10, -wingDepth-bodyDepthBack, laserRadius);
		drawCylinderX(25 + laserDistance, laserLength + laserDistance, -4, wingDepth, laserRadius);
		drawCylinderX(25 + laserDistance, laserLength + laserDistance, -4, -wingDepth-bodyDepthBack, laserRadius);
	}
	
	// draw thrusters
	if (thrust) {
		drawCone(-3, -7, 7, 7, 2, 0.75);
		drawCone(-3, -7, 7, 7, -2-bodyDepthBack, 0.75);
		drawCone(-3, -7, -1, -1, 2, 0.75);
		drawCone(-3, -7, -1, -1, -2-bodyDepthBack, 0.75);
	}
	
	/********************/
	/***TOP RIGHT WING***/
	/********************/
	wing(0, 4, wingDepth, 0, 10);							// draw wing
	drawCylinderX(2, wingBaseWidth + 1, 7, 2, 1.75);		// engine
	drawCylinderX(-3, 2, 7, 2, 0.75);						// exhaust
	drawCylinderX(2, wingBaseWidth + 2, 10, wingDepth, 1);	// phaser
	drawCone(12, 25, 10, 10, wingDepth, 0.33);			// top right phaser cone
	
	/*******************/
	/***TOP LEFT WING***/
	/*******************/
	wing(0, 1, wingDepth, 0, -4);									// wing
	drawCylinderX(2, wingBaseWidth + 1, 7, -2-bodyDepthBack, 1.75);	// engine
	drawCylinderX(-3, 2, 7, -2-bodyDepthBack, 0.75);				// exhaust
	drawCylinderX(2, 12, 10, -wingDepth-bodyDepthBack, 1);			// phaser
	drawCone(12, 25, 10, 10,
			 -wingDepth - bodyDepthBack, 0.33);			// top right phaser cone
	
	/***********************/
	/***BOTTOM RIGHT WING***/
	/***********************/
	wing(0, 4, -wingDepth-bodyDepthBack, -bodyDepthFront, 10);	// wing
	drawCylinderX(3, wingBaseWidth + 1, -1, +2, 1.75);			// engine
	drawCylinderX(-3, 5, -1, 2, 0.75);							// exhaust
	drawCylinderX(2, 12, -4, wingDepth, 1);						// phaser
	drawCone(12, 25, -4, -4, wingDepth, 0.33);			// top right phaser cone
	
	/*****************/
	/***BOTTOM LEFT***/
	/*****************/
	wing(0, 1, -wingDepth-bodyDepthBack, -bodyDepthFront, -4);		// wing
	drawCylinderX(3, wingBaseWidth + 1, -1, -2-bodyDepthBack, 1.75);// engine
	drawCylinderX(-3, 5, -1, -2-bodyDepthBack, 0.75);				// exhaust
	drawCylinderX(2, 12, -4, -wingDepth-bodyDepthBack, 1);			// phaser
	drawCone(12, 25, -4, -4,
			 -wingDepth - bodyDepthBack, 0.33);			// top right phaser cone
	
}

//----------------------------------------------------------------------------//

void XWing::display (){
	
	// enable trasparency.
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBegin(GL_QUADS);
	
	/*** Draw body ***/
	// top body panel
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3d(0, bodyBackHight, 0);
	glVertex3d(0, bodyBackHight, -bodyDepthFront);
	glVertex3d(bodyWidth, bodyFrontHight, -bodyDepthFront);
	glVertex3d(bodyWidth, bodyFrontHight, 0);
	
	// front panel REDONE
	glBegin(GL_QUAD_STRIP);
	glColor3d(colorTop, colorTop, colorTop);
	glVertex3d(bodyWidth, 0, 0);
	glVertex3d(bodyWidth, 0, -bodyDepthFront);
	glVertex3d(bodyWidth, (double) bodyFrontHight/2,
			   (double) -bodyDepthFront-0.5);
	glVertex3d(bodyWidth, (double) bodyFrontHight/2, +0.5);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3d(bodyWidth, (double) bodyFrontHight/2, +0.5);
	glVertex3d(bodyWidth, (double) bodyFrontHight/2,
			   (double) -bodyDepthFront-0.5);
	glVertex3d(bodyWidth, bodyFrontHight, 0);
	glVertex3d(bodyWidth, bodyFrontHight, -bodyDepthFront);
	glEnd();
	glEnd();
	
	glBegin(GL_QUADS);
	// body bottom panel
	glColor3f(colorBottom, colorBottom, colorBottom);
	glVertex3d(0, 0, 0);
	glVertex3d(bodyWidth, 0, 0);
	glVertex3d(bodyWidth, 0, -bodyDepthFront);
	glVertex3d(0, 0, -bodyDepthFront);
	glEnd();
	
	// body back panel
	glBegin(GL_QUADS);
	glColor3d (colorBottom, colorBottom, colorBottom);
	glVertex3d(0, bodyBackHight,	0);
	glVertex3d(0, bodyBackHight,	-bodyDepthBack);
	glVertex3d(0, bodyBackHight/2, -1-bodyDepthBack);
	glVertex3d(0, bodyBackHight/2, 1);
	
	glVertex3d(0, bodyBackHight/2, -1-bodyDepthBack);
	glVertex3d(0, bodyBackHight/2, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, -bodyDepthBack);
	glEnd();
	
	// Right tilt panels (top then bot)
	glBegin(GL_QUAD_STRIP);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3f(0, bodyBackHight, 0);
	glVertex3f(bodyWidth, bodyFrontHight, 0);
	glVertex3f(0, bodyBackHight/2, +1);
	glVertex3f(bodyWidth, bodyFrontHight/2, +0.5);
	glColor3f(colorBottom, colorBottom, colorBottom);
	glVertex3f(0, 0, 0);
	glVertex3f(bodyWidth, 0, 0);
	glEnd();
	
	// left tilt panels (top then bot)
	glBegin(GL_QUAD_STRIP);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3f(0, bodyBackHight, -bodyDepthBack);
	glVertex3f(bodyWidth, bodyFrontHight, -bodyDepthFront);
	glVertex3f(0, bodyBackHight/2, -1-bodyDepthBack);
	glVertex3f(bodyWidth, bodyFrontHight/2, -0.5-bodyDepthFront);
	glColor3f(colorBottom, colorBottom, colorBottom);
	glVertex3f(0, 0, -bodyDepthBack);
	glVertex3f(bodyWidth, 0, -bodyDepthFront);
	glEnd();
	
	drawWings();
	
	// draw cockpit
	// right side
	int backWindowWidth = 2;
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(colorBottom, colorBottom, colorBottom, 0.5);
	glVertex3f(11, bodyBackHight - 1, 0);
	glVertex3f(11 + 8, bodyBackHight - 1, -1.5);
	glVertex3f(11 + backWindowWidth, bodyBackHight +1, -1.5);
	glVertex3f(10, bodyBackHight - 1, -1.5);
	glEnd();
	//left side
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(11, bodyBackHight - 1, -5);
	glVertex3f(11 + 8, bodyBackHight - 1, +1.5 - 5);
	glVertex3f(11 + backWindowWidth, bodyBackHight +1, +1.5 - 5);
	glVertex3f(10, bodyBackHight - 1, +1.5 - 5);
	glEnd();
	// front and back
	glBegin(GL_QUAD_STRIP);
	glVertex3f(10, bodyBackHight - 1, -1.5);
	glVertex3f(10, bodyBackHight - 1, -3.5);
	glVertex3f(11 + backWindowWidth, bodyBackHight + 1, -1.5);
	glVertex3f(11 + backWindowWidth, bodyBackHight + 1, -3.5);
	glVertex3f(19, bodyBackHight - 1, -1.5);
	glVertex3f(19, bodyBackHight - 1, -3.5);
	glEnd();
	
}

#endif
