// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
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

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
  //the rotation axis is defined by the vector "up"
  mat3 R = rotate(-degrees, glm::normalize(up)); 

  eye = R * eye;
  up  = R * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.  
  //the rotation axis is defined by the vector that is orthogonal bot to the
  //vector "up" and "eye"
  vec3 axis = glm::normalize(glm::cross(up,eye));
  mat3 R = rotate(degrees, axis);
  eye = R * eye;

  up = glm::normalize(R*up);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
  // obtain the coordinate frame
  vec3 a = eye;
  vec3 b = up;
  
  vec3 w = glm::normalize(a);
  vec3 u = glm::normalize(glm::cross(b,w));
  vec3 v = glm::cross(w,u);
  
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
  ret  =  T * R;
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
  float radians = (fovy / 2) * pi / 180.0; 
  float cosine  = glm::cos(radians);
  float sine    = glm::sin(radians);
  float  d = cosine / sine;
  float a = -(zFar + zNear) / (zFar - zNear);
  float b = -(2 * zFar * zNear)/(zFar - zNear);
  ret = mat4(
		  d/aspect,   0,   0,   0,
		         0,   d,   0,   0,
			 0,   0,   a,   b,
			 0,   0,  -1,   0
	    );
   

    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 

    ret = mat4(
		    sx,  0,  0,  0,
		    0,  sy,  0,  0,
		    0,   0, sz,  0,
		    0,   0,  0,  1
	      );

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    ret = mat4(
		  1,   0,   0,	 tx,
		  0,   1,   0,   ty,
		  0,   0,   1,   tz,
		  0,   0,   0,     1
	      );
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
