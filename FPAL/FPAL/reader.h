#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include <fstream>
#include <string>
#include "exceptions.h"

int readEntireFile(const char *filename, char ** contents);

#endif // READER_H_INCLUDED
