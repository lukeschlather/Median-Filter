using namespace std;
// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include "ppm.h"

using namespace std;
int main(int argc,char** argv)
{
  if(argc!=7) {
    cout << "Usage: PokeImage <input image> <RGB> <value> <x> <y> <radius> " << endl;
  }
  cout << "Infile: " << argv[1] << endl;
  ppm image(argv[1]);
  colorchannel color=R; /*
  switch(*argv[2]) {
  case 'R':
    color=R;
    break;
  case 'G':
    color=G;
    break;
  case 'B':
    color=B;
    break;
  }
		      */
  int value=atoi(argv[3]);
  int x=atoi(argv[4]);
  int y=atoi(argv[5]);
  int radius=atoi(argv[6]);
  cout << "Radius:" << radius << endl;  
  string outfile= "changed_";
  outfile+=argv[2];
  outfile+="_r_";
  outfile+=argv[3];
  outfile+=".ppm";
  //image.reduceNoise(framesize,threshold);
  image.poke(color,value,x,y,radius);
  image.write(outfile.c_str());

  return 0;
}
