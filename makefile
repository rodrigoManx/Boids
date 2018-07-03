CFLAGS = -Wall 
LIBS = -lGL -lGLU -lglut
SOURCES = boids.cpp
EXECUTABLE = boids
build: $(SOURCES)
	   g++ $(SOURCES) -o $(EXECUTABLE) $(LIBS)   
