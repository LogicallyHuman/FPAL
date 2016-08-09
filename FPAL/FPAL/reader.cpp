#include "reader.h"

int readEntireFile(const char *filename, char ** contents){
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in){
    in.seekg(0, std::ios::end);
    int fileSize = in.tellg();
    *contents = new char[fileSize];
    in.seekg(0, std::ios::beg);
    in.read(*contents, fileSize);
    in.close();
    return fileSize;
  }
  throw(EXCEPTION_FILE_NOT_FOUND);
}
