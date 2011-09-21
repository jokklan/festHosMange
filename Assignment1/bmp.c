#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

/* see lecture notes for more information on pragma pack directive */
#pragma pack(push, 1)

/*
 *   BITMAP FILE: See http://en.wikipedia.org/wiki/BMP_file_format
 *   ___________________________________________________________
 *  |          |          |            |                        |
 *  |   file   |   info   |  Palette   |       Pixel data       |     
 *  |  header  |  header  | (optional) |                        |
 *  |__________|__________|____________|________________________|
 *  start of file 									  end of file
 * 
 *  - Lines must be word-aligned!
 *  
 * 
 */

/**********************************************************************
 * Bitmap File Header
 **********************************************************************/
typedef struct {
	HALFWORD 	BfType;  		    /* Must be 0x4D42 				  */
	WORD	 	BfSize;				/* Size of the file in bytes 	  */
	HALFWORD	BfReserved1;		/* Should be 0 					  */
	HALFWORD	BfReserved2;		/* Should be 0 					  */
	WORD		BfOffBits;			/* Offset of image data in file   */
} BITMAPFILEHEADER;

/**********************************************************************
 * Bitmap Information Header
 **********************************************************************/
typedef struct {	
	WORD 		BiSize;				/* Size of this structure 		  */
	WORD		BiWidth;			/* Width of the image in bytes    */
	WORD		BiHeight;			/* Height of the image in bytes   */
	HALFWORD	BiPlanes;			/* Should be 1 					  */
	HALFWORD	BiBitCount;			/* Bit count (..) 				  */
	WORD		BiCompression;		/* Compression used 			  */
	WORD		BiSizeImage;		/* Size of the image in bytes 	  */
	WORD		BiXPelsPerMeter; 	/* Pixels per meter, X			  */
	WORD		BiYPelsPerMeter;	/* Pixels per meter, Y 			  */
	WORD		BiClrUsed;			/* number of colors used 		  */
	WORD		BiClrImportant;		/* number of important colors 	  */
} BITMAPINFOHEADER;

/* add here other structs you want to pack */

#pragma pack(pop)


BOOL bmp_open(char* file, IMAGE* image) {

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	/* note: "rb" means open for binary read */
	FILE* fp = fopen(file, "rb");

	if(fp == NULL) {							   
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}

	/* todo: process file */
	fread(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	fread(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fread(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fread(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fread(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	fread(&bmih.BiSize, sizeof(bmih.BiSize), 1, fp);
	fread(&bmih.BiWidth, sizeof(bmih.BiWidth), 1, fp);
	fread(&bmih.BiHeight, sizeof(bmih.BiHeight), 1, fp);
	fread(&bmih.BiPlanes, sizeof(bmih.BiPlanes), 1, fp);
	fread(&bmih.BiBitCount, sizeof(bmih.BiBitCount), 1, fp);
	fread(&bmih.BiCompression, sizeof(bmih.BiCompression), 1, fp);
	fread(&bmih.BiSizeImage, sizeof(bmih.BiSizeImage), 1, fp);
	fread(&bmih.BiXPelsPerMeter, sizeof(bmih.BiXPelsPerMeter), 1, fp);
	fread(&bmih.BiYPelsPerMeter, sizeof(bmih.BiYPelsPerMeter), 1, fp);
	fread(&bmih.BiClrUsed, sizeof(bmih.BiClrUsed), 1, fp);
	fread(&bmih.BiClrImportant, sizeof(bmih.BiClrImportant), 1, fp);

	image->Height = bmih.BiHeight;
	image->Width = bmih.BiWidth;

	unsigned int i = 0;
	BYTE red;
	BYTE green;
	BYTE blue;

	unsigned int imgSize = image->Height * image->Width;

	while(i < imgSize) {
		fread(&blue, sizeof(BYTE), 1, fp);
		fread(&green, sizeof(BYTE), 1, fp);
		fread(&red, sizeof(BYTE), 1, fp);

		image->Pixels[i] = (0.3 * red + 0.59 * green + 0.11 * blue);

		i++;
	}

	/* success */
	fclose(fp);
	return TRUE;
}

BOOL bmp_save(char* file, IMAGE* image) {

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	unsigned int i = 0;

	/* note: "wb" means open for binary write */
	FILE* fp = fopen(file, "wb");

	if(fp == NULL) {
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}

	/* todo: store image to fp */
	fwrite(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	fwrite(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fwrite(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fwrite(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fwrite(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	fwrite(&bmih.BiSize, sizeof(bmih.BiSize), 1, fp);
	fwrite(&bmih.BiWidth, sizeof(bmih.BiWidth), 1, fp);
	fwrite(&bmih.BiHeight, sizeof(bmih.BiHeight), 1, fp);
	fwrite(&bmih.BiPlanes, sizeof(bmih.BiPlanes), 1, fp);
	fwrite(&bmih.BiBitCount, sizeof(bmih.BiBitCount), 1, fp);
	fwrite(&bmih.BiCompression, sizeof(bmih.BiCompression), 1, fp);
	fwrite(&bmih.BiSizeImage, sizeof(bmih.BiSizeImage), 1, fp);
	fwrite(&bmih.BiXPelsPerMeter, sizeof(bmih.BiXPelsPerMeter), 1, fp);
	fwrite(&bmih.BiYPelsPerMeter, sizeof(bmih.BiYPelsPerMeter), 1, fp);
	fwrite(&bmih.BiClrUsed, sizeof(bmih.BiClrUsed), 1, fp);
	fwrite(&bmih.BiClrImportant, sizeof(bmih.BiClrImportant), 1, fp);
	
	unsigned int imgSize = image->Height * image->Width;

    for(i = 0; i < imgSize; i++) {
    	fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
    	fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
    	fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
    }

	fclose(fp);
	return TRUE;
}
