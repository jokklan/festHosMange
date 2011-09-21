
/********************************************************************************
 * Test (main)
 ********************************************************************************/

#include "ccd.h"
#include "lcd.h"

int main(int argc, char *argv[]) {
	WORD i, w, h;

	/* capture image */
	ccd_capture_image();

	/* reset ccd and lcd pointers */
	ccd_reset_pointer();
	lcd_reset_pointer();

	/* set width/height of lcd to match captured image */
	w = ccd_get_width();
	h = ccd_get_height();

	lcd_set_width(w);
	lcd_set_height(h);

	/* transfer image from ccd to lcd a pixel at a time */
	for(i = 0; i < w * h; i++)
		lcd_set_pixel(ccd_get_pixel());

	/* show image on lcd */
	lcd_show_image();

	return 0;
}
