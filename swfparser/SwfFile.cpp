#include "SwfFile.h"

// Permet de transformer la taille en texte humain
char* readable_fs(unsigned long int s) {
    int i = 0;

    float d = static_cast<float>(s);
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    char *buf = new char[10];

    while (d >= 1024) {
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
    cout << "Preparation a la decompression" << endl;
    int ret;
    unsigned char in[tailleFichier-8];
    unsigned char out[m_size];

    // On crée la structure pour décompresser et on initialise des paramètres
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    cout << "Initialisation de la decompression" << endl;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return;

    m_swf.read((char*)in, sizeof(in));
    strm.avail_in = sizeof(in);
    if (strm.avail_in == 0)
        return;
    strm.next_in = in;

    strm.avail_out = sizeof(out);
    strm.next_out = out;

    cout << "Décompression en cours ..." << endl;
    ret = inflate(&strm, Z_NO_FLUSH);

    switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return;
    }

    // On libère la mémoire utilisée pour la décompression
    inflateEnd(&strm);
}
