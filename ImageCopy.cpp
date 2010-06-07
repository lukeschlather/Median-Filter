// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include "Image.h"

using namespace std;
int main(int argc,char** argv)
{
  if(argc!=3) {
    cout << "Usage: ReduceImageNoise <input image> <output image> " << endl;
  }
  cout << "Infile: " << argv[1] << endl;
  Image image(argv[1]);
  image.write(argv[2]);



  return 0;
}
