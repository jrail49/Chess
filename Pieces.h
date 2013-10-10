//
//  Pieces.h
//  Assignment 4: Projections
//
//  Created by Jacob Rail on 10/2/13.
//  Copyright (c) 2013 Jacob Rail. All rights reserved.
//

#ifndef Assignment_4__Projections_Pieces_h
#define Assignment_4__Projections_Pieces_h

#include <list>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

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

int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int inc = 10;		  // Ball increment
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

struct boardTile {
	int x;
	int z;
};

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);
	//  For a sphere at the origin, the position
	//  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3d(x,y,z);
}
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// the change in color from top to bottom.
static double diff = 0.3;


class Piece {
public:
	void SetPosistion(char x, int z);
	// position of pieces
	boardTile position;
	// color of the piece
	string color;
	
private:
	
	
protected:
	// moves a piece can preform.
	list<boardTile> moves;
	// color for top and bottom of pieces that are fading colors
	double colorTop;
	double colorBottom;
	// the change in color from top to bottom.
	//double diff = 0.3;
	
	
	// functions to help draw pieces
	void drawSphere(double x, double y, double z, double r, int lats, int longs);
	void drawCircle(double x, double y, double z, double radius, double red,
					double green, double blue);
	void drawCylinderY(double x, double y1, double y2, double z, double radius);
	void drawConeY(double x, double y1, double y2, double z, double r1,
				   double r2);
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //



void Piece:: SetPosistion(char x, int z){
	if (x == 'a' || x == 'A')
		position.x = -35;
	else if (x == 'b' || x == 'B')
		position.x = -25;
	else if (x == 'c' || x == 'C')
		position.x = -15;
	else if (x == 'd' || x == 'D')
		position.x = -5;
	else if (x == 'e' || x == 'E')
		position.x = 5;
	else if (x == 'f' || x == 'F')
		position.x = 15;
	else if (x == 'g' || x == 'G')
		position.x = 25;
	else if (x == 'h' || x == 'H')
		position.x = 35;
	// invalid position for a piece
	else
		EXIT_FAILURE;
	
	// trannslate z into cordinates for drawing.
	if ( z >= 1 && z <= 8) {
		position.z = z*-10 + 45;
	}
	else
		EXIT_FAILURE;
}

// -------------------------------------------------------------------------- //

void Piece:: drawSphere(double x, double y, double z, double r, int lats, int longs) {
	
	int th,ph;
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,static_cast<float>(0.01*emission),1.0};
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r, r, r);

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


void Piece:: drawCircle(double x, double y, double z, double radius,
						double red, double green, double blue){
	
	double x1, y1, z1;
	double angle;
	
	glPushMatrix();
	glTranslated(x, 0, z);
	
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(x, y, z);
	
	glColor3d(red, green, blue);
	for (angle = 1.0; angle <= 360; angle += 0.5) {
		x1 = sin(angle)*radius;
		y1 = y;
		z1 = cos(angle)*radius;
		glVertex3f(x1,y1,z1);
	}
	glEnd();
	glPopMatrix();
	
}

// -------------------------------------------------------------------------- //

void Piece:: drawCylinderY(double x, double y1, double y2, double z, double radius){

	
	glPushMatrix();
	glTranslated(x, 0, z);
	
	glBegin(GL_QUAD_STRIP);
	
	unsigned int a;
	double x1, z1;
	for (a = 0; a <= 360; a += 5){
		// color bottom
		x1 = Sin(a)*radius;
		z1 = Cos(a)*radius;
		glNormal3f(x1, y1, z1);
		glColor3f(colorBottom, colorBottom, colorBottom);
		glVertex3d(x1, y1, z1);
		// color top
		glColor3f(colorTop, colorTop, colorTop);
		glVertex3d(x1, y2, z1);
	}
	glEnd();
	
	// draw and color bottom circle
	drawCircle(0, y1, 0, radius, colorBottom, colorBottom, colorBottom);
	// draw and color top circle
	drawCircle(0, y2, 0, radius, colorTop, colorTop, colorTop);
	
	glPopMatrix();
}

// -------------------------------------------------------------------------- //

void Piece::drawConeY(double x, double y1, double y2, double z, double r1, double r2){
	
	glPushMatrix();
	glTranslated(x, 0, z);
	glBegin(GL_QUAD_STRIP);
	unsigned int a;
	double x1 = 0, x2 = 0;
	double z1 = 0, z2 = 0;
	for (a = 0; a <= 360; a += 5){
		x1 = Sin(a)*r1;
		x2 = Sin(a)*r2;
		z1 = Cos(a)*r1;
		z2 = Cos(a)*r2;
		
		glNormal3f(x1, 0, z1);
		// color bottom
		glColor3f(colorBottom, colorBottom, colorBottom);
		glVertex3d(x1, y1, z1);
		// color top
		glColor3f(colorTop, colorTop, colorTop);
		glVertex3d(x2, y2, z2);
	}
	glEnd();
	glPopMatrix();
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

class Pawn: public Piece{
public:
	Pawn (double c);
	void draw(double x, double z);
	void drawPawn();
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

Pawn::Pawn (double c){
	colorBottom = c;
	colorTop = c + diff;
}

// -------------------------------------------------------------------------- //

void Pawn:: draw(double x, double z){
	
	double baseRadius = 3;
	double baseH1 = 0.85;
	drawCylinderY(x, 0, baseH1, z, baseRadius);
	
	// draw body
	double toBody = baseH1;
	double bodyH1 = 0.75;
	
	double bodyR1 = baseRadius;
	double bodyR2 = baseRadius - 0.5;
	drawConeY(x, toBody, toBody + bodyH1, z, bodyR1, bodyR2);
	drawCircle(x, toBody + bodyH1, z, bodyR2, colorTop, colorTop, colorTop);
	
	double bodyH2 = 0.6;
	double bodyR3 = baseRadius - 1;
	double bodyR4 = baseRadius - 1.20;
	drawConeY(x, toBody + bodyH1, toBody + bodyH1 + bodyH2,
			  z, bodyR3, bodyR4);
	
	double bodyH3 = 0.25;
	double bodyR5 = baseRadius - 1.75;
	drawConeY(x, toBody + bodyH1 + bodyH2,
			  toBody + bodyH1 + bodyH2 + bodyH3, z, bodyR4, bodyR5);
	
	double bodyH4 = 5;
	drawCylinderY(x, toBody + bodyH1 + bodyH2 + bodyH3,
				  toBody + bodyH1 + bodyH2 + bodyH3 + bodyH4, z,
				  bodyR5);
	
	// draw ring
	double bodyH5 =  toBody + bodyH1 + bodyH2 + bodyH3 + 2.5;
	drawConeY(x, bodyH5, bodyH5 + 0.3, z, bodyR5, 2.35);
	drawCylinderY(x, bodyH5 + 0.3, bodyH5 + 0.5, z, 2.35);
	drawConeY(x, bodyH5 + 0.5, bodyH5 + 0.8, z, 2.35, bodyR5);
	
	// draw top
	double sphereColor = colorBottom + diff/2;
	glColor3f(sphereColor, sphereColor, sphereColor);
	drawSphere(x, 8.5, z, 2.0, 20, 20);
	
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

class Rook: public Piece{
public:
	Rook (double c);
	void draw(double x, double z);
	void drawRook();
};
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

Rook::Rook (double c){
	colorBottom = c;
	colorTop = c + diff;
}

// -------------------------------------------------------------------------- //

void Rook:: drawRook(){
	draw(position.x, position.z);
}

// -------------------------------------------------------------------------- //

void Rook:: draw(double x, double z){
	double baseHeight = 1.5;
	double baseRadius = 3;
	double bodyH1 = 1;
	double bodyH2 = 5;
	
	double bodyR1 = 2.25;
	double bodyR2 = 2.75;
	double bodyR3 = 1.75;
	
	// draw base
	drawCylinderY(x, 0, baseHeight, z, baseRadius);
	
	// draw body
	drawConeY(x, baseHeight, baseHeight + bodyH1, z, bodyR1, bodyR2);
	drawConeY(x, baseHeight + bodyH1, baseHeight + bodyH1 + bodyH2, z, bodyR2,
			  bodyR3);
	
	// draw top
	double bodyToTop = baseHeight + bodyH1 + bodyH2;
	double topH1 = 0.75;
	double topH2 = 1.0;
	double topH3 = 0.75;
	double topH4 = 2.0;
	double topR1 = bodyR3 + 0.25;
	double topR2 = bodyR3 + 0.75;
	double topR3 = bodyR3 + 0.50;
	double topR4 = bodyR3 + 0.80;
	drawCylinderY(x, bodyToTop, bodyToTop + topH1, z, topR1);
	drawCylinderY(x, bodyToTop + topH1,
				  bodyToTop + topH1 + topH2, z, topR2);
	drawCylinderY(x, bodyToTop + topH1 + topH2,
				  bodyToTop + topH1 + topH2 + topH3,
				  z, topR3);
	drawCylinderY(x, bodyToTop + topH1 + topH2 + topH3,
				  bodyToTop + topH1 + topH2 + topH3 + topH4, z, topR4);
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

class King: public Piece{
public:
	King (double c);
	void draw(double x, double z);
	void drawKing();
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

King::King (double c){
	colorBottom = c;
	colorTop = c + diff;
}

// -------------------------------------------------------------------------- //

void King:: drawKing(){
	draw(position.x, position.z);
}

// -------------------------------------------------------------------------- //

void King:: draw(double x, double z){
	double h1 = 0.75;
	double r1 = 3;
	
	drawCylinderY(x, 0, h1, z, r1);

	double r2 =2.65;
	double r3 = 2.8;
	double h2 = h1 + 0.5;
	drawConeY(x, h1, h2, z, r2, r3);
	
	
	double h3 = h2 + 1.75;
	double r4 = 1.75;
	drawConeY(x, h2, h3, z, r3, r4);
	
	double h4 = h3 + 0.35;
	double r5 = r4 + 0.45;
	drawConeY(x, h3, h4, z, r4, r5);
	
	double h5 = h4 + 0.35;
	drawCylinderY(x, h4, h5, z, r5);
	
	double h6 = h5 + 0.35;
	double r6 = 1.6;
	drawConeY(x, h5, h6, z, r5, r6);
	
	// draw body
	double h7 = h6 + 5.5;
	double r7 = 1.45;
	drawConeY(x, h6, h7, z, r6, r7);
	
	// upper disk (disk below crown)
	double r8 = 2.5;
	double h8 = h7 + 0.2;
	drawConeY(x, h7, h8, z, r7, r8);
	
	double h9 = h8 + 0.2;
	drawCylinderY(x, h8, h9, z, r8);
	
	double h10 = h9 + 0.4;
	double r9 = 1.6;
	drawConeY(x, h9, h10, z, r8, r9);
	
	double h11 = h10 + 0.2;
	double r10 = 1.85;
	drawConeY(x, h10, h11, z, r9, r10);
	
	double h12 = h11 + 0.2;
	drawCylinderY(x, h11, h12, z, r10);
	
	double h13 = h12 + 0.4;
	// r7 = radius of the top of the cone that makes the body's shaft.
	drawCylinderY(x, h12, h13, z, r7);
	
	// draw top
	double h14 = h13 + 0.15;
	double r11 = 1.55;
	drawCylinderY(x, h13, h14, z, r11);
	
	double h15 = h14 + 2;
	double r12 = 1.50;
	double r13 = 2.25;
	drawConeY(x, h14, h15, z, r12, r13);
	
	double h16 = h15 + 0.2;
	double r14 = 1.5;
	drawConeY(x, h15, h16, z, r13, r14);
	
	double h17 = h16 + 0.3;
	double r15 = 1.3;
	drawConeY(x, h16, h17, z, r14, r15);
	drawCircle(x, h17, z, r15, colorTop, colorTop, colorTop);

	// draw front of cross, then back of cross.
	double xBase = 0.75;
	double h18 = h17 + 0.65;
	double zBase = 0.60;

	double x2Base = 1.75;
	double h19 = h18 + 0.85;
	double h20 = h19 + 0.95;
	double x3Base = 0.5;
	double x4Base = 0.85;
	
	double color1 = colorBottom + 0.1;
	glBegin(GL_QUAD_STRIP);
	// front of crown
	glColor3f(color1, color1, color1);
	glVertex3d(-xBase + x, h17 , zBase + z);
	glVertex3d(xBase + x, h17, zBase + z);
	// add color here to have it fade
	glVertex3d(-xBase + x, h18, zBase + z);
	glVertex3d(xBase + x, h18, zBase + z);
	
	glVertex3d(-x2Base + x, h18, zBase + z);
	glVertex3d(x2Base + x, h18, zBase + z);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3d(-x2Base + x, h19, zBase + z);
	glVertex3d(x2Base + x, h19, zBase + z);

	glVertex3d(-x3Base + x, h19, zBase + z);
	glVertex3d(x3Base + x, h19, zBase + z);
	// add color here to have it fade
	glVertex3d(-x4Base + x, h20, zBase + z);
	glVertex3d(x4Base + x, h20, zBase + z);
	glEnd();
	
	// back of crown
	glBegin(GL_QUAD_STRIP);
	glColor3f(color1, color1, color1);
	glVertex3d(-xBase + x, h17 , -zBase + z);
	glVertex3d(xBase + x, h17, -zBase + z);
	// add color here to have it fade
	glVertex3d(-xBase + x, h18, -zBase + z);
	glVertex3d(xBase + x, h18, -zBase + z);

	glVertex3d(-x2Base + x, h18, -zBase + z);
	glVertex3d(x2Base + x, h18, -zBase + z);
	glColor3f(colorTop, colorTop, colorTop);
	glVertex3d(-x2Base + x, h19, -zBase + z);
	glVertex3d(x2Base + x, h19, -zBase + z);
	
	glVertex3d(-x3Base + x, h19, -zBase + z);
	glVertex3d(x3Base + x, h19, -zBase + z);
	// add color here to have it fade
	glVertex3d(-x4Base + x, h20, -zBase + z);
	glVertex3d(x4Base + x, h20, -zBase + z);
	
	glEnd();

	
	glBegin(GL_QUAD_STRIP);
	// sides of crown
	// bottom right
	glColor3f(color1, color1, color1);
	glVertex3d(+xBase + x, h17, +zBase + z);
	glVertex3d(+xBase + x, h17, -zBase + z);
	
	glVertex3d(+xBase + x, h18, +zBase + z);
	glVertex3d(+xBase + x, h18, -zBase + z);
		
	double color2 = colorBottom + diff/2;
	glColor3f(color2, color2, color2);
	
	glVertex3d(+x2Base + x, h18, +zBase + z);
	glVertex3d(+x2Base + x, h18, -zBase + z);
	
	glVertex3d(+x2Base + x, h19, +zBase + z);
	glVertex3d(+x2Base + x, h19, -zBase + z);

	glColor3f(colorTop, colorTop, colorTop);

	glVertex3d(+x3Base + x, h19, +zBase + z);
	glVertex3d(+x3Base + x, h19, -zBase + z);
	
	glVertex3d(+x4Base + x, h20, +zBase + z);
	glVertex3d(+x4Base + x, h20, -zBase + z);
	
	glVertex3d(-x4Base + x, h20, +zBase + z);
	glVertex3d(-x4Base + x, h20, -zBase + z);
	
	glColor3f(color2, color2, color2);
	
	glVertex3d(-x3Base + x, h19, +zBase + z);
	glVertex3d(-x3Base + x, h19, -zBase + z);
	
	glVertex3d(-x2Base + x, h19, +zBase + z);
	glVertex3d(-x2Base + x, h19, -zBase + z);
	
	glVertex3d(-x2Base + x, h18, +zBase + z);
	glVertex3d(-x2Base + x, h18, -zBase + z);
	
	glColor3f(color1, color1, color1);
	
	glVertex3d(-xBase + x, h18, +zBase + z);
	glVertex3d(-xBase + x, h18, -zBase + z);
	
	glVertex3d(-xBase + x, h17, +zBase + z);
	glVertex3d(-xBase + x, h17, -zBase + z);
	
	glEnd();
	
}










#endif




