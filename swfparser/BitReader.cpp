#include "BitReader.h"

BitReader::BitReader(char *data, int taille)
{
    ss.write(data, taille);

    currentByte = data[0];
    availableBits = 8;
}

unsigned char BitReader::readBits(int aLire)
{
    unsigned char retour = 0;

    for(int i = aLire; i > 0; i--)
    {
        retour = retour | ( readBit() << (i-1) );
    }

    return retour;
}

unsigned char BitReader::readUChar()
{
    unsigned char retour = 0;

    for(int i = 0; i < sizeof(retour)*8; i++)
    {
        retour = (retour | readBit()) << 1;
    }

    return retour;
}

uint16_t BitReader::readUInt16()
{
    uint16_t retour = 0;
    int aLire = 16;

    for(int i = aLire; i > 0; i--)
    {
        retour = retour | ( readBit() << (i-1) );
    }

    return retour;
}

unsigned char BitReader::readBit()
{
    if(availableBits <= 0) {
        ss.read((char*)&currentByte, 1);
        availableBits = 8;
    }

    unsigned char masque = 0;
    switch(availableBits)
    {
    case 8:
        masque = 0x80;
        break;
    case 7:
        masque = 0x40;
        break;
    case 6:
        masque = 0x20;
        break;
    case 5:
        masque = 0x10;
        break;
    case 4:
        masque = 0x08;
        break;
    case 3:
        masque = 0x04;
        break;
    case 2:
        masque = 0x02;
        break;
    case 1:
        masque = 0x01;
        break;
    }

    return ( (currentByte & masque) >> (--availableBits) );
}
