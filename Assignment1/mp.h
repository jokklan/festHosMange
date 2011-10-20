/*
 * mp.h
 *
 *  Created on: 20/10/2011
 *      Author: Johan
 */

#ifndef MP_H_
#define MP_H_

#include "types.h"
#include <string.h>



/* filter's the given image */
BOOL mp_filter_image(int filter_id, int extra_filter_id);

BOOL mp_compress_image(char *filename);

BOOL mp_load_image(char *filename, BOOL compressed);

BOOL mp_save_image(char *filename);

/* getting the image */
BOOL mp_get_image(char *filename);

#endif /* MP_H_ */
