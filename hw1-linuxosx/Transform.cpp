// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE

  float pi = 3.14159265;
  float radians = degrees * pi / 180.0;
  float cosine  = glm::cos(radians);
  float sine    = glm::sin(radians);
  

  mat3 identityMatrix = mat3(
		  1, 0, 0,
		  0, 1, 0,
		  0, 0, 1
		  );
  
  mat3 dualMatrix = mat3(
		  0, -axis[2], axis[1],
		  axis[2], 0,  -axis[0],
		  -axis[1], axis[0], 0
		  );

  mat3 aa_transpose = mat3(
		  axis[0]*axis[0], axis[0]*axis[1], axis[0]*axis[2],
		  axis[0]*axis[1], axis[1]*axis[1], axis[1]*axis[2],
		  axis[0]*axis[2], axis[1]*axis[2], axis[2]*axis[2]
		  );
  
  //Rodrigues Formula:
  mat3 result = identityMatrix * cosine + aa_transpose * (1 - cosine) +	  dualMatrix * sine;
  
  return result;
  
		  
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {

  //the rotation axis is defined by the vector "up"
  mat3 R = rotate(-degrees, glm::normalize(up)); 

  eye = R * eye;
  up  = R * up;


}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {


  //the rotation axis is defined by the vector that is orthogonal bot to the
  //vector "up" and "eye"
  vec3 axis = glm::normalize(glm::cross(up,eye));
  mat3 R = rotate(degrees, axis);
  eye = R * eye;

  up = glm::normalize(R*up);

}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // obtain the coordinate frame
  vec3 a = eye;
  vec3 b = up;
  
  vec3 w = glm::normalize(a);
  vec3 u = glm::normalize(glm::cross(b,w));
  vec3 v = glm::cross(w,u);
  
  //here we write the matrices in row major form 
  //because the main program automatically transpose 
  //the matrix
  mat4 R = mat4(
		  u.x, u.y, u.z, 0, 
		  v.x, v.y, v.z, 0,
		  w.x, w.y, w.z, 0,
		  0,   0,   0,   1 
	       );

  mat4 T = mat4(
		  1,   0,   0,	 -eye.x,
		  0,   1,   0,   -eye.y,
		  0,   0,   1,   -eye.z,
		  0,   0,   0,        1
	       );
  return  T * R;

}

Transform::Transform()
{

}

Transform::~Transform()
{

}
