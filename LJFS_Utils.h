#include<string>
#ifndef _LJFS_UTILS_
#define _LJFS_UTILS_

// Luke Schlather
// Thursday, September 10 2009
// Licensed under the LGPL
void checkContinue(std::string message);
std::string filenameSansExtension(std::string filename);
std::string filenameExtension(std::string filename);
std::string printCurrentTime();

#endif // _LJFS_UTILS_
