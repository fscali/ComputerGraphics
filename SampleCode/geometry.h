/**** Basic setup for defining and drawing objects ****/
// Moved to a header for the second OpenGL program

#ifndef __INCLUDEGEOMETRY
#define __INCLUDEGOEMETRY

const int numobjects = 2 ; // ** NEW ** number of objects for buffer 
const int numperobj  = 3 ; 
const int ncolors = 4 ; 
GLuint buffers[numperobj*numobjects+ncolors] ; // ** NEW ** List of buffers for geometric data 
GLuint objects[numobjects] ; // ** NEW ** For each object
GLenum PrimType[numobjects] ;
GLsizei NumElems[numobjects] ; 

// ** NEW ** Floor Geometry is specified with a vertex array
// ** NEW ** Same for other Geometry 
// The Buffer Offset Macro is from Red Book, page 103, 106

#define BUFFER_OFFSET(bytes) ((GLubyte *) NULL + (bytes))
#define NumberOf(array) (sizeof(array)/sizeof(array[0])) 

enum {Vertices, Colors, Elements} ; // For arrays for object 
enum {FLOOR, CUBE} ; // For objects, for the floor


const GLfloat floorverts[4][3] = {
  {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, {0.5, -0.5, 0.0}
} ; 
const GLfloat floorcol[4][3] = {
  {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}
} ; 
const GLubyte floorinds[1][4] = { {0, 1, 2, 3} } ; 
const GLfloat floortex[4][2] = { 
  {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}, {1.0, 0.0}
} ;

// CUBES FOR ADDING TO THE SCENE

const GLfloat wd = 0.1 ; 
const GLfloat ht = 0.5 ; 
const GLfloat _cubecol[4][3] = { 
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0} } ; 
const GLfloat cubeverts[8][3] = {
  {-wd, -wd, 0.0}, {-wd, wd, 0.0}, {wd, wd, 0.0}, {wd, -wd, 0.0},
  {-wd, -wd, ht}, {wd, -wd, ht}, {wd, wd, ht}, {-wd, wd, ht}
} ; 
GLfloat cubecol[8][3] ;
const GLubyte cubeinds[6][4] = { 
  {0, 1, 2, 3}, // BOTTOM 
  {4, 5, 6, 7}, // TOP 
  {0, 4, 7, 1}, // LEFT 
  {0, 3, 5, 4}, // FRONT
  {3, 2, 6, 5}, // RIGHT 
  {1, 7, 6, 2}  // BACK
} ; 


void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) ;
void initobjectnocol(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type) ;
void drawobject(GLuint object) ; 
void initcolorscube (void) ;
void drawcolor(GLuint object, GLuint color) ;

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 

void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) {
   int offset = object * numperobj ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
   glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
   glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_VERTEX_ARRAY) ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ; 
   glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
   glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_COLOR_ARRAY) ; 
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
   PrimType[object] = type ; 
   NumElems[object] = sizeind ; 
}


// Simple function to set the color separately.  Takes out colors
void initobjectnocol(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type) {
   int offset = object * numperobj ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
   glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
   glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_VERTEX_ARRAY) ; 
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
   PrimType[object] = type ; 
   NumElems[object] = sizeind ; 
}

// Simple function to init a bunch of color buffers for the cube 
void initcolorscube (void) {
  int base = numobjects * numperobj ; 
  for (int i = 0 ; i < ncolors ; i++) {
    for (int j = 0 ; j < 8 ; j++) 
      for (int k = 0 ; k < 3 ; k++) 
        cubecol[j][k] = _cubecol[i][k] ; 
    glBindBuffer(GL_ARRAY_BUFFER, buffers[base+i]) ; 
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubecol), cubecol ,GL_STATIC_DRAW);
    glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
    glEnableClientState(GL_COLOR_ARRAY) ;       
  }  
}
// And a function to draw with them, similar to drawobject but with color
void drawcolor(GLuint object, GLuint color) {
   int offset = object * numperobj ; 
   int base = numobjects * numperobj ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
   glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_VERTEX_ARRAY) ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[base+color]) ; // Set color correctly 
   glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_COLOR_ARRAY) ; 
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
   glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, BUFFER_OFFSET(0)) ; 
}


void drawobject(GLuint object) {
   int offset = object * numperobj ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ; 
   glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_VERTEX_ARRAY) ; 
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ; 
   glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ; 
   glEnableClientState(GL_COLOR_ARRAY) ; 
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ; 
   glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, BUFFER_OFFSET(0)) ; 
}

#endif
