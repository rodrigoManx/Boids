//#include "boid.h"

class predator : public boid {
	float r, g, b;
	float size;
public:	
	predator();
	predator(vector3 dim, vector3 rgb, float size, float vLimit);
	void drawNewPosition(float deltaTime);
};

predator::predator() : boid(){}
predator::predator(vector3 dim, vector3 rgb, float size, float vLimit) : boid(dim, vLimit){
	this-> r = rgb.x;
	this-> g = rgb.y;
	this-> b = rgb.z;
	this-> size = size;
}
void predator::drawNewPosition(float deltaTime){
	vector3 tmp = *position + (*speed * deltaTime); 
	if(tmp.x < minx || tmp.x > maxx){
		speed->x *= -1;
	}
	if(tmp.y < miny || tmp.y > maxy){
		speed->y *= -1;
	}
	if(tmp.z < minz || tmp.z > maxz){
		speed->z *= -1;
	}
	*position = *position + (*speed * deltaTime);
	glPushMatrix();
	glColor3d(r,g,b);
	glTranslatef(position->x, position->y, position->z);
	glutSolidSphere(size,20,20);	
	glPopMatrix();
}
