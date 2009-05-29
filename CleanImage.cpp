using namespace std;
// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include "ppm.h"

using namespace std;

string filenameSansExtension(string filename) {
  int i=0;
  for (i=filename.size()-1;i>=0;--i) {
    if(filename[i]=='.') {
      break;
    }
  }
  return filename.substr(0,i);
}

int main(int argc,char** argv)
{
  if(argc!=4) {
    cout << "Usage: ReduceImageNoise <input image> <framesize> <threshold> " << endl;
  }
  cout << "Infile: " << argv[1] << endl;
  ppm image(argv[1]);
  //Framesize,threshold
  int framesize=atoi(argv[2]);
  int threshold=atoi(argv[3]);
  cout << "Framesize:" << framesize << endl;
  cout << "Threshold:" << threshold << endl;
  string outfile= filenameSansExtension(argv[1]) + "-f_";
  outfile+=argv[2];
  outfile+="_t_";
  outfile+=argv[3];
  outfile+=".ppm";
  //ppm other(image);
  image.reduceNoise(framesize,threshold);
  image.write(outfile.c_str());
  // other.reduceNoiseAlt(framesize,threshold); //this version is unsurprisingly crap.
  //other.write(("Alt"+outfile).c_str());

  return 0;
}
