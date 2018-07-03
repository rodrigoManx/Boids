#include "headers.h"
#include <stdlib.h>
#include <time.h>

//class predator;

class boid{
protected:
	float size;
	float vLimit;
	float visionRadius;
	float personalSpace;
	float minx, miny, minz, maxx, maxy, maxz;
	float panicTime;
	float x,y,z;
public:
	bool panic;
	vector3 *position;
	vector3 *speed;
	boid();
	boid(vector3 dim, float personalSpace, float visionRadius, float vLimit, float size);
	boid(vector3 dim, float vLimit, float size, float visionRadius);
	void setUp(vector3 dim, float vLimit, float size);
	~boid();
	bool inVisionRadius(boid *b);
	bool inRadius(boid *b, float visionRadius);
	void rules(vector3 *r1, vector3 *r2, vector3 *r3, vector < boid *> boids);
	void drawNewPosition(float deltaTime, vector < boid *> boids, vector < class predator *> predators);
	virtual void drawNewPosition(float deltaTime, vector < predator *> predators);
	vector3 separation(vector < boid *> boids);
	vector3 cohesion(vector < boid *> boids);
	vector3 alignment(vector < boid *> boids);
};



class predator : public boid {
	float r, g, b;
public:	
	predator();
	predator(vector3 dim, float visionRadius, vector3 rgb, float vLimit, float size);
	void drawNewPosition(float deltaTime, vector3 point);
	bool inVisionRadius(vector3 point);
};

predator::predator() : boid(){}
predator::predator(vector3 dim, float visionRadius, vector3 rgb, float vLimit, float size) : boid(dim, vLimit, size, visionRadius){
	this-> r = rgb.x;
	this-> g = rgb.y;
	this-> b = rgb.z;
}
void predator::drawNewPosition(float deltaTime, vector3 point){
	if (this->inVisionRadius(point)){
		*speed = *speed + (point - *position);
	}
	vector3 SUV = (*speed / speed->magnitude());
	*speed = SUV * vLimit;
	
	vector3 visionRadiusP = *position + (SUV * visionRadius);

	if(visionRadiusP.x < minx)
		speed->x += vLimit * 0.4;
	else if(visionRadiusP.x > maxx)
		speed->x -= vLimit * 0.4;
	if(visionRadiusP.y < miny)
		speed->y += vLimit * 0.4;
	else if(visionRadiusP.y > maxy)
		speed->y -= vLimit * 0.4;
	if(visionRadiusP.z < minz)
		speed->z += vLimit * 0.4;
	else if(visionRadiusP.z > maxz)
		speed->z -= vLimit * 0.4;

	vector3 futurePostion = *position + (*speed * deltaTime);

	if(futurePostion.x < minx || futurePostion.x > maxx){
		speed->x *= -1;
	}
	if(futurePostion.y < miny || futurePostion.y > maxy){
		speed->y *= -1;
	}
	if(futurePostion.z < minz || futurePostion.z > maxz){
		speed->z *= -1;
	}

	SUV = (*speed / speed->magnitude());
	*speed = SUV * vLimit; 

	

	*position = *position + (*speed * deltaTime);
	glColor3d(255,0,0);
	glBegin(GL_LINES);
	glVertex3d(position->x, position->y, position->z);
	glVertex3d(position->x + (SUV.x * visionRadius * 0.2),
			   position->y + (SUV.y * visionRadius * 0.2),
			   position->z + (SUV.z * visionRadius * 0.2));
	glEnd();

	glPushMatrix();
	glTranslatef(position->x, position->y, position->z);
	glutSolidSphere(size, 20, 20);	
	glPopMatrix();
}


boid::boid(){
	size = 0.5;
	panic = false;
	panicTime = 0;
	visionRadius = 0;
	personalSpace = 0;
	vLimit = 0;
	speed = position = NULL;
}
boid::boid(vector3 dim, float vLimit, float size, float visionRadius){
	panic = false;
	panicTime = 0;
	this-> visionRadius = visionRadius;
	personalSpace = 0;
	setUp(dim, vLimit, size);
}
boid::boid(vector3 dim, float personalSpace, float visionRadius, float vLimit, float size){
	this-> panic = false;
	this-> panicTime = 0;
	this-> visionRadius = visionRadius;
	this-> personalSpace = personalSpace;
	setUp(dim, vLimit, size);
}
void boid::setUp(vector3 dim, float vLimit, float size){
	this-> size = size;
	this-> vLimit = vLimit;
	minx = -dim.x/2;
	maxx = dim.x/2;
	miny = -dim.y/2;
	maxy = dim.y/2;
	minz = -dim.z/2;
	maxz = dim.z/2;
	this-> speed = new vector3((rand() % (int)vLimit) - vLimit/2, 
		                       (rand() % (int)vLimit) - vLimit/2, 
		                       (rand() % (int)vLimit) - vLimit/2);



	this-> position = new vector3((rand() % ((int)dim.x)) - (((int)dim.x) / 2), 
		                          (rand() % ((int)dim.y)) - (((int)dim.y) / 2), 
		                          (rand() % ((int)dim.z)) - (((int)dim.z) / 2));
}

boid::~boid(){
	delete position;
	delete speed;
}
vector3 boid::alignment(vector < boid *> boids){
	vector3 meanVelocity(0,0,0);
	for (int i = 0; i < boids.size(); ++i){
		meanVelocity = meanVelocity + *boids[i]->speed;
	}
	meanVelocity = meanVelocity / boids.size();
	return meanVelocity / 8 ;
}
vector3 boid::cohesion(vector < boid *> boids){
	vector3 meanCenter(0,0,0);
	for (int i = 0; i < boids.size(); ++i){
		meanCenter = meanCenter + *boids[i]->position;
	}
	meanCenter = meanCenter / boids.size();
	return meanCenter / 100;
}
vector3 boid::separation(vector < boid *> boids){
	vector3 center(0,0,0);
	for (int i = 0; i < boids.size(); ++i){
		if((*boids[i]->position - *position).magnitude() < personalSpace){
			center = center - (*boids[i]->position - *position);
		}
	}
	return center;
}
void boid::rules(vector3 *r1, vector3 *r2, vector3 *r3, vector < boid *> boids){
	int n = boids.size();
	for (int i = 0; i < n; ++i){
			*r2 = *r2 + *boids[i]->speed;
			*r3 = *r3 + *boids[i]->position; 
			if((*boids[i]->position - *position).magnitude() < personalSpace){
				*r1 = *r1 - (*boids[i]->position - *position);
			}
	}
	if(n != 0){
		*r2 = *r2 / n;
		*r3 = *r3 / n;
		*r3 = (*r3 - *position) / 100;
	}
}
void boid::drawNewPosition(float deltaTime, vector < boid *> boids, vector < predator *> predators){
	if(predators.size() > 0){
		vector3 huida(0,0,0);
		for(int i = 0; i < predators.size(); ++i){
			huida = huida + (*predators[i]->position - *position) * -1;
		}
		huida = huida / predators.size();
		*speed = *speed + huida;
	}
	else{
		vector3 separationRule(0,0,0); 
		vector3 alignmentRule(0,0,0); 
		vector3 cohesionRule(0,0,0);
		rules(&separationRule, &alignmentRule, &cohesionRule, boids);
		*speed = *speed + separationRule + alignmentRule + cohesionRule;
	}

	vector3 SUV = (*speed / speed->magnitude());
	*speed = SUV * vLimit;
	
	vector3 visionRadiusP = *position + (SUV * visionRadius);


	if(visionRadiusP.x < minx)
		speed->x += vLimit * 0.4;
	else if(visionRadiusP.x > maxx)
		speed->x -= vLimit * 0.4;
	if(visionRadiusP.y < miny)
		speed->y += vLimit * 0.4;
	else if(visionRadiusP.y > maxy)
		speed->y -= vLimit * 0.4;
	if(visionRadiusP.z < minz)
		speed->z += vLimit * 0.4;
	else if(visionRadiusP.z > maxz)
		speed->z -= vLimit * 0.4;

	vector3 futurePostion = *position + (*speed * deltaTime); 

	if(futurePostion.x < minx || futurePostion.x > maxx){
		speed->x *= -1;
		panic = true;
	}
	if(futurePostion.y < miny || futurePostion.y > maxy){
		speed->y *= -1;
		panic = true;
	}
	if(futurePostion.z < minz || futurePostion.z > maxz){
		speed->z *= -1;
		panic = true;
	}

	if(panic)
		panicTime = 0.5;
	
	*position = *position + (*speed * deltaTime);
	SUV = (*speed / speed->magnitude());
	glColor3d(0,255,0);
	glBegin(GL_LINES);
	glVertex3d(position->x, position->y, position->z);
	glVertex3d(position->x + (SUV.x * (visionRadius * 0.15)),
			   position->y + (SUV.y * (visionRadius * 0.15)),
			   position->z + (SUV.z * (visionRadius * 0.15)));
	glEnd();

	glPushMatrix();
	glTranslatef(position->x, position->y, position->z);
	glutSolidSphere(size, 20, 20);	
	glPopMatrix();
}

void boid::drawNewPosition(float deltaTime, vector < predator *> predators){
	panicTime -= deltaTime;

	vector3 SUV = (*speed / speed->magnitude());
	*speed = SUV * vLimit;
	
	vector3 visionRadiusP = *position + (SUV * visionRadius);

	if(visionRadiusP.x < minx)
		speed->x += vLimit * 0.4;
	else if(visionRadiusP.x > maxx)
		speed->x -= vLimit * 0.4;
	if(visionRadiusP.y < miny)
		speed->y += vLimit * 0.4;
	else if(visionRadiusP.y > maxy)
		speed->y -= vLimit * 0.4;
	if(visionRadiusP.z < minz)
		speed->z += vLimit * 0.4;
	else if(visionRadiusP.z > maxz)
		speed->z -= vLimit * 0.4;

	vector3 futurePostion = *position + (*speed * deltaTime); 

	if(futurePostion.x < minx || futurePostion.x > maxx){
		speed->x *= -1;
		panicTime = 0.5;
	}
	if(futurePostion.y < miny || futurePostion.y > maxy){
		speed->y *= -1;
		panicTime = 0.5;
	}
	if(futurePostion.z < minz || futurePostion.z > maxz){
		speed->z *= -1;
		panicTime = 0.5;
	}
	
	if (panicTime <= 0){
		panic = false;
	}

	*position = *position + (*speed * deltaTime);
	SUV = (*speed / speed->magnitude());
	glColor3d(0,255,0);
	glBegin(GL_LINES);
	glVertex3d(position->x, position->y, position->z);
	glVertex3d(position->x + (SUV.x * (visionRadius * 0.15)),
			   position->y + (SUV.y * (visionRadius * 0.15)),
			   position->z + (SUV.z * (visionRadius * 0.15)));
	glEnd();

	glPushMatrix();
	glTranslatef(position->x, position->y, position->z);
	glutSolidSphere(size, 20, 20);	
	glPopMatrix();
}

bool boid::inVisionRadius(boid *b){
	vector3 v = *b-> position - *position;
	float n = v * (*speed);
	float d = v.magnitude() * speed->magnitude();
	if(d != 0 && n != 0){
		if((acos(n/d) * 180.0 / PI) <= 135)
			return true;
	}
	return false;
}
bool predator::inVisionRadius(vector3 point){
	vector3 v = point - *position;
	float n = v * (*speed);
	float d = v.magnitude() * speed->magnitude();
	if(d != 0 && n != 0){
		if((acos(n/d) * 180.0 / PI) <= 70)
			return true;
	}
	return false;
}


bool boid::inRadius(boid *b, float vr){
	if ((*b->position - *position).magnitude() <= vr ){
		return true;
	}
	return false;
}