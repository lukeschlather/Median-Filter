#ifndef _PPM_
#define _PPM_
#include<iostream>
#include<vector>
#include <fstream>
// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
using namespace std;
void checkContinue(string message);

enum colorchannel { R,G,B};

class ppm {
 protected:
  string ppmtype;
  int modifiedPixels;
  int height;
  int width;
  int bitdepth;
  int framesize;
  vector<vector<vector <char> > > image;
  public:
  ppm();
  ppm(const char* filename);
  ppm(ifstream infile);
  ppm(const ppm& src) {
    copy(src);
  }
  ppm& operator=(const ppm& src) {
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
