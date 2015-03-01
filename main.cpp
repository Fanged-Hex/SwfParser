#include <iostream>
using namespace std;

#include "swfparser/SwfFile.h"

int main()
{
    SwfFile fichier;
    fichier.open("test.swf");
    fichier.parseHeader();
    return 0;
}
