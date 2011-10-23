/*
 * filter.h
 *
 *  Created on: 17/10/2011
 *      Author: Johan
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "types.h"
#include "bmp.h"

#define MAX_FILTER_WIDTH	9
#define MAX_FILTER_HEIGHT	9

/* data structure for filter */
typedef struct {
	WORD  Height;	//Height of the filter
	WORD  Width;	//Width of the filter
	float Bias;		//Bias of the filter. This value is added to the sum when calculating the new image.
	float  Factor;	//Factor of the filter. This value is multiplied with the sum when calculating the new image. If 0, it will calculate its own factor based on the filter.
	WORD  Values[MAX_FILTER_WIDTH*MAX_FILTER_HEIGHT];	//The actual filter values
} FILTER;

static int filter_data[19][4] = {
	{1, 1, 0, 0},	//1
	{1, 1, 0, 0},	//1
	{1, 1, -1, 256},//2
	{3, 3, 0, 128},	//3
	{3, 3, 0, 0},	//4
	{3, 3, 0, 0},	//5
	{3, 3, 0, 0},	//6
	{3, 3, 0, 0},	//7
	{3, 3, 0, 0},	//8
	{5, 5, 0, 0},	//9
	{5, 5, 0, 128},	//10
	{5, 5, 0, 0},	//11
	{5, 5, 0, 0},	//12
	{5, 5, 0, 0},	//13
	{5, 5, 0, 0},	//14
	{5, 5, 0, 0},	//15
	{5, 5, 0, 0},	//16
	{5, 5, 0, 0},	//17
	{9, 9, 0, 0},	//18

};

static int filter_values[19][MAX_FILTER_WIDTH*MAX_FILTER_HEIGHT] = {
	{// 0: Erase
		0
	},
	{// 1: Normal filter, do not change anything
		1
	},
	{// 2: Invert filter
		1
	},
	{// 3: Emboss (light)
		-1 , -1 , 0 ,
		-1 , 0 , 1  ,
		0 , 1 , 1
	},
	{// 4: Blur (light)
		0 , 1 , 0 ,
		1 , 1 , 1  ,
		0 , 1 , 0
	},
	{// 5: Find edges (all directions, light)
		-1 , -1 , -1 ,
		-1 , 8 , -1 ,
		-1 , -1 , -1
	},
	{// 6: Sharpen (light)
		-1 , -1 , -1 ,
		-1 , 9 , -1 ,
		-1 , -1 , -1
	},
	{// 7: Show edges (light)
		1 , 1 , 1 ,
		1 , -7 , 1 ,
		1 , 1 , 1
	},
	{// 8: Mean (light)
		1 , 1 , 1 ,
		1 , 1 , 1 ,
		1 , 1 , 1
	},

	{// 9: Find edges (horizontal, medium)
	     0,  0,  1,  0,  0,
	     0,  0,  1,  0,  0,
	     0,  1,  1,  1,  0,
	     0,  0,  0,  0,  0,
	     0,  0,  0,  0,  0,
	},
	{// 10: Emboss (medium)
	    -1, -1, -1, -1,  0,
	    -1, -1, -1,  0,  1,
	    -1, -1,  0,  1,  1,
	    -1,  0,  1,  1,  1,
	     0,  1,  1,  1,  1
	},
	{// 11: Find edges (vertical, medium)
		0 , 0 , -1 , 0 , 0 ,
		0 , 0 , -1 , 0 , 0 ,
		0 , 0 , 4 , 0 , 0 ,
		0 , 0 , -1 , 0 , 0 ,
		0 , 0 , -1 , 0 , 0
	},
	{// 12: Motion blur (medium)
		1 , 0 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 , 0 ,
		0 , 0 , 1 , 0 , 0 ,
		0 , 0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 0 , 1
	},
	{// 13: Find edges (45° direction, medium)
		-1 , 0 , 0 , 0 , 0 ,
		0 , -2 , 0 , 0 , 0 ,
		0 , 0 , 6 , 0 , 0 ,
		0 , 0 , 0 , -2 , 0 ,
		0 , 0 , 0 , 0 , -1
	},
	{// 14: Blur (medium)
		0 , 0 , 1 , 0 , 0 ,
		0 , 1 , 1 , 1 , 0 ,
		1 , 1 , 1 , 1 , 1 ,
		0 , 1 , 1 , 1 , 0 ,
		0 , 0 , 1 , 0 , 0
	},
	{// 15: Mean (medium)
		1 , 1 , 1 , 1 , 1 ,
		1 , 1 , 1 , 1 , 1 ,
		1 , 1 , 1 , 1 , 1 ,
		1 , 1 , 1 , 1 , 1 ,
		1 , 1 , 1 , 1 , 1
	},
	{ // 16: Shatter (diagonal, medium)
		1 , 0 , 0 , 0 , 1 ,
		0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 ,
		1 , 0 , 0 , 0 , 1 ,
	},
	{ // 17: 'Fire' effect (medium)
		0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 0 , 0 , 0 ,
		0 , 0 , 1 , 0 , 0 ,
		0 , 1 , 1 , 1 , 0 ,
		1 , 1 , 1 , 1 , 1 ,
	},
	{// 18: Motion blur (Very heavy)
	    1, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 1, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 1, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 1, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 1, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 1, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 1, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 1, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 1,
	}
};

/* add filter to an existing filter */
BOOL add_extra_filter(FILTER* filter, int* array, int size);

/* create filter */
FILTER create_filter(int height, int width, int* array, float factor, float bias);

/* filter image */
BOOL filter_image(IMAGE* image, int filter_id);

/* filter image */
BOOL filter_process(FILTER* filter, IMAGE* image);

/* change to one dimension from two dimensions */
int to_one_dimension(int row, int col, int width);

/* get absolut value of a point value */
int abs_point(int value, int index);

/* get absolut value */
int abs(int value);

#endif /* FILTER_H_ */
