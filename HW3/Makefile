# Jello cube Makefile 
# Jernej Barbic, USC

LIBRARIES = -framework OpenGL -framework GLUT 

COMPILER = g++ -lgsl -lgslcblas
COMPILERFLAGS = -O1

all: jello 

jello: jello.o showCube.o input.o physics.o perlinNoise.o ppm.o pic.o 
	$(COMPILER) $(COMPILERFLAGS) -o $@ $^ $(LIBRARIES)

jello.o: jello.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) jello.cpp
input.o: input.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) input.cpp
showCube.o: showCube.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) showCube.cpp
physics.o: physics.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) physics.cpp
perlinNoise.o: perlinNoise.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) perlinNoise.cpp

clean:
	-rm -rf core *.o *~ "#"*"#" test


