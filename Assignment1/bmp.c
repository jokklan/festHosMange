#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "filter.h"

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

typedef struct {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[256];
} BITMAPINFO;
/* add here other structs you want to pack */

#pragma pack(pop)

BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;

BOOL bmp_open(char* file, IMAGE* image) {
	unsigned int imgSize;
	unsigned int i = 0;
	BYTE red;
	BYTE green;
	BYTE blue;

	/* note: "rb" means open for binary read */
	FILE* fp = fopen(file, "rb");

	if(fp == NULL) {							   
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}

	/* Read the bitmap file header (BITMAPFILEHEADER) */
	fread(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	fread(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fread(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fread(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fread(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	/* Read the bitmap info header (BITMAPINFOHEADER) */
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
	imgSize = image->Height * image->Width;

	/* throwing away the palette if 8 bit */
	if (bmih.BiBitCount == 8) {
		BYTE trash;
		for (i = 0; i < 256*4; i++) {
			fread(&trash, sizeof(BYTE), 1, fp);
		}
	}

	/* checks if the picture is compressed and treats is as well */
	if (bmih.BiCompression != 0) {

		/* picture is compressed */
		bmp_uncompress(fp, image);

	} else {
		/* picture is not compressed */
		printf("Opening uncompressed picture\n");

		/* checks wether picture is 8 bit or 24 bit */
		if (bmih.BiBitCount == 8) {

			/* picture is 8 bit */
			/* NOT WORKING */

			BYTE grayscale;
			for (i = 0; i < imgSize; i++) {
				fread(&grayscale, sizeof(BYTE), 1, fp);
				image->Pixels[i] = grayscale;
			}
		} else if (bmih.BiBitCount == 24) {

			/* picture is 24 bit */
			for (i = 0; i < imgSize; i++) {
				fread(&red, sizeof(red), 1,fp);
				fread(&green, sizeof(green), 1,fp);
				fread(&blue, sizeof(blue), 1,fp);
				image->Pixels[i] = 0.3*red + 0.59*green + 0.11*blue; /* calculating pixel from colorscale to greyscale */
			}
		} else {
			perror("Not 8 nor 24 bit picture\n");
			return FALSE;
		}
	}

	/* success */
	fclose(fp);
	return TRUE;
}

BOOL bmp_uncompress(FILE* fp, IMAGE* image) {
	unsigned int j;
	BYTE ind; /* indicator */
	BYTE clr; /* color */
	BOOL endReached = FALSE; /* decides if end of picture is reached */
	int pixIndex = 0; /* pixel-index */

	printf("Uncompressing picture\n");
	/* uncompressing */
	while (!endReached) {
		fread(&ind, sizeof(BYTE), 1, fp);
		fread(&clr, sizeof(BYTE), 1, fp);

		/* if count != 0, get color */
		if (ind != 0) {
			for (j = 0; j < ind; j++) {
				image->Pixels[pixIndex++] = clr;
			}

		/* if count == 0, end-indicator */
		} else if (clr == 1){
			endReached = TRUE;
		}
	}

	/* setting picture as uncompressed */
	bmih.BiCompression = 0;

	return TRUE;
}

BOOL bmp_save(char* file, IMAGE* image, BOOL compress) {
	BITMAPINFO bmi;
	unsigned int i;
	int no_of_pixels = bmih.BiWidth * bmih.BiHeight;
	bmi.bmiHeader = bmih;

	BOOL use_filter = TRUE;

	/* note: "wb" means open for binary write */
	FILE* fp = fopen(file, "wb");

	if(fp == NULL) {
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}

	/* setting the new values to make it 8-bitmap */
	bmih.BiSizeImage     = 0;
	bmfh.BfSize          = sizeof(bmfh) + sizeof(bmih) + 256*4 + no_of_pixels;
	bmfh.BfOffBits       = sizeof(bmfh) + sizeof(bmih) + 256*4;
	bmih.BiClrUsed       = 256;
	bmih.BiBitCount      = 8;
	if(compress) bmih.BiCompression = 1;

	//Write the bitmap file header (BITMAPFILEHEADER)
	fwrite(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	fwrite(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fwrite(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fwrite(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fwrite(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	//Write the bitmap info header (BITMAPINFOHEADER)
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

	//Create the color palette (RGBQUAD)
	for(i = 0; i < 256; i++){
		bmi.bmiColors[i].rgbRed = i;
		bmi.bmiColors[i].rgbGreen = i;
		bmi.bmiColors[i].rgbBlue = i;
		bmi.bmiColors[i].rgbReserved = 0;
	}

	//Write the color palette (RGBQUAD)
	for(i = 0; i < 256; i++){
		fwrite(&bmi.bmiColors[i], sizeof(bmi.bmiColors[i]), 1, fp);
	}


	if(use_filter){
		filter(image);
	}

	if(compress){
		/* saving as compressed picture */
		bmp_compress(fp, image);
	}else{
		/* saving as uncompressed picture */
		printf("Saving uncompressed picture \n");
		for(i = 0; i < imgSize; i++) {
			fwrite(&image->Pixels[i], sizeof(BYTE), 1, fp);
		}
	}

    printf("Save Complete \n");
	fclose(fp);
	return TRUE;
}

BOOL bmp_compress(FILE* fp, IMAGE* image) {
	unsigned int i;
	unsigned int imgSize = image->Height * image->Width;
	BYTE one = 1;
	BYTE ind = 1;
	BYTE zero = 0;
	bmih.BiSizeImage = 0;

	/* saving as compressed picture */
	printf("Compressing and saving picture \n");

	/* compressing */
	bmih.BiSizeImage = 0;

	/* for every pixel */
	for (i = 0; i < imgSize; i++) {
		if ((i+1) % bmih.BiWidth == 0) {
			if(i < 500){
			}
			/* newline coming - write latest colour */
			fwrite(&ind, sizeof(BYTE), 1, fp);
			fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
			bmih.BiSizeImage = bmih.BiSizeImage + sizeof(image->Pixels[i]);
			/* newline */
			fwrite(&zero, sizeof(BYTE), 1, fp);
			fwrite(&zero, sizeof(BYTE), 1, fp);
			bmih.BiSizeImage = bmih.BiSizeImage + sizeof(BYTE) + sizeof(BYTE);

			ind = 1;
		} else if (image->Pixels[i] == image->Pixels[i + 1] && ind < 255) {
			if(i < 500){
			}
			/* next color is the same or the count almost reached overflow */
			ind++;
		} else {
			if(i < 500){
			}
			/* next color is NOT the same - write */
			fwrite(&ind, sizeof(BYTE), 1, fp);
			fwrite(&image->Pixels[i], sizeof(BYTE), 1, fp);
			bmih.BiSizeImage = bmih.BiSizeImage + sizeof(image->Pixels[i]);
			ind = 1;
		}
	}

	/* end of file write */
	fwrite(&zero, sizeof(BYTE), 1, fp);
	fwrite(&one, sizeof(BYTE), 1, fp);

	bmih.BiSizeImage = bmih.BiSizeImage + sizeof(BYTE) + sizeof(BYTE);
	bmfh.BfSize = sizeof(bmfh) + sizeof(bmih) + 256*4 + bmih.BiSizeImage;

	return TRUE;
}

