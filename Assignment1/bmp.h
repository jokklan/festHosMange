
/********************************************************************************
 * BMP Header
 ********************************************************************************/

#ifndef __BMP_H
#define __BMP_H

#include "types.h"

#define MAX_WIDTH	512
#define MAX_HEIGHT	512

/* data structure for the grayscale image, 1 BYTE / pixel */
typedef struct {
	WORD  Height;
	WORD  Width;
	BYTE  Pixels[MAX_WIDTH * MAX_HEIGHT];	
} IMAGE;

/* open and read a BMP file into image */
BOOL bmp_open(char* file, IMAGE* image);

/* store image to BMP file */
BOOL bmp_save(char* file, IMAGE* image, BOOL compress);

/* uncompress BMP image */
BOOL bmp_uncompress(FILE* fp, IMAGE* image);

/* compress BMP image */
BOOL bmp_compress(FILE* fp, IMAGE* image);

#endif /* __BMP_H */
