#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <iostream>
#include "boid.h"

using namespace std;


typedef float type;
struct boundingBox{
    vector3 *center;
    vector3 *dim;
    type minx,
         maxx,
         miny,
         maxy,
         minz,
         maxz;
    boundingBox(vector3 dim, vector3 center);
    boundingBox();
    bool containBoid(vector3 b);
    void draw();
    ~boundingBox();
};
boundingBox::boundingBox(vector3 dim, vector3 center){
    this->center = new vector3(center.x, center.y, center.z);
    this->dim = new vector3(dim.x, dim.y, dim.z);
    minx = this->center->x - this->dim->x/2;
    maxx = this->center->x + this->dim->x/2;
    miny = this->center->y - this->dim->y/2;
    maxy = this->center->y + this->dim->y/2;
    minz = this->center->z - this->dim->z/2;
    maxz = this->center->z + this->dim->z/2;
}
void boundingBox::draw(){
    glPushMatrix();
    glColor3d(1,1,1);
    glBegin(GL_LINE_STRIP);
    glVertex3d(center->x - dim->x/2, center->y + dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y + dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y - dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y - dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y + dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y + dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y + dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y - dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y - dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y + dim->y/2, center->z + dim->z/2);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(center->x - dim->x/2, center->y + dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y + dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y - dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y - dim->y/2, center->z - dim->z/2);
    glVertex3d(center->x - dim->x/2, center->y - dim->y/2, center->z + dim->z/2);
    glVertex3d(center->x + dim->x/2, center->y - dim->y/2, center->z + dim->z/2);
    glEnd();
    glPopMatrix();
}
boundingBox::boundingBox(){

}
bool boundingBox::containBoid(vector3 b){ 
    if(b.x >= minx && b.x <= maxx && 
       b.y >= miny && b.y <= maxy && 
       b.z >= minz && b.z <= maxz)
        return true;
    return false;
}

boundingBox::~boundingBox(){
    delete center;
    delete dim;
}

#endif // BOUNDINGBOX_H
