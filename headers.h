#ifndef HEADERS_H
#define HEADERS_H
#include <GL/glut.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#define PI 3.14159265359
using namespace std;
struct vector3{
	float x;
	float y;
	float z;
	vector3();
	vector3(float x, float y, float z);
	vector3 operator+(vector3 vector);
	vector3 operator*(float scalar);
	vector3 operator/(float scalar);
	vector3 operator-(vector3 vector);
	float operator*(vector3 vector);
	float magnitude();
	void print();
	~vector3();
};
vector3::vector3(){
	x = y = z = 0;
}
vector3::vector3(float x, float y, float z){
	this-> x = x;
	this-> y = y;
	this-> z = z;
}
vector3 vector3::operator+(vector3 vector){
	vector3 result(this->x + vector.x, this->y + vector.y, this->z + vector.z); 
	return result;
}
vector3 vector3::operator-(vector3 vector){
	vector3 result(this->x - vector.x, this->y - vector.y, this->z - vector.z); 
	return result;
}
vector3 vector3::operator*(float scalar){
	vector3 result(this->x * scalar, this->y * scalar, this->z * scalar);
	return result;
}
vector3 vector3::operator/(float scalar){
	vector3 result(this->x / scalar, this->y / scalar, this->z / scalar);
	return result;
}
float vector3::operator*(vector3 vector){
	float result = this->x * vector.x + this->y * vector.y + this->z * vector.z; 
	return result;
}
float vector3::magnitude(){
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}
void vector3::print(){
	cout << "x:" << x << ",y:" << y << ",z:" << z << endl;
}
vector3::~vector3(){

}

#endif