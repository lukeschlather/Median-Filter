// Luke Schlather
// Thursday, September 10 2009
// Licensed under the LGPL
#include<iostream>
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


string filenameSansExtension(string filename) {
  int i=0;
  for (i=filename.size()-1;i>=0;--i) {
    if(filename[i]=='.') {
      break;
    }
  }
  return filename.substr(0,i);
}

string filenameExtension(string filename) {
  int i=0;
  for (i=filename.size()-1;i>=0;--i) {
    if(filename[i]=='.') {
      break;
    }
  }
  return filename.substr(i+1,filename.size());
}
