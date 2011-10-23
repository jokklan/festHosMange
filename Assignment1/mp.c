/*
 * mp.c
 *
 *  Created on: 20/10/2011
 *      Author: Johan
 */


#include <stdio.h>
#include "filter.h"
#include "mp.h"
#include "bmp.h"

static IMAGE stored_image;             // current image
static WORD mp_pixel_pointer;   // next pixel to write

/* set width of current image */
void mp_set_width(WORD width) {
	/* set desired image width */
	stored_image.Width = width;
}

/* set height of current image */
void mp_set_height(WORD height) {
	/* set desired image height */
	stored_image.Height = height;
}

/* set width of current image */
void mp_reset_pointer() {
	/* reset, ready for new image */
	mp_pixel_pointer = 0;
}

/* set a pixel */
void mp_set_pixel(BYTE pixel) {
	stored_image.Pixels[mp_pixel_pointer++] = pixel;
}

/* get a pixel */
BYTE mp_get_pixel() {
	return stored_image.Pixels[mp_pixel_pointer++];
}

/* get height */
WORD mp_get_height() {
	return stored_image.Height;
}

/* get width */
WORD mp_get_width() {
	return stored_image.Width;
}

/* filter the current image */
BOOL mp_filter_image(int filter_id) {
    if (!filter_image(&stored_image, filter_id)) {
        perror("ERROR occured when trying to filter");
        return FALSE;
    }
    return TRUE;
}


