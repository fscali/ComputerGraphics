#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::vec4 vec4;


class Base{

public:
	virtual ~Base(){};
	vec3 diffuse;
	
	vec3 ambient;
	vec3 specular;
	vec3 emission;
	float shininess;

	char* type;
	mat4 transform;
	virtual vec3 getNormal(vec3 ) = 0;

};

//getNormal
class Sphere : public Base{
public:
	Sphere(vec3 , float );
	virtual	~Sphere();

	vec3 center;
	float radius;

	virtual vec3 getNormal(vec3 );
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//	vec3 emission;
//	float shininess;
//
//	mat4 transform;
};

class Triangle : public Base{
public:
	Triangle(vec4, vec4 , vec4 );
	//Triangle(unsigned int, unsigned int , unsigned int);
	virtual ~Triangle();

	void applyTransform();
	virtual vec3 getNormal(vec3 );
	void setNormal();
	vec3 getNormal();

	vec4 A,B,C;
	vec3 normal;
//	vec3 diffuse;
//	
//	vec3 ambient;
//	vec3 specular;
//	vec3 emission;
//	float shininess;
//
//	mat4 transform;
};

#endif
