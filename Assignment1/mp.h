/*
 * mp.h
 *
 *  Created on: 20/10/2011
 *      Author: Johan
 */

#ifndef MP_H_
#define MP_H_

/* set width of current image */
void mp_set_width(WORD width);

/* set height of current image */
void mp_set_height(WORD height);

/* set width of current image */
void mp_reset_pointer();

/* set a pixel */
void mp_set_pixel(BYTE pixel);

/* get a pixel */
BYTE mp_get_pixel();

/* get height */
WORD mp_get_height();

/* get width */
WORD mp_get_width();

/* filter the current image */
BOOL mp_filter_image(int filter_ID);


#endif
