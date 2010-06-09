
This is a basic program which reads and writes PPM images. The utility functions store pixel data in basically the same format it is stored in a ppm; scanlines of 24-bit pixels (the colorRGB struct.)

The primary program that the class defined in Image.h was written for is CleanImage, which applys a median filter to a ppm image, and spits it out to a new ppm with the parameters used in its generation appended to the end of the filename. 

Usage: CleanImage <input image> <framesize> <threshold>

A general sort of usage is:

    CleanImage /home/myfile.ppm 5 100

This will create a new file, /home/myfile-f_5_t_100.ppm  

Framesize of 5 and threshold of 100 generally seem like sane defaults, though you can certainly play around a bit and have some fun. A framesize of 2 can be effective, and is much faster - this code is definitely not optimized. It takes a few minutes to deal with an 800x600 image on framesize 5.

For some examples, see [my blog.](http://flinchbaughschlather.com/luke/2010/06/07/median-filter/)

I started to work in some jpeg reading functionality which puts a libjpeg dependency which should be pretty trivial to remove.