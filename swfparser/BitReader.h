#ifndef BITREADER_H_INCLUDED
#define BITREADER_H_INCLUDED

#include <iostream>
#include <sstream>
using namespace std;

class BitReader
{
public:
    BitReader(char *data);

    unsigned char readBits(uint32_t aLire);
    unsigned char readUChar(void);
    uint16_t readUInt16(void);
private:
    stringstream ss;

    unsigned char availableBits;
    unsigned char currentByte;

    unsigned char readBit(void);
};

#endif // BITREADER_H_INCLUDED
