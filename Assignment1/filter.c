/*
 * filter.c
 *
 *  Created on: 17/10/2011
 *      Author: Johan
 */
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "filter.h"

FILTER create_filter(int height, int width, int* array, float factor, float bias) {
	FILTER filter;
	int i;
	filter.Bias = bias;
	filter.Factor = factor;
	filter.Height = height;
	filter.Width = width;
	for(i = 0; i < height*width; i++) {
		filter.Values[i] = array[i];
	}
	return filter;
}

BOOL add_extra_filter(FILTER* filter, int* array, int size) {
	int i;
	for(i = 0; i < size; i++) {
		filter->Values[i] += array[i];
	}
	return TRUE;
}

BOOL filter(IMAGE* image) {
	int filter_id = 2;
	FILTER filter = create_filter(filter_data[filter_id][0], filter_data[filter_id][1], filter_values[filter_id], filter_data[filter_id][2], filter_data[filter_id][3]);
	int extra_filter_id = 0;
	add_extra_filter(&filter, filter_values[extra_filter_id], filter_data[extra_filter_id][0]*filter_data[extra_filter_id][1]);
	filter_process(&filter, image);

	return TRUE;
}

BOOL filter_process(FILTER* filter, IMAGE* image) {
	IMAGE filtered_image;
	int filter_row = filter->Height;
	float bias = filter->Bias;
	int filter_column = filter->Width;
	int filter_height = (filter_row -1 )/2;
	int filter_width =  (filter_column -1 )/2;
	unsigned int row;
	unsigned int col;
	int i;
	int j;
	unsigned int imgSize = image->Height * image->Width;
	float factor;
	float filter_sum;
	int sum = 0;
	int pixel_index;
	int filter_index;
	float new_value;

	printf("Filter picture \n");

	if(filter->Factor == 0){
		filter_sum = 0;
		for (i = 0; i < filter_row; i++) {
			for (j = 0; j < filter_column; j++) {
				filter_sum += (int) filter->Values[to_one_dimension(i,j, filter_column)];
			}
		}

		if (filter_sum != 0) {
			factor = 1/filter_sum;
		} else {
			factor = 1;
		}
	}else{
		factor = filter->Factor;
	}

	//2D Convolution algorithm
	for(row = 0; row < image->Height; row++) {
		for(col = 0; col < image->Width; col++) {
			pixel_index = to_one_dimension(row, col, image->Width);
			for(i = -filter_height; i <= filter_height; i++){
				for(j = -filter_width; j <= filter_width; j++){
					filter_index = to_one_dimension(row + i, col + j, image->Width);
					if((row + i < 0) || (row + i >= image->Height) || (col + j < 0) || (col + j >= image->Width)){
						sum += filter->Values[to_one_dimension(abs_point(i, filter_row),abs_point(j, filter_column),filter_column)] * image->Pixels[pixel_index];
					}else{
						sum += filter->Values[to_one_dimension(abs_point(i, filter_row),abs_point(j, filter_column),filter_column)] * image->Pixels[filter_index];
					}
				}
			}

			new_value = factor*sum+bias;

			if (new_value > 255) {
				filtered_image.Pixels[pixel_index] = 255;
			} else if (new_value < 0) {
				filtered_image.Pixels[pixel_index] = 0;
			} else {
				filtered_image.Pixels[pixel_index] = new_value;
			}

			sum = 0;
		}
	}

	for(i = 0; i < imgSize; i++) {
		image->Pixels[i] = filtered_image.Pixels[i];
	}

	return TRUE;
}

int abs(int value){
	if (value < 0) {
		return value*(-1);
	}else{
		return value;
	}
}


int abs_point(int value, int index){
	return value = value + ((index) / 2);
}

int to_one_dimension(int row, int col, int width){
	return row * width + col;
}
