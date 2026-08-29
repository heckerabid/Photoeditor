#ifndef OPENBMP_H
#define OPENBMP_H

#include <stdio.h>

#pragma pack(push, 1)

typedef struct {
    unsigned short type;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataoffset;
} BMPFileHeader;


typedef struct {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsperpixel;
    unsigned int compression;
    unsigned int xpixelsPerM;
    unsigned int ypixelsPerM;
    unsigned int colorsUsed;
    unsigned int importantColors;
} BMPInfoHeader;

#pragma pack(pop)


typedef struct {

    BMPFileHeader fileheader;

    BMPInfoHeader infoheader;

    unsigned char *pixels;

    int width;
    int height;

} BMPImage;


BMPImage *openBMP(const char *filename);

int saveBMP(const char *filename, BMPImage *image);

void freeBMP(BMPImage *image);


#endif