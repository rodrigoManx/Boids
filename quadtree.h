#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include "boundingbox.h"

#define CHILDRENNUM 8

class quadTree3d{
    int boidsAmount;
    int predatorsAmount;
    float visionRadius;
    float personalSpaceR;
    float vLimit;
    vector < boid *> boids;
    vector < predator *> predators;
    boundingBox *quadrant;
    quadTree3d **children;
    bool leaf;
public:
    quadTree3d(vector3 dim, vector3 center, int boidsAmount, float visionRadius, float personalSpaceR, float vLimit, int predatorsAmount, float size);
    quadTree3d(vector3 dim, vector3 center, float visionRadius);
    quadTree3d *getMostPopulatedRegion(quadTree3d *region);
    void setUp(vector3 dim, vector3 center, float visionRadius);
    void draw(bool b);
    void drawLimits();
    bool insertBoids();
    bool insertPredators();
    void clearVectors();
    void foo(float deltaTime);
    void getNeighborsQuadrants(vector < quadTree3d *> *neighbors, quadTree3d *q);
    quadTree3d *getQuadrant(vector3 b);
    ~quadTree3d();
};
quadTree3d::quadTree3d(vector3 dim, vector3 center, int boidsAmount, float visionRadius, float personalSpaceR, float vLimit, int predatorsAmount, float size){
    this-> boidsAmount = boidsAmount;
    this-> visionRadius = visionRadius;
    this-> personalSpaceR = personalSpaceR;
    this-> predatorsAmount = predatorsAmount;
    for (int i = 0; i < this->boidsAmount; ++i){
        boids.push_back(new boid(dim, personalSpaceR, visionRadius, vLimit, size)); 
    }
    vector3 RGB(255,0,0);
    for (int i = 0; i < predatorsAmount; ++i){
        predators.push_back(new predator(dim, visionRadius, RGB, vLimit*2, size * 2));
    }
    setUp(dim, center, visionRadius);
}
quadTree3d::quadTree3d(vector3 dim, vector3 center, float visionRadius){
    setUp( dim, center, visionRadius);
}
void quadTree3d::setUp(vector3 dim, vector3 center, float visionRadius){
    quadrant = new boundingBox(dim, center);
    if(dim.x >= visionRadius * 2 && dim.y >= visionRadius * 2 && dim.z >= visionRadius * 2){
        leaf = false;
        children = new quadTree3d*[CHILDRENNUM];

        int i = 0;
        for (int dx = -1; dx <= 1; dx+=2)
            for (int dy = -1; dy <= 1; dy+=2)
                for (int dz = -1; dz <= 1; dz+=2, ++i){
                    vector3 childCenter(center.x + (dim.x/4 * dx), center.y + (dim.y/4 * dy), center.z + (dim.z/4 * dz));
                    children[i] = new quadTree3d(dim/2, childCenter, visionRadius);
                }
    }
    else{
        leaf = true;
    }   
}
void quadTree3d::draw(bool b){
    if(b){
        if(leaf){
            quadrant-> draw();
        }
        else{
            for (int i = 0; i < CHILDRENNUM; ++i){
                children[i]->draw(b);
            }    
        }
    }
    else{
        quadrant-> draw();
    }
}
void quadTree3d::drawLimits(){
    if(!leaf){
        quadrant-> draw();
    }
}

bool quadTree3d::insertBoids(){
    for (int i = 0; i < boidsAmount; ++i)
    {
        quadTree3d *tmp = getQuadrant(*boids[i]->position);
        if(tmp == NULL)
            return false;
        tmp->boids.push_back(boids[i]);
    }
    return true;
}
bool quadTree3d::insertPredators(){
    for (int i = 0; i < predatorsAmount; ++i)
    {
        quadTree3d *tmp = getQuadrant(*predators[i]->position);
        if(tmp == NULL)
            return false;
        tmp->predators.push_back(predators[i]);
    }
    return true;
}
quadTree3d *quadTree3d::getQuadrant(vector3 b){
    if(leaf){
        if(quadrant->containBoid(b)){
            return this;
        }
        else return NULL;
    }
    vector3 c = b - *quadrant->center;
    if(c.x < 0 && c.y < 0 && c.z < 0){
        quadTree3d * q = children[0]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x < 0 && c.y < 0 && c.z >= 0){
        quadTree3d * q = children[1]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x < 0 && c.y >= 0 && c.z < 0){
        quadTree3d * q = children[2]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x < 0 && c.y >= 0 && c.z >= 0){
        quadTree3d * q = children[3]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x >= 0 && c.y < 0 && c.z < 0){
        quadTree3d * q = children[4]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x >= 0 && c.y < 0 && c.z >= 0){
        quadTree3d * q = children[5]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else if(c.x >= 0 && c.y >= 0 && c.z < 0){
        quadTree3d * q = children[6]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    else{
        quadTree3d * q = children[7]->getQuadrant(b);
        if(q != NULL)
            return q;
    }
    return NULL;
}

void quadTree3d::getNeighborsQuadrants(vector < quadTree3d *> *neighbors, quadTree3d *q){
    for(int i = -1; i <= 1; i+=1)
        for(int j = -1; j <= 1; j+=1)
            for(int k = -1; k <= 1; k+=1){
                    vector3 tmp((q->quadrant->center->x + (q->quadrant->dim->x/2) * i) + (0.0001 * i),
                                 (q->quadrant->center->y + (q->quadrant->dim->y/2) * j) + (0.0001 * j),
                                 (q->quadrant->center->z + (q->quadrant->dim->z/2) * k) + (0.0001 * k));
                    quadTree3d *p = getQuadrant(tmp);
                    if(p) 
                        neighbors->push_back(p);
            }
}

quadTree3d *quadTree3d::getMostPopulatedRegion(quadTree3d *region){
    if(leaf){
        if(!region->leaf || (region->boids.size() < boids.size())){
            return this;
        }
        else {
            return region;
        }
    }
    else{
        quadTree3d *tmp = region;
        for(int i = 0; i < CHILDRENNUM; i++){
            tmp = children[i]->getMostPopulatedRegion(tmp);
        }
        return tmp;
    }
}

void quadTree3d::foo(float deltaTime){
    insertBoids();
    quadTree3d *mpr = getMostPopulatedRegion(this);
    insertPredators();
    vector < quadTree3d *> neighbors;
    vector < boid *> nboids;
    vector < predator *> nPredators;
    for (int i = 0; i < boidsAmount; ++i){
        getNeighborsQuadrants(&neighbors, getQuadrant(*boids[i]->position));
        if(!boids[i]->panic){
            for (int j = 0; j < neighbors.size(); ++j){
                for (int k = 0; k < neighbors[j]->predators.size(); ++k){
                    if (boids[i]->inRadius(neighbors[j]->predators[k], visionRadius*2) && boids[i]->inVisionRadius(neighbors[j]->predators[k])){
                        nPredators.push_back(neighbors[j]->predators[k]);
                    }
                }
                for (int k = 0; k < neighbors[j]->boids.size(); ++k){
                    if (boids[i]->inRadius(neighbors[j]->boids[k], visionRadius) && boids[i]->inVisionRadius(neighbors[j]->boids[k])){
                        nboids.push_back(neighbors[j]->boids[k]);
                    }
                }
            }
            boids[i]->drawNewPosition(deltaTime, nboids, nPredators);
        }
        else{
            for (int j = 0; j < neighbors.size(); ++j){
                for (int k = 0; k < neighbors[j]->predators.size(); ++k){
                    if (boids[i]->inRadius(neighbors[j]->predators[k], visionRadius * 2) && boids[i]->inVisionRadius(neighbors[j]->predators[k])){
                        nPredators.push_back(neighbors[j]->predators[k]);
                    }
                }
            }
            boids[i]->drawNewPosition(deltaTime, nPredators);
        }
            
        nboids.clear();
        nPredators.clear();
        neighbors.clear();
    }
    for (int i = 0; i < predatorsAmount; ++i){
        predators[i]->drawNewPosition(deltaTime, *mpr->quadrant->center);
    }
    clearVectors();
}

void quadTree3d::clearVectors(){
    if(leaf){
        boids.clear();
        predators.clear();
    }
    else{
        for (int i = 0; i < CHILDRENNUM; ++i){
            children[i]->clearVectors();
        }
    }
}
quadTree3d::~quadTree3d(){
    delete quadrant;
    for (int i = 0; i < CHILDRENNUM; ++i){
        delete children[i];
    }
    delete children;
    for (int i = 0; i < boids.size(); ++i){
        delete boids[i];
    }
    boids.clear();
}

#endif // QUADTREE_H