#include "SwfFile.h"

// Permet de transformer la taille en texte humain
char* readable_fs(unsigned long int s)
{
    int i = 0;

    float d = static_cast<float>(s);
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    char *buf = new char[10];

    while (d >= 1024)
    {
        d /= 1024;
        i++;
    }
    sprintf(buf, "%.2f %s", d, units[i]);
    return buf;
}

SwfFile::SwfFile()
    : m_isSwf(false)
{
}

void SwfFile::open(string filepath)
{
    m_swf.open(filepath.c_str(), ios::in | ios::binary);
}

void SwfFile::close()
{
    m_swf.close();
    m_isSwf = false;
}

void SwfFile::parseHeader()
{
    cout << "Entete fichier" << endl;
    // On récupère la taille du fichier
    m_swf.seekg (0, m_swf.end);
    unsigned long int tailleFichier = m_swf.tellg();
    m_swf.seekg (0, m_swf.beg);

    cout << "\tTaille " << tailleFichier << " ( " << readable_fs(tailleFichier) << " )" << endl;

    // On vérifie la compression du fichier
    m_swf >> m_compression;

    // On vérifie le reste de la signature (comme on lit 2 octets, ils sont inversés)
    unsigned short sig;
    m_swf.read((char*)&sig, 2);
    m_isSwf = (sig == 0x5357);
    if(!m_isSwf)
        return;

    m_swf.read((char*)&m_version, 1);
    m_swf.read((char*)&m_size, 4);

    cout << "Entete SWF" << endl;
    cout << "\tVersion " << (int)m_version << endl;
    cout << "\tTaille " << m_size << " ( " << readable_fs(m_size) << " )" << endl;

    // Déclaration de variables pour la décompression
    int ret;
    unsigned char *in = new unsigned char[tailleFichier-8];
    unsigned char *out = new unsigned char[m_size];

    // On crée la structure pour décompresser et on initialise des paramètres
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return;

    m_swf.read((char*)in, tailleFichier-8);
    strm.avail_in = tailleFichier-8;
    if (strm.avail_in == 0)
        return;
    strm.next_in = in;

    strm.avail_out = m_size;
    strm.next_out = out;

    ret = inflate(&strm, Z_NO_FLUSH);

    switch (ret)
    {
    case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
        (void)inflateEnd(&strm);
        return;
    }

    // On libère la mémoire utilisée pour la décompression
    inflateEnd(&strm);

    BitReader reader( (char*)out );

    unsigned char rectSize = reader.readBits(5);
    cout << "Frame RECT Size " << (int)rectSize << endl;

    /*uint16_t posX;
    fileData.read( (char*)&posX, 2 );
    cout << "\tPos X " << posX << endl;

    uint16_t posY;
    fileData.read( (char*)&posY, 2 );
    cout << "\tPos Y " << posY << endl;

    uint16_t width;
    fileData.read( (char*)&width, 2 );
    cout << "\tWidth " << width << endl;

    uint16_t height;
    fileData.read( (char*)&height, 2 );
    cout << "\tHeight " << height << endl;

    uint16_t frameRate;
    fileData.read((char*)&frameRate, sizeof(frameRate));
    cout << "Frame rate " << frameRate << endl;

    uint16_t frameCount;
    fileData.read((char*)&frameCount, sizeof(frameCount));
    cout << "Frame count " << frameCount << endl;*/
}
