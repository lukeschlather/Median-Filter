// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include<vector>
#include <fstream>
using namespace std;
string ppmtype;
int height;
int width;
int bitdepth;

vector<vector<vector <char> > > image;

void checkContinue(string message) {
  string input;
  cout << "Warning: " << message << endl;
  cout << "Continue: (y/n)" << endl;
  cin >> input;
  if ((input!="y")&&(input!="Y")) {
    exit(1);
  }
}

int main(int argc,char** argv)
{

  if(argc!=3) {
    cout << "Usage: ReduceImageNoise <input image> <output image> " << endl;
  }
  cout << "Infile: " << argv[1] << endl;
  ifstream infile(argv[1]);
  ofstream outfile(argv[2]);
  infile >> ppmtype;
  cout << ppmtype << endl;
  if(ppmtype!="P6") {
    checkContinue("The input file is not a P6 PPM");
  }
  infile >> height;
  infile >> width;
  infile >> bitdepth;
  if(bitdepth!=255) {
    checkContinue("The input depth is not 255."); 
  }
  cout << height << " x " << width << endl;
  cout << "Bit Depth: " << bitdepth << endl;
  infile.get(); //Discard the newline
  image.resize(height);
  for (int y=0;y<height;++y) {
    image.at(y).resize(width);
    for (int x=0;x<width;++x) {
      image.at(y).at(x).resize(3);
      infile.get(image.at(y).at(x).at(0));
      infile.get(image.at(y).at(x).at(1));
      infile.get(image.at(y).at(x).at(2));
    }
  }

  outfile << ppmtype << endl;
  outfile << height << " " << width << endl;
  outfile << bitdepth << endl;;
  for (int y=0;y<height;++y) {
    for (int x=0;x<width;++x) {
      outfile.put(image.at(y).at(x).at(0));
      outfile.put(image.at(y).at(x).at(1));
      outfile.put(image.at(y).at(x).at(2));
    }
  }
  
  

return 0;
}
