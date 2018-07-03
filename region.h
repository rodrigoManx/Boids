#include "headers.h"

struct region{
	region();
	void draw(float dimx, float dimy, float dimz);
	~region();
};

region::region(){

}
void region::draw(float dimx, float dimy, float dimz){
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	glVertex3d(-dimx/2, dimy/2, dimz/2);
	glVertex3d(-dimx/2, dimy/2, -dimz/2);
	glVertex3d(-dimx/2, -dimy/2, -dimz/2);
	glVertex3d(-dimx/2, -dimy/2, dimz/2);
	glVertex3d(-dimx/2, dimy/2, dimz/2);
	glVertex3d(dimx/2, dimy/2, dimz/2);
	glVertex3d(dimx/2, dimy/2, -dimz/2);
	glVertex3d(dimx/2, -dimy/2, -dimz/2);
	glVertex3d(dimx/2, -dimy/2, dimz/2);
	glVertex3d(dimx/2, dimy/2, dimz/2);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(-dimx/2, dimy/2, -dimz/2);
	glVertex3d(dimx/2, dimy/2, -dimz/2);
	glVertex3d(-dimx/2, -dimy/2, -dimz/2);
	glVertex3d(dimx/2, -dimy/2, -dimz/2);
	glVertex3d(-dimx/2, -dimy/2, dimz/2);
	glVertex3d(dimx/2, -dimy/2, dimz/2);
	glEnd();
	glPopMatrix();
}
region::~region(){

}