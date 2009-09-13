#ifndef _IMAGE_
#define _IMAGE_
#include<iostream>
#include<vector>
#include <fstream>
#include<cstdlib>
// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
using namespace std;


enum colorchannel { R,G,B};
struct colorRGB {
  char red;
  char green;
  char blue;
  char& operator[](const colorchannel chan) { 
    return ((char*)this)[chan];
  }
};

class Image {
 protected:
  int modifiedPixels;
  int height;
  int width;
  int bitdepth;
  int framesize;
  colorRGB **data;
  public:
  Image();
  ~Image() { destruct(); };
  Image(const char* filename);
  Image(ifstream infile);
  Image(const Image& src) {
    copy(src);
  }
  void destruct();
  Image& operator=(const Image& src) {
    //    destruct();
    copy(src);
    return *this;
  }
  void copy(const Image& src);

  void write (string filename);
  void write(char const*  filename)  { write(string(filename));}
  void writePPM(ostream& outfile);
  void writeJPEG(ostream& outfile);
  
  void read(const string filename);
  void read(char const* filename){ read(string(filename));}
  void readPPM(istream& infile);
  void readJPEG(const char* filename);
  
  void reduceNoise(int framesize,int threshold);
  void reduceNoiseAlt(int framesize,int threshold);
  bool needsCleaning(int threshold,int x,int y,colorchannel c);
  void clean(int threshold,int x, int y,colorchannel c);
  void poke(colorchannel c, char val,int x,int y, int radius);

};

#endif // _IMAGE_
