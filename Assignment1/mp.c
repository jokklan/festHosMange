/*
 * mp.c
 *
 *  Created on: 20/10/2011
 *      Author: Johan
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "mp.h"
#include "filter.h"

static IMAGE image;
char root[150] = "C:\\Users\\Johan\\Dropbox\\festHosMange\\Assignment1\\\0";

/* compresses an image */
BOOL mp_compress_image(char *filename) {
    char path[150];
    strcpy(path, root);
	strcat(filename, ".bmp");
	strcat(path, filename);
	printf("Path: %s \n", path);
    if(!bmp_save(path, &image, TRUE)) {
		printf("lcd_show_image(): failed to save file\n");
        return FALSE;
	}
    return TRUE;
}

/* captures an image */
	BOOL mp_get_image(char *filename) {
    char path[150];
    strcpy(path, root);
    strcat(filename, ".bmp");
    strcat(path, filename);
    printf("Path: %s \n", path);
    if(!bmp_open(path, &image)) {
		printf("mp_get_image(): failed to open file\n");
        return FALSE;
	}
    return TRUE;
}

/* loads an image from the uncompressed files */
BOOL mp_load_image(char *filename, BOOL compressed) {
    char path[150];
    if (compressed) strcpy(path, root);
    else strcpy(path, root);
	strcat(filename, ".bmp");
	strcat(path, filename);
	printf("Path: %s \n", path);
    if(!bmp_open(path, &image)) {
		printf("mp_get_image(): failed to open file\n");
        return FALSE;
	}
    return TRUE;
}

/* filters an image */
BOOL mp_filter_image(int filter_id, int extra_filter_id) {
	FILTER filter = create_filter(filter_data[filter_id][0], filter_data[filter_id][1], filter_values[filter_id], filter_data[filter_id][2], filter_data[filter_id][3]);

	add_extra_filter(&filter, filter_values[extra_filter_id], filter_data[extra_filter_id][0]*filter_data[extra_filter_id][1]);
	if(!filter_process(&filter, &image)){
		printf("ERROR: mp_filter_image(): failed filtering image");
		return FALSE;
	}

	return TRUE;
}

/* saves an image */
BOOL mp_save_image(char *filename) {
    char path[150];
    strcpy(path, root);
    strcat(filename, ".bmp");
    strcat(path, filename);
    printf("Path: %s \n", path);
    if(!bmp_save(path, &image, FALSE)) {
		printf("mp_save_image(): failed to save file\n");
        return FALSE;
	}
    return TRUE;
}
