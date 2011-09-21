
/********************************************************************************
 * LCD
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"
#include "bmp.h"

/* todo: add image data structure */

/* image representation compabible with bmp library */
static IMAGE image;

/* next pixel to write */
static WORD lcd_pixel_pointer;

void lcd_set_width(WORD width) {
	/* set desired image width */
	image.Width = width;	
}

void lcd_set_height(WORD height) {
	/* set desired image height */
	image.Height = height;
}

void lcd_show_image() {
	if(!bmp_save("output.bmp", &image)) {
		printf("lcd_show_image(): failed to save file\n");
	}
}

void lcd_reset_pointer() {

	/* reset, ready for new image */
	lcd_pixel_pointer = 0;
}

void lcd_set_pixel(BYTE pixel) {
	image.Pixels[lcd_pixel_pointer++] = pixel;
}

void lcd_set_pixels(WORD pixels) {

	/* todo: store (four) pixels */

	/* update pixel pointer */
	lcd_pixel_pointer += 4;
}
