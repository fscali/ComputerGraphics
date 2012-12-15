
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include <FreeImage.h>
#include <math.h>
#include <cmath>
#include <limits>

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
#include "Transform.h"

#define DEV_MODE 0
#define DEV_MODE_WIDTH 160
#define DEV_MODE_HEIGHT 120

#define DEFAULT_ATTENUATION 1,0,0
	
#define	 DEFAULT_AMBIENT .2,.2,.2
#define DEFAULT_MAX_DEPTH 5
	
//#define EPSILON  0.000000000001

typedef struct {
	bool result;
	float t;
	Base* object;
	vec3 p_intersect;
	vec3 normal;
	vec3 direction;

} IntersectionResult;

int effectiveWidth;
int effectiveHeight;
const float  eps=1e-9;

void checkDevMode(){
	if (DEV_MODE){
		effectiveWidth = DEV_MODE_WIDTH;
		effectiveHeight = DEV_MODE_HEIGHT;
	}
	else{
		effectiveWidth = sceneWidth;
		effectiveHeight = sceneHeight;
	}
}

void checkDefaults(){

	if (outputFileName){
		std::cout << "Filename=" << outputFileName  << "\n";
	} else {

		outputFileName = "raytrace.png";
	}

}

//return the vector p1
vec3 rayThruPixel(int i, int j){
	float fovyRad = fovy * pi / 180.0;
	vec3 a = eyeinit - center;
	vec3 b = upinit;
	vec3 w = glm::normalize(a);
	vec3 cr = glm::cross(b,w);
	vec3 u = glm::normalize(cr);
	vec3 v = glm::cross(w,u);
	float aspectRatio = (float)effectiveWidth / effectiveHeight;
	float fovx = aspectRatio * fovyRad;


	float alpha = glm::tan((float)fovyRad/2) * aspectRatio * ((float)(j+0.5 -effectiveWidth/2)/((float)effectiveWidth / 2));
	float beta  = glm::tan((float)fovyRad/2) * ((float)(effectiveHeight/2 -(i+0.5))/((float)effectiveHeight/2));

	
	vec3 p0=eyeinit;

	vec3 p1=alpha*u + beta*v -w;

	return p1;
}

void transformTriangles(){
	for (int i=0; i<triangles.size(); i++){
		//std::cout << "Now transforming triangle " << i  << "\n";
		Triangle* t = &(triangles[i]);
		t->applyTransform();
	}

}

IntersectionResult* intersectWithTriangle(vec3 P0, vec3 direction, Triangle T){


	IntersectionResult* r = new IntersectionResult();
	r->result = false;

	vec3 Ap = vec3(T.A[0]/T.A[3], T.A[1]/T.A[3], T.A[2]/T.A[3]);
	vec3 Bp = vec3(T.B[0]/T.B[3], T.B[1]/T.B[3], T.B[2]/T.B[3]);
	vec3 Cp = vec3(T.C[0]/T.C[3], T.C[1]/T.C[3], T.C[2]/T.C[3]);

	vec3 normal = T.getNormal(Ap);

	float dotProduct = glm::dot(direction,normal);

	if (abs(dotProduct) < eps){
		std::cout << "dotProduct nullo"  << "\n";
		return r;
	}else{

		float t =  (glm::dot(Ap, normal) - glm::dot(P0,normal))/dotProduct;
		vec3 P = P0 + t * direction;
		vec3 R = P - Ap;
		vec3 Q1 = Bp - Ap;
		vec3 Q2 = Cp - Ap;

		float det = glm::dot(Q1,Q1)*glm::dot( Q2,Q2) - pow(glm::dot(Q1,Q2),2);

		if (det != 0){

			float scalar = 1 / det;

			mat2 m = mat2(
					glm::dot(Q2,Q2),              -glm::dot(Q1,Q2),
					-glm::dot(Q1,Q2),             glm::dot(Q1,Q1)
				     );

			vec2 v = vec2(glm::dot(R,Q1), glm::dot(R,Q2));
			vec2 w = m * v;
			w = scalar * w;

			if (w[0] >= 0 && w[1] >= 0 && (w[0] + w[1] <=1)){
				r->result = true;
				r->t = t;
				r->p_intersect = P0 + t * direction;
			}
		}
		else{
			std::cout << "Determinante nullo"  << "\n";
		}
		return r;
	}
}


IntersectionResult* intersectWithSphere(vec3 P0, vec3 direction, Sphere s){
	IntersectionResult* r = new IntersectionResult();
	r->result = false;

	mat4 transform = glm::inverse(s.transform);

	vec4 P0h = vec4(P0[0],P0[1],P0[2],1);
	vec4 dH  = vec4(direction[0], direction[1], direction[2], 0);

	P0h = transform * P0h;
	dH = transform * dH;
	vec4 ch = vec4(s.center[0], s.center[1], s.center[2], 1);

	float a = glm::dot(dH,dH);
	float b = 2 * glm::dot(dH , (P0h - ch));
	float c = glm::dot((P0h - ch),(P0h - ch)) - pow(s.radius,2);
	float delta = pow(b,2) - 4 * a * c;
	if (abs(delta) < eps){
		float t = -b / (2 * a);
		r->t = t;
		r->result = true;
	}
	else if (delta > 0){
		float t1 = -b/(2*a) + sqrt(delta)/(2*a);
		float t2 = -b/(2*a) - sqrt(delta)/(2*a);
		float t = (t1 < t2 ? t1 : t2);
		r->t = t;
		r->result = true;
	}

	if (r->result){
		vec4 P0ht = s.transform * P0h;
		vec4 dHt = s.transform * dH;
		
		vec4 Pt = P0ht + (r->t) * dHt;
		r->p_intersect = vec3(Pt[0]/Pt[3], Pt[1]/Pt[3], Pt[2]/Pt[3]);
	}

	return r;
}

IntersectionResult* findIntersection(vec3 P0, vec3 direction, bool verbose, bool setNormal, bool stopAtFirst, float maxDist){
	IntersectionResult* result = NULL;
	Base* hitObject = NULL;
	float minDist = numeric_limits<float>::max(); 
	bool found = false;
	for (int k=0; k<triangles.size(); k++){
		IntersectionResult* r = intersectWithTriangle(P0,direction,triangles[k]);
		if (r->result){

			if (r->t > 0 && r->t < minDist && (r->t < maxDist)){
				minDist = r->t;
				hitObject = &(triangles[k]);
				if (result){
					delete(result);
				}
				result = new IntersectionResult();
				result->t = minDist;
				result->object = hitObject;
				result->result = true;
				result->p_intersect = r->p_intersect;
				found =  true;
				if (stopAtFirst){
					break;

				}
			}
			free(r);
		}
		else if (r){

			free(r);
		}
	}
		if (!(stopAtFirst && found)){
			for (int k=0; k<spheres.size(); k++){
				IntersectionResult* r = intersectWithSphere(P0,direction,spheres[k]);
				if (r->result){

					if (r->t > 0 && r->t < minDist && (r->t < maxDist)){
						minDist = r->t;
						hitObject = &(spheres[k]);
						if (result){
							delete(result);
						}
						result = new IntersectionResult();
						result->t = minDist;
						result->object = hitObject;
						result->result = true;
						result->p_intersect = r->p_intersect; 
						found =  true;
						if (stopAtFirst){
							break;

						}
					}
				}
				free(r);


			}
		}

	if (result && result->result ){
		if (setNormal){

			result->normal = result->object->getNormal(result->p_intersect);
		}
		result->direction = direction;
	}

	return result;

}

BYTE* getRGBFromFloatingPoint(vec3 color){

	BYTE* res = (BYTE*)malloc(3*sizeof(BYTE));

	res[0] = (BYTE) (color[2] * 255);
	res[1] = (BYTE) (color[1] * 255);
	res[2] = (BYTE) (color[0] * 255);

	return res;

}

float getAttenuationTerm(float d){

	return 1.0/(attenuation[0] + attenuation[1] * d + attenuation[2] * pow(d,2));

}

vec3 getSpecularDirection(vec3 rayDirection, vec3 normal){
	
	vec3 res;
	vec3 dir = - rayDirection;
	float dot = glm::dot(dir, normal) ;
	res = 2*dot*normal - dir;
	res = glm::normalize(res);
	return res;


}

vec3 shade(IntersectionResult *r, int depth, vec3 pointOfView){

	vec3 color = vec3(0);
	if (depth){
		for (int i=0; i<numused; i++){

			float x = lightposn[4*i];
			float y = lightposn[4*i+1];
			float z = lightposn[4*i+2];
			float w = lightposn[4*i+3];
			vec3 direction, posLight;
			bool isDirectional=false;
			if (w != 0){
				x = x / w;
				y = y / w;
				z = z / w;
				posLight = vec3(x,y,z);
				direction =  ( posLight - r->p_intersect);

			} else{
				posLight = vec3(x,y,z);
				direction = posLight;
				isDirectional = true;

			}
			direction = glm::normalize(direction);

			vec3 offset = eps * direction;
			vec3 d = r->p_intersect + offset ; 
			float maxDist = sqrt(glm::dot(r->p_intersect -  posLight, r->p_intersect - posLight));
			IntersectionResult* isFree = findIntersection(d, direction,true,false,  true, maxDist);


			if (!isFree ||  !isFree->result   || (isFree->t < 0) || isFree->t > maxDist){

				float rr = lightcolor[4*i];
				float g = lightcolor[4*i+1];
				float b = lightcolor[4*i+2];
				vec3 I = vec3(rr,g,b);

				//diffuse
				vec3 mynormal =  r->normal;
				float dot = glm::dot(mynormal,direction);
				//std::cout << "Normal:" << mynormal.x << "," << mynormal.y << "," <<mynormal.z <<"; Dot:" << dot  << "\n";
				vec3 diffuseTerm = (dot > 0 ? r->object->diffuse * dot : vec3(0));

				//specular 
				//direction from the hit point to the point of
				//view
				vec3 eyedirn = glm::normalize(pointOfView - r->p_intersect);
				//half vector
				vec3 H = glm::normalize(direction  + eyedirn);

				dot = glm::dot(mynormal,H);
				vec3 specularTerm = (dot > 0 ? r->object->specular * glm::pow(dot,r->object->shininess) : vec3(0));
				float norm = sqrt(glm::dot(r->p_intersect - posLight, r->p_intersect - posLight));
				float attenuation = (isDirectional ? 1.0 :  getAttenuationTerm(norm));

				color = color + attenuation *  I * (diffuseTerm + specularTerm);
			}
			else{
				//no direct lightning
//				std::cout << "found intersection: original object= " << isFree->object->type << "original object pointer=" << isFree->object  << ",intersectedObject="  << r->object << ", t=" << isFree->t << "\n";

			}



		}

		color = color + r->object->ambient + r->object->emission;
		vec3 newDir=getSpecularDirection(r->direction, r->normal);
		vec3 offset = eps*newDir;
		vec3 d = r->p_intersect + offset;
		IntersectionResult*	newIntersection = findIntersection(d, newDir,false,true, false,numeric_limits<float>::max());
		if (newIntersection && newIntersection->result && (newIntersection->t > 0)){
			vec3 add = shade(newIntersection, depth-1, r->p_intersect);
			vec3 mySpecular = r->object->specular;

			color += mySpecular *  add;
		}


	}
	if (color[0] > 1) color[0] = 1;
	if (color[1] > 1) color[1] = 1;
	if (color[2] > 1) color[2] = 1;

	if (color[0] < 0) color[0] = 0;
	if (color[1] < 0) color[1] = 0;
	if (color[2] < 0) color[2] = 0;

	return color;
}


void rayTrace(BYTE* pixels){
	for (int i=0; i<effectiveHeight; i++){

		if (i>=95 && i <=410){
			std::cout <<  "Processing row " << i  << "\n";
			for (int j=0; j<effectiveWidth; j++){

				int base = (i * effectiveWidth + j)*3;
				if (i >= 95  && i<=410 && j >=90 && j<= 530){
					std::cout << "."   ;

					vec3 p1=rayThruPixel(i,j);
					IntersectionResult* r = findIntersection(eyeinit,p1,false,true,false, numeric_limits<float>::max());
					if (r && r->object){

						vec3 color = shade(r, maxDepth,eyeinit);
						BYTE* res = getRGBFromFloatingPoint(color);
						pixels[base] = res[0];;
						pixels[base+1] = res[1];
						pixels[base+2] = res[2];
					}
				}else{

					pixels[base] = 0;
					pixels[base+1] = 0;
					pixels[base+2] = 0;
				}

			}
			std::cout <<    "\n";
		}
	}
}



int main(int argc , char* argv[]){


	if (argc < 2){
		cerr << "Usage: raytrace inputfile\n";

	}
	else{
		attenuation = vec3(DEFAULT_ATTENUATION);
		ambient = vec3(DEFAULT_AMBIENT);

		maxDepth = DEFAULT_MAX_DEPTH;
		

		readfile(argv[1]);
		checkDevMode();
		checkDefaults();	
		transformTriangles();
		BYTE pixels[3*effectiveWidth*effectiveHeight];
		rayTrace(pixels);
		
		FreeImage_Initialise();
		FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, effectiveWidth, effectiveHeight,effectiveWidth*3, 24, 0xFF0000,0x00FF00, 0x0000FF, true);
		//FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, effectiveWidth, effectiveHeight,effectiveWidth*3, 24, 0x0000FF,0x00FF00, 0xFF0000, true);
		FreeImage_Save(FIF_PNG, img, outputFileName, 0);

		FreeImage_DeInitialise();

	}
	

	return 0;
}
