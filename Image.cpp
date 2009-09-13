// Luke Schlather
// Sunday, September  7 2008
// Licensed under the LGPL
#include<iostream>
#include<cmath>
#include<set>
#include<cstdlib>
#include<stdio.h>
#include<jpeglib.h>
#include<setjmp.h>
#include<jerror.h>

#include "Image.h"
#include "LJFS_Utils.h"

using namespace std;


void Image::destruct() {
  if(width) {
  for (int y=0;y<height;++y) {
    delete [] data[y];
  }
  }
  if(height) delete [] data;
  height=width=0;
}

Image::Image(const char* filename) {
  read(filename);    
}

void Image::read(string filename) {
  string format = filenameExtension(filename);
  //todo: downcase string; sanity checks
    if (format=="jpeg"||format=="jpg") {
      readJPEG(filename.c_str());
    }
    else if (format=="ppm") {
      ifstream filehandle(filename.c_str());
      readPPM(filehandle);
    }
    else { cout << "bad format: <" << format << ">" << endl; throw 1; }
}

void Image::write(const string filename) {
  ofstream outfile(filename.c_str());
  writePPM(outfile);
}

void Image::readPPM(istream& infile) {
  string Imagetype;
  infile >> Imagetype;
  cout << Imagetype << endl;
  //Todo:PPM Comments
  if(Imagetype!="P6") {
    checkContinue("The input file is not a P6 IMAGE");
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

void Image::readJPEG(const char* filename) {
  //This is basically copied from example.c in the libjpeg-6b source tar.
  // see that for better documentation.
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);

  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  //int row_stride;   Always 3	/* physical row width in output buffer */

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, true);

  (void) jpeg_start_decompress(&cinfo);
  if(cinfo.output_components != 3 ) {
    cout << "This program is only designed to handle images in RGB format." 
	 << endl;
  }
  //  buffer = (*cinfo.mem->alloc_sarray)
  //((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  height = cinfo.output_height;
  width  = cinfo.output_width;

  cout << height << " x " << width << endl;


  data = new colorRGB*[height];
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, width*3, 1);
  for (int y=0;y < height;++y) {
    cout << "reading scanline " << cinfo.output_scanline << endl;
    data[y]=new colorRGB[width];
    jpeg_read_scanlines(&cinfo,buffer,1);

    for (int x=0;x<width*3;x+=3) {
      cout << (int)buffer[0][x+R] << " " << x << " " << y << endl;
      data[y][x][R] = buffer[0][x+R];
      data[y][x][G] = buffer[0][x+G];
      data[y][x][B] = buffer[0][x+B];
    }

  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);


}


void Image::writePPM(ostream& outfile) {
  outfile << "P6" << endl;
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

void Image::reduceNoise(int fs,int threshold) {

  framesize=fs;
  modifiedPixels=0;
  Image output(*this);
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
  output.write("test");
  *this=output;

}

void Image::reduceNoiseAlt(int fs,int threshold) {
  framesize=fs;
  modifiedPixels=0;
  Image output(*this);
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
  output.write("test");
  *this=output;
}

void ReplaceIfOOBs(const int& lower,const int& upper, int& val) {
  if(val<lower) {
    val=lower;
  }
  if(val>upper) {
    val=upper;
  }
}

bool Image::needsCleaning(int threshold,int x,int y,colorchannel c) {
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
void Image::clean(int threshold,int x, int y,colorchannel c) {
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
  for(multiset<int>::iterator it=values.begin();i<values.size()/2;++i,++it) 
    {
      newval=*it;
    }
  data[y][x][c]=newval;
}

void Image::copy (const Image& src) {
  height = src.height;
  width = src.width;
  bitdepth = src.bitdepth;
  framesize = src.framesize;
  data = new colorRGB*[height];
  for (int y=0;y<height;++y) {
    data[y]=new colorRGB[width];
    for (int x=0;x<width;++x) {
      data[y][x][R]=src.data[y][x][R]; 
      data[y][x][G]=src.data[y][x][G];
      data[y][x][B]=src.data[y][x][B];
    }
  }
}

void Image::poke(colorchannel color, char value, int x,int y, int radius) {
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
	data[i][j][B]=value;
      }
      else {
	++oobs;
      }
      //cout << "(" << i << "," << j << ")" << endl;
    }
  }
  cout << "Out of bounds: " << oobs << endl;
  cout << height << endl;
  cout << width << endl;
  write("text");
}
