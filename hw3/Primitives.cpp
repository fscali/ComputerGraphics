#include "Transform.h"
#include "Primitives.hpp"

Sphere::Sphere(vec3 center, float radius){

	this->center = center;
	this->radius = radius;
	this->type = "sphere";
}


vec3 Sphere::getNormal(vec3 P){
	vec4 Papp = vec4(P[0], P[1], P[2], 1);

	Papp = glm::inverse(this->transform) * (Papp);
	float xApp = Papp[0] / Papp[3];
	float yApp = Papp[1] / Papp[3];
	float zApp = Papp[2] / Papp[3];
	vec3 P3app = vec3(xApp, yApp, zApp);

	vec3 diff = P3app - this->center;
	//vec3 normal =  glm::normalize(diff);
	vec3 normal =  glm::normalize(diff);
//	vec3 normal =  diff;
	vec4 n4 = vec4(normal[0], normal[1], normal[2], 0);
	n4 = glm::transpose(glm::inverse(this->transform)) * n4;
      
	float x = n4[0] ;
	float y = n4[1] ;
	float z = n4[2] ;
	normal = glm::normalize(vec3(x,y,z));
	return normal;

}

Triangle::Triangle(vec4 A, vec4 B, vec4 C){
	this->A = A;
	this->B = B;
	this->C = C;
	this->type = "triangle";
}

vec4 getTransformedVertex(vec4 v, mat4 m){
	vec4 aux =  m * v;
	float x = aux.x / aux.w;
	float y = aux.y / aux.w;
	float z = aux.z / aux.w;
	vec3 res = vec3(x,y,z);
	return aux;
	
}
void Triangle::applyTransform(){

	this->A = getTransformedVertex(this->A,this->transform);
	this->B = getTransformedVertex(this->B,this->transform);
	this->C = getTransformedVertex(this->C,this->transform);

	this->setNormal();
}

void Triangle::setNormal(){
	this->normal = getNormal();

}

vec3 Triangle::getNormal(){
	vec3 Ap = vec3(A[0],A[1],A[2]);
	vec3 Bp = vec3(B[0],B[1],B[2]);
	vec3 Cp = vec3(C[0],C[1],C[2]);
	vec3 normal = glm::cross((Bp - Ap),(Cp - Ap));
	normal = glm::normalize(normal);

	return normal;
	
	

}

vec3 Triangle::getNormal(vec3 P){
//g	vec3 Ap = vec3(A[0],A[1],A[2]);
//g	vec3 Bp = vec3(B[0],B[1],B[2]);
//g	vec3 Cp = vec3(C[0],C[1],C[2]);
//g	vec3 normal = glm::cross((Bp - Ap),(Cp - Ap));
//g	normal = glm::normalize(normal);
//g
//g	return normal;

	return this->normal;

}


Sphere::~Sphere(){

}

Triangle::~Triangle(){

}

