#ifndef SWFFILE_H_INCLUDED
#define SWFFILE_H_INCLUDED

#include <sys/stat.h>

#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <sstream>
using namespace std;

#include <stdio.h>
#include <string.h>
#include <zlib.h>
/*#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif*/

#include "BitReader.h"

enum SwfCompression : unsigned char { NONE = 0x46, ZLIB = 0x43, LZMA = 0x5A };

class SwfFile
{
public:
    SwfFile();
    void open(string filepath);
    void close();
    void parseHeader();
private:
    ifstream m_swf;
    unsigned char m_compression;
    bool m_isSwf;
    unsigned char m_version;
    unsigned long int m_size;
};

#endif // SWFFILE_H_INCLUDED
