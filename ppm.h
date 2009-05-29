#ifndef _PPM_
#define _PPM_
#include<iostream>
#include<vector>
#include <fstream>
#include<cstdlib>
// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
using namespace std;
void checkContinue(string message);

enum colorchannel { R,G,B};
struct colorRGB {
  char red;
  char green;
  char blue;

  char& operator[](const colorchannel chan) { 
    return ((char*)this)[chan];
    // switch (chan) {
    // case R:
    //   return red;
    // case G:
    //   return green;
    // case B:
    //   return blue;
    // default:
    //   throw 1;
    // }
    return red;
  }
};

class ppm {
 protected:
  string ppmtype;
  int modifiedPixels;
  int height;
  int width;
  int bitdepth;
  int framesize;
  colorRGB **data;
  public:
  ppm();
  ~ppm() { destruct(); };
  ppm(const char* filename);
  ppm(ifstream infile);
  ppm(const ppm& src) {
    copy(src);
  }
  void destruct();
  ppm& operator=(const ppm& src) {
    //    destruct();
    copy(src);
    return *this;
  }
  void copy(const ppm& src);
  void write(const char* filename);
  void write(ofstream outfile);
  void read(const char* infile);
  void reduceNoise(int framesize,int threshold);
  void reduceNoiseAlt(int framesize,int threshold);
  bool needsCleaning(int threshold,int x,int y,colorchannel c);
  void clean(int threshold,int x, int y,colorchannel c);
  void poke(colorchannel c, char val,int x,int y, int radius);

};

#endif // _PPM_
