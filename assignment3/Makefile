# Simple Makefile for 6.837 assignment 0
# for g++ on linux platforms

# ===============================================================

SRCS 	 	= main.cpp \
		  matrix.cpp \
		  image.cpp \
		  scene_parser.cpp \
		  Object3D/sphere.cpp \
		  camera.cpp \
		  Object3D/group.cpp\
		  Object3D/plane.cpp \
		  Object3D/triangle.cpp \
		  Object3D/transform.cpp \
		  glCanvas.cpp \
		  material.cpp \
		  light.cpp
		  

# ===============================================================

LIBS            = -lm -lGL -lGLU -lglut -L/usr/lib
CC		= g++
INCLUDE 	= -I/usr/include
EXE 	  	= raytracer
OBJS =  $(SRCS:.cpp=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE) 

depend:
	$(CC) -E -M $(INCLUDE) $(SRCS) > Makefile.depend

clean: 
	rm -f *~ *bak *.o  $(EXE) Makefile.depend Object3D/*.o

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

# ===============================================================

-include Makefile.depend

# DO NOT DELETE
