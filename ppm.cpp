// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include "ppm.h"
#include<cmath>
#include<set>
#include<cstdlib>
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

void ppm::destruct() {
  for (int y=0;y<height;++y) {
    delete [] data[y];
  }
  delete [] data;
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
  data = new colorRGB*[height];
  for (int y=0;y<height;++y) {
    data[y]=new colorRGB[width];
    for (int x=0;x<width;++x) {
      infile.get(data[y][x][R]);
      infile.get(data[y][x][G]);
      infile.get(data[y][x][B]);
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
      outfile.put(data[y][x][R]);
      outfile.put(data[y][x][G]);
      outfile.put(data[y][x][B]);
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
      total+=data[v][h][c];
      ++oobs;
    }
  }
  int average = total/(framesize*framesize);
  
  if(((int)abs(average-data[y][x][c]))>threshold) {
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
      values.insert(data[v][h][c]);
    }
  }
  unsigned int i=0;
  int newval=data[y][x][c];
  for(multiset<int>::iterator it=values.begin();i<values.size()/2;++i,++it) {
    newval=*it;
  }
  data[y][x][c]=newval;
}

void ppm::copy (const ppm& src) {
  ppmtype = src.ppmtype;
  height = src.height;
  width = src.width;
  bitdepth = src.bitdepth;
  framesize = src.framesize;
  data = src.data;
}

void ppm::poke(colorchannel color, char value, int x,int y, int radius) {
  int oobs=0;
  int xmax = radius+x;
  int ymax = radius+y;
  cout << "ymax: " << ymax << "xmax: " << xmax << endl;
  if (xmax > width) xmax=width;
  if (ymax > height) ymax=height;
    cout << "ymax: " << ymax << "xmax: " << xmax << endl;
    cout << "y: " << y << "x: " << x << endl;
  for(int i=y;i<ymax;++i) {
    for (int j=x;j<xmax;++j) {
      if((i<height)&&(j<width)) {
	data[i][j][R]=value;
	data[i][j][G]=0;
	data[i][j][B]=0;
      }
      else {
	++oobs;
      }
      //cout << "(" << i << "," << j << ")" << endl;
    }
  }
  cout << oobs << endl;
  cout << height << endl;
  cout << width << endl;
  write("text.ppm");
}
