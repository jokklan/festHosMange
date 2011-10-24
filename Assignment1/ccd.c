
/********************************************************************************
 * CCD
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

static IMAGE current_image;

WORD ccd_pixel_pointer;

WORD ccd_get_height() {
	return current_image.Height;
}

WORD ccd_get_width() {
	return current_image.Width;
}

void ccd_capture_image(char* image_path) {
	
	if(!bmp_open(image_path, &current_image)) {
		printf("ccd_capture_image(): failed to open file\n");
		exit(0);
	}
}

void ccd_reset_pointer() {

	/* reset */
	ccd_pixel_pointer = 0;
}

BYTE ccd_get_pixel() {
	return current_image.Pixels[ccd_pixel_pointer++];
}

WORD ccd_get_pixels() {

	/* todo: return (four) pixels */

	/* update pixel pointer */
	ccd_pixel_pointer += 4;

	return 0;
}
