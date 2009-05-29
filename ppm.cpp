// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include "ppm.h"
#include<cmath>
#include<set>
using namespace std;


void checkContinue(string message) {
  string input;
  cout << "Warning: " << message << endl;
  cout << "Continue: (y/n)" << endl;
  cin >> input;
  if ((input!="y")&&(input!="Y")) {
    exit(1);
  }
}

ppm::ppm(const char* filename) {
  read(filename);    
}

void ppm::read(const char* filename) {
  ifstream infile(filename);
  infile >> ppmtype;
  cout << ppmtype << endl;
  if(ppmtype!="P6") {
    checkContinue("The input file is not a P6 PPM");
  }
  infile >> width;
  infile >> height;
  infile >> bitdepth;
  if(bitdepth!=255) {
    checkContinue("The input depth is not 255."); 
  }
  cout << height << " x " << width << endl;
  cout << "Bit Depth: " << bitdepth << endl;
  infile.get(); //Discard the newline
  image.resize(height);
  for (int y=0;y<height;++y) {
    image[y].resize(width);
    for (int x=0;x<width;++x) {
      image[y][x].resize(3);
      infile.get(image[y][x][R]);
      infile.get(image[y][x][G]);
      infile.get(image[y][x][B]);
    }
  }
}


void ppm::write(const char * filename) {
  ofstream outfile(filename);
  outfile << ppmtype << endl;
  outfile << width << " " << height << endl;
  outfile << bitdepth << endl;;
  for (int y=0;y<height;++y) {
    for (int x=0;x<width;++x) {
      outfile.put(image[y][x][R]);
      outfile.put(image[y][x][G]);
      outfile.put(image[y][x][B]);
    }
  }
  
}
void ppm::reduceNoise(int fs,int threshold) {
  framesize=fs;
  modifiedPixels=0;
  ppm output(*this);
  cout << "Processing: " << endl;
  cout << "[=";
  int step = floor(height/40);
  for (int y=0;y<height;++y) {
    if(!(y%step)) {
      cout << "="; cout.flush();
    }
    for (int x=0;x<width;++x) {
      if(needsCleaning(threshold,x,y,R)) {
	if(needsCleaning(threshold,x,y,G)) {
	  if(needsCleaning(threshold,x,y,B)) {
	    output.clean(threshold,x,y,R);
	    output.clean(threshold,x,y,G);
	    output.clean(threshold,x,y,B);
	  }
	}
      }
    }
  }
  cout << "=]" << endl;
  output.write("test.ppm");
  *this=output;
}

void ppm::reduceNoiseAlt(int fs,int threshold) {
  framesize=fs;
  modifiedPixels=0;
  ppm output(*this);
  cout << "Processing: " << endl;
  cout << "[=";
  int step = floor(height/40);
  for (int y=0;y<height;++y) {
    if(!(y%step)) {
      cout << "="; cout.flush();
    }
    for (int x=0;x<width;++x) {
      if(needsCleaning(threshold,x,y,R)) {
	output.clean(threshold,x,y,R); 
      }
      if(needsCleaning(threshold,x,y,G)) {
	output.clean(threshold,x,y,G);
      }
      if(needsCleaning(threshold,x,y,B)) {
	output.clean(threshold,x,y,B);
      }

    }
  }
  cout << "=]" << endl;
  output.write("test.ppm");
  *this=output;
}

void ReplaceIfOOBs(const int lower,const int upper, int& val) {
  if(val<lower) {
    val=lower;
  }
  if(val>upper) {
    val=upper;
  }
}

bool ppm::needsCleaning(int threshold,int x,int y,colorchannel c) {
  int minX = x-framesize;
  int minY = y-framesize;
  int maxX = x+framesize;
  int maxY = y+framesize;
  ReplaceIfOOBs(0,height,minY);
  ReplaceIfOOBs(0,height,maxY);
  ReplaceIfOOBs(0,width,maxX);
  ReplaceIfOOBs(0,width,minX);
  int total=0;
  int oobs=0;
  for (int v=minY;v<maxY;v++) {
    for (int h=minX;h<maxX;h++) {
      total+=image[v][h][c];
      ++oobs;
    }
  }
  int average = total/(framesize*framesize);
  
  if(abs(average-image[y][x][c])>threshold) {
    ++modifiedPixels;
    return true;
  }
  else {
    return false;
  }
}
void ppm::clean(int threshold,int x, int y,colorchannel c) {
  int minX = x-framesize;
  int minY = y-framesize;
  int maxX = x+framesize;
  int maxY = y+framesize;
  ReplaceIfOOBs(0,height,minY);
  ReplaceIfOOBs(0,height,maxY);
  ReplaceIfOOBs(0,width,maxX);
  ReplaceIfOOBs(0,width,minX);
  multiset<int> values;
  for (int v=minY;v<maxY;v++) {
    for (int h=minX;h<maxX;h++) { 
      values.insert(image[v][h][c]);
    }
  }
  int i=0;
  int newval=image[y][x][c];
  for(multiset<int>::iterator it=values.begin();i<values.size()/2;++i,++it) {
    newval=*it;
  }
  image[y][x][c]=newval;
}

void ppm::copy (const ppm& src) {
  ppmtype = src.ppmtype;
  height = src.height;
  width = src.width;
  bitdepth = src.bitdepth;
  framesize = src.framesize;
  image = src.image;
}

void ppm::poke(colorchannel color, char value, int x,int y, int radius) {
  int oobs=0;
  for(int i=y;i<radius;++i) {
    for (int j=x;j<radius;++j) {
      if((i<height)&&(j<width)) {
	image[i][j][R]=value;
	image[i][j][G]=0;
	image[i][j][B]=0;
      }
      else {
	++oobs;
      }
    }
  }
  cout << oobs << endl;
  cout << image.size() << endl;
  cout << image[0].size() << endl;
  write("text.ppm");
}
