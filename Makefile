GENOPTS = -Wall -g

OPTS = $(GENOPTS)
COPTS = $(GENOPTS) -c
INCL = -I/usr/common/include -I/usr/include/GL

MYLIBS = LJFS_Utils.o Image.o 

CleanImage: CleanImage.cpp $(MYLIBS)
	g++ $(OPTS) $(INCL) -o CleanImage CleanImage.cpp $(MYLIBS) 

PokeImage: PokeImage.cpp ppm.o
	g++ $(OPTS) $(INCL) -o PokeImage PokeImage.cpp $(MYLIBS) 

ImageCopy: ImageCopy.cpp Image.o
	g++ $(OPTS) $(INCL) -o ImageCopy ImageCopy.cpp $(MYLIBS) 

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
