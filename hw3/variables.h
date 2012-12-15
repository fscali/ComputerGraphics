#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Primitives.hpp"
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

EXTERN int sceneWidth;
EXTERN int sceneHeight;

EXTERN int maxDepth;
EXTERN char* outputFileName;
EXTERN vec3 eyeinit;
EXTERN vec3 center;
EXTERN vec3 upinit;
EXTERN float fovy;
EXTERN unsigned int maxVerts;
EXTERN unsigned int maxVertsNorm;
EXTERN std::vector<Sphere> spheres;
EXTERN std::vector<Triangle> triangles;
EXTERN std::vector<vec4> vertices;
EXTERN glm::vec3 attenuation; 
EXTERN glm::vec3 ambient;
EXTERN glm::vec3 diffuse;
EXTERN glm::vec3 specular;
EXTERN glm::vec3 emission;
EXTERN float shininess;

const int numLights = 10;
EXTERN float lightposn[numLights*4];
EXTERN float lightcolor[numLights*4];

EXTERN int numused;


