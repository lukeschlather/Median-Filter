
OPTS = -Wall -g -O3
INCL = -I/usr/common/include -I/usr/include/GL
#LIBS = -L/usr/X11R6/lib -lX11 -lglut -lGL -lGLU -lm #-lXi -lXmu

CleanImage: CleanImage.cpp ppm.o
	g++ $(OPTS) $(INCL) -o CleanImage CleanImage.cpp ppm.o $(LIBS)

PokeImage: PokeImage.cpp ppm.o
	g++ $(OPTS) $(INCL) -o PokeImage PokeImage.cpp ppm.o $(LIBS)

ImageCopy: ImageCopy.cpp ppm.o
	g++ $(OPTS) $(INCL) -o ImageCopy ImageCopy.cpp ppm.o $(LIBS)

clean:
	-rm *.o ImageCopy PokeImage CleanImage

kill~: 
	-rm *~ *\#

ppm.o: ppm.h ppm.cpp
	g++ $(OPTS) -c ppm.cpp

