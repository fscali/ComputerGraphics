/***************************************************************************/
/* This is a simple demo program written for CS 184 by Ravi Ramamoorthi    */
/* This program corresponds to the first OpenGL lecture.                   */
/*                                                                         */
/* Successive lectures/iterations make this program more complex.          */
/* This is the first simple program to draw a ground plane allowing zooming*/  
/* The intent is to show how to draw a simple scene.                       */
/***************************************************************************/


// I start with the modified first opengl program to do shaders
// I am now trying to do vertices/pixels in a modern way. 
// I have two planes

// I continue to use OpenGL matrix stacks as they are a good teaching tool, 
// even though they are deprecated in modern versions.

#include <stdlib.h>
#include <GL/glut.h>
#include "shaders.h"

int mouseoldx, mouseoldy ; // For mouse motion
GLdouble eyeloc = 2.0 ; // Where to look from; initially 0 -2, 2
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders

/***************   BASIC SET UP FOR DRAWING OBJECTS ***********************/

const int numobjects = 2 ; // number of objects for buffer 
const int numperobj  = 3 ; // Vertices, colors, indices
GLuint buffers[numperobj] ; // List of buffers for geometric data 
GLuint objects[numobjects] ; // For each object
GLenum PrimType[numobjects] ; // Primitive Type (quads, polygons etc.)
GLsizei NumElems[numobjects] ; // Number of geometric elements

// Floor Geometry is specified with a vertex array
// The Buffer Offset Macro is from Red Book, page 103, 106
// Note for more complex objects the indices must be integers, not bytes.

#define BUFFER_OFFSET(bytes) ((GLubyte *) NULL + (bytes))
#define NumberOf(array) (sizeof(array)/sizeof(array[0])) 

enum {Vertices, Colors, Elements} ; // For arrays for object 
enum {FLOOR, FLOOR2} ; // For objects, for the floor

//note that the eye location is {0,-2,2}, so the point with negative y 
//are nearer
const GLfloat floorverts[4][3] = {
  {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}, {-0.5, -0.5, 0.0}, {0.5, -0.5, 0.0}
  //right furthest, left furthest, left nearest, right nearest
} ; 
/*   const GLfloat floorcol[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}
} ; */
   const GLfloat floorcol[4][3] = {
  {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}
} ; 

//const GLfloat floorcol[4][3] = {
//  {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}
//} ; 

const GLubyte floorinds[1][4] = { {0, 1, 2, 3} } ; 

const GLfloat floorverts2[4][3] = {
  {0.5, 0.5, 1.0}, {-0.5, 0.5, 1.0}, {-0.5, -0.5, 1.0}, {0.5, -0.5, 1.0}
} ; 
const GLfloat floorcol2[4][3] = {
  {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
} ; 
const GLubyte floorinds2[1][4] = { {0, 1, 2, 3} } ; 

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

/****************   BASIC SETUP FOR DRAWING OBJECTS ***********************/


void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   // draw polygon (square) of unit length centered at the origin
   // Note that vertices must generally go counterclockwise
   // This code draws each vertex in a different color.  
   // The hardware will blend between them.  
   // This is a useful debugging trick.  I can make sure that each vertex 
   // appears exactly where I expect it to appear.

   // The old OpenGL code of using glBegin... glEnd no longer appears. 
   // The new version uses vertex buffer objects from init.   

   drawobject(FLOOR) ;
   drawobject(FLOOR2) ; 


/* don't wait!  
 * start processing buffered OpenGL routines 
 */
   glFlush ();
}

/* Defines a Mouse callback to zoom in and out */
/* This is done by modifying gluLookAt         */
/* The actual motion is in mousedrag           */
/* mouse simply sets state for mousedrag       */
void mouse(int button, int state, int x, int y) 
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_UP) {
      // Do Nothing ;
    }
    else if (state == GLUT_DOWN) {
      mouseoldx = x ; mouseoldy = y ; // so we can move wrt x , y 
    }
  }
  else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
    { // Reset gluLookAt
      eyeloc = 2.0 ; 
      glMatrixMode(GL_MODELVIEW) ;
      glLoadIdentity() ;
      gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1) ;
      glutPostRedisplay() ;
    }
}

void mousedrag(int x, int y) {
  int yloc = y - mouseoldy  ;    // We will use the y coord to zoom in/out
  eyeloc  += 0.005*yloc ;         // Where do we look from
  if (eyeloc < 0) eyeloc = 0.0 ;
  mouseoldy = y ;

  /* Set the eye location */
  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;
  gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1) ;
  
  glutPostRedisplay() ;
}

/* Defines what to do when various keys are pressed */
void keyboard (unsigned char key, int x, int y) 
{
  switch (key) {
  case 27:  // Escape to quit
    exit(0) ;
    break ;
  default:
    break ;
  }
}

/* Reshapes the window appropriately */
void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Think about the rationale for this choice for gluPerspective 
   // What would happen if you changed near and far planes? 
   gluPerspective(30.0, (GLdouble)w/(GLdouble)h, 1.0, 10.0) ;

}


void init (void) 
{
/* select clearing color 	*/
   glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Think about this.  Why is the up vector not normalized?
   glMatrixMode(GL_MODELVIEW) ;
   glLoadIdentity() ;
   //gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1) ;
   gluLookAt(0,-eyeloc,eyeloc,0,0,0,0,1,1) ;
   

   // Set up the Geometry for the scene.  
   // From OpenGL book pages 103-109

   
   
   glGenBuffers(numperobj*numobjects, buffers) ; 

   initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof (floorcol), (GLubyte *) floorinds, sizeof (floorinds), GL_POLYGON) ; 
   initobject(FLOOR2, (GLfloat *) floorverts2, sizeof(floorverts2), (GLfloat *) floorcol2, sizeof (floorcol2), (GLubyte *) floorinds2, sizeof (floorinds2), GL_POLYGON) ; 

   vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/nop.vert") ;
   fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/nop.frag") ;
   shaderprogram = initprogram(vertexshader, fragmentshader) ;  
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   // Requests the type of buffers (Single, RGB).
   // Think about what buffers you would need...

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB) ;

   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Simple Demo with Shaders");
   init (); // Always initialize first

   // Now, we define callbacks and functions for various tasks.
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape) ;
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse) ;
   glutMotionFunc(mousedrag) ;

   glutMainLoop(); // Start the main code
   return 0;   /* ANSI C requires main to return int. */
}
