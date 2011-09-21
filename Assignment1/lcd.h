
/********************************************************************************
 * LCD Header
 ********************************************************************************/

#ifndef __LCD_H
#define __LCD_H

#include "types.h"

/* set the width of the next image to show */
void lcd_set_width(WORD width);

/* set the height of the next image to show */
void lcd_set_height(WORD height);

/* show the image */
void lcd_show_image();

/* reset the internal pixel pointer */
void lcd_reset_pointer();

/* set the next four pixels of the image */
void lcd_set_pixel(BYTE pixel);

/* set the next four pixels of the image */
void lcd_set_pixels(WORD pixels);

#endif /* __LCD_H */