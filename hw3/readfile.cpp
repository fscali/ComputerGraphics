#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "variables.h"
#include <stdlib.h>
#include "Transform.h"
#include "Primitives.hpp"
using namespace std;
using namespace glm;

void multiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

bool readvals(stringstream &s, const int numvals, float *values){
	for (int i=0; i<numvals; i++){
		s >> values[i];
		if (s.fail()){
			std::cout << "Failed reading value" << i << "will skip\n";
			return false;
		}
	}
	return true;
}
bool readvalsi(stringstream &s, const int numvals, unsigned int *values){
	for (int i=0; i<numvals; i++){
		s >> values[i];
		if (s.fail()){
			std::cout << "Failed reading value" << i << "will skip\n";
			return false;
		}
	}
	return true;
}


void readfile(const char *filename){
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()){
		
		stack<mat4> transfstack;
		transfstack.push(mat4(1.0));
		getline(in, str);
		numused = 0;
		while(in){

			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
			//ruled out comment and blank lines
				stringstream s(str);
				s >> cmd;

				float values [10];
				unsigned int ivalues [10];
				char *string_values[10];
				bool validinput;
				if (cmd =="directional" || cmd == "point"){
					if (numused > numLights){
						cerr << "Reached Maximum Number of Lights \n";

					} else{
						validinput = readvals(s,6,values);
						if (validinput){
							for (int i=0; i<3; i++){
								lightposn[4*numused+i] = values[i];

							}
							lightposn[4*numused+3] = (cmd == "directional" ? 0.0 : 1.0);

							for (int i=0; i<3; i++){
								lightcolor[4*numused+i] = values[3+i];
							}
							lightcolor[3] = 1.0;
							numused++;

						}

					}
				}
				if (cmd == "size") {
					validinput = readvals(s, 2, values);
					if (validinput){
						sceneWidth = values[0];
						sceneHeight = values[1];
						std::cout << "Read width=" << sceneWidth << ", height=" << sceneHeight  << "\n";
					}

				}
				else if (cmd == "maxdepth"){
					validinput = readvals(s,1,values);
					if (validinput){
						maxDepth=values[0];

					}
				}
				else if (cmd == "output"){
					outputFileName = (char*) malloc(100);
					s >> outputFileName;
				} else if (cmd == "sphere") {
					validinput = readvals(s,4,values); // 
					if (validinput) {

						vec3 centerOfSphere = vec3(values[0],values[1],values[2]);
						Sphere s =  Sphere(centerOfSphere,values[3]);
						s.diffuse = diffuse;
						s.specular = specular;
						s.emission = emission;
						s.shininess = shininess;
						s.ambient = ambient;
						s.transform = transfstack.top();
						spheres.push_back(s);

					}
				} else if (cmd == "maxverts") {
					validinput = readvals(s,1,values); // 
					if (validinput) {

						maxVerts = values[0];


					}
				} else if (cmd == "maxvertsnorm") {
					validinput = readvals(s,1,values); // 
					if (validinput) {

						maxVertsNorm = values[0];


					}
				} else if (cmd == "vertex") {
					validinput = readvals(s,3,values); // 
					if (validinput) {
						vec4 vertex = vec4(values[0],values[1],values[2],1.0);
						vertices.push_back(vertex);
					}

				} else if (cmd == "tri") {
					validinput = readvalsi(s,3,ivalues); // 
					if (validinput) {
						vec4 vertex0 = vertices[ivalues[0]];
						vec4 vertex1 = vertices[ivalues[1]];
						vec4 vertex2 = vertices[ivalues[2]];

						Triangle t = Triangle(vertex0, vertex1, vertex2);
						t.diffuse = diffuse;
						t.specular = specular;
						t.emission = emission;
						t.shininess = shininess;
						t.ambient = ambient;
						t.transform = transfstack.top();
						triangles.push_back(t);
					}

				} else if (cmd == "vertexnormal") {
					//TODO
				} else if (cmd == "trinormal") {
					//TODO

				} else if (cmd == "translate") {
					validinput = readvals(s,3,values); 
					if (validinput) {

						mat4  tMatrix = Transform::translate(values[0], values[1], values[2]);
						multiply(glm::transpose(tMatrix), transfstack);
					}
				} else if (cmd == "scale") {
					validinput = readvals(s,3,values); 
					if (validinput) {

						mat4 scaleMatrix = Transform::scale(values[0], values[1], values[2]);
						multiply(glm::transpose(scaleMatrix), transfstack);

					}
				} else if (cmd == "rotate") {
					validinput = readvals(s,4,values); 
					if (validinput) {

						vec3 axis = vec3(values[0], values[1], values[2]);
						axis = glm::normalize(axis);

						mat3 r = Transform::rotate(values[3], axis);
						mat4 r4 = mat4(
								r[0][0], r[0][1], r[0][2], 0,
								r[1][0], r[1][1], r[1][2], 0,
								r[2][0], r[2][1], r[2][2], 0,
								0,       0,       0, 1
							      );

						multiply(glm::transpose(r4), transfstack);
					}
				} else if (cmd == "pushTransform") {
					transfstack.push(transfstack.top()); 
				} else if (cmd == "popTransform") {
					if (transfstack.size() <= 1) {
						cerr << "Stack has no elements.  Cannot Pop\n"; 
					} else {
						transfstack.pop(); 
					}
				} else if (cmd == "attenuation") {
					validinput = readvals(s,3,values);
					if (validinput){

						attenuation = vec3(values[0], values[1], values[2]);
					}
				} else if (cmd == "ambient") {
					validinput = readvals(s,3,values);
					if (validinput){

						ambient = vec3(values[0], values[1], values[2]);
					}
				} else if (cmd == "camera") {
					validinput = readvals(s,10,values); // 10 values eye cen up fov
					if (validinput) {


						eyeinit.x = values[0];
						eyeinit.y = values[1];
						eyeinit.z = values[2];

						center.x  = values[3];
						center.y  = values[4];
						center.z  = values[5];

						upinit.x  = values[6];
						upinit.y  = values[7];
						upinit.z  = values[8];

						upinit = Transform::upvector(upinit, eyeinit - center);

						fovy      = values[9];



					}
				} else if (cmd == "diffuse") {
					validinput = readvals(s,3,values);
					if (validinput){

						diffuse = vec3(values[0], values[1], values[2]);
					}

				} else if (cmd == "specular") {
					validinput = readvals(s,3,values);
					if (validinput){

						specular = vec3(values[0], values[1], values[2]);
					}

				} else if (cmd == "emission") {
					validinput = readvals(s,3,values);
					if (validinput){

						emission = vec3(values[0], values[1], values[2]);
					}

				} else if (cmd == "shininess") {
					validinput = readvals(s,1,values);
					if (validinput){

						shininess = values[0];
					}

				}
			}
			getline (in, str); 
		}


	}else{
		cerr << "Unable to Open Input Data File " << filename << "\n"; 
		throw 2; 
	}	



}

