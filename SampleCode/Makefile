CC = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I/usr/X11/include -DOSX
LDFLAGS = -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lGL -lGLU -lm -lstdc++ 

CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11R6/include -I/sw/include \
	                                          -I/usr/sww/include -I/usr/sww/pkg/Mesa/include
        LDFLAGS = -L./lib/nix -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		                                                  -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lGLEW -lfreeimage



RM = /bin/rm -f
all: mytest1 mytest2 mytest3
mytest1: mytest1.o shaders.o shaders.h 
	$(CC) $(CFLAGS) -o mytest1 shaders.o mytest1.o $(LDFLAGS) 
mytest1.o: mytest1.cpp shaders.h 
	$(CC) $(CFLAGS) -c mytest1.cpp 
mytest2: mytest2.o shaders.o shaders.h geometry.h 
	$(CC) $(CFLAGS) -o mytest2 shaders.o mytest2.o $(LDFLAGS) 
mytest2.o: mytest2.cpp shaders.h geometry.h
	$(CC) $(CFLAGS) -c mytest2.cpp 
mytest3: mytest3.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o mytest3 shaders.o mytest3.o $(LDFLAGS) 
mytest3.o: mytest3.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) -c mytest3.cpp 
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) -c shaders.cpp 
clean: 
	$(RM) *.o mytest1 mytest2 mytest3
 
