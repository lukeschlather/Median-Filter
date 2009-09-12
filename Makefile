GENOPTS = -O3

OPTS = $(GENOPTS)
COPTS = $(GENOPTS) -c
INCL = -I/usr/common/include -I/usr/include/GL
#LIBS = -L/usr/X11R6/lib -lX11 -lglut -lGL -lGLU -lm #-lXi -lXmu
MYLIBS = LJFS_Utils.o Image.o 

CleanImage: CleanImage.cpp $(MYLIBS)
	g++ $(OPTS) $(INCL) -o CleanImage CleanImage.cpp $(MYLIBS) $(LIBS)

PokeImage: PokeImage.cpp ppm.o
	g++ $(OPTS) $(INCL) -o PokeImage PokeImage.cpp ppm.o $(LIBS)

ImageCopy: ImageCopy.cpp ppm.o
	g++ $(OPTS) $(INCL) -o ImageCopy ImageCopy.cpp ppm.o $(LIBS)

clean:
	-rm *.o ImageCopy PokeImage CleanImage

kill~: 
	-rm *~ *\#

ppm.o: ppm.h ppm.cpp
	g++ $(COPTS) ppm.cpp

Image.o: Image.h Image.cpp LJFS_Utils.o
	g++ $(COPTS) Image.cpp

LJFS_Utils.o: LJFS_Utils.h LJFS_Utils.cpp
	g++ $(COPTS) LJFS_Utils.cpp
