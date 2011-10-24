/********************************************************************************
 * Test (main)
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccd.h"
#include "lcd.h"
#include "mp.h"
#include "bmp.h"
#include "filter.h"
#include <time.h>

/* outcomment define statement to disable performance check */
#define PERFORMANCE

#ifdef PERFORMANCE
    #include <time.h>
    clock_t start, end;
    double cpu_time_used;
#endif

/* prototypes */
void test(char* filename, BOOL compressed, BOOL filter, int filter_ID);

/* path of images - need to be set relative to the computer testing */
char path_input[150]    = "C:\\Users\\Johan\\Dropbox\\festHosMange\\Assignment1\\input\\\0";
char path_output[150]   = "C:\\Users\\Johan\\Dropbox\\festHosMange\\Assignment1\\output\\\0";



void test(char* filename_input, BOOL compressed, BOOL filter, int filter_ID);
void start_ui();

int main(int argc, char *argv[]) {

	test("example24",TRUE, TRUE, 3);
	//start_ui();
	return 0;


}

void start_ui() {
	int option;
	char choice;
	int i, w, h;
	BOOL performance;
	char filename_input[20];
	printf("Do you want to make an performance analysis as you go? (y/n)\n> ");
	scanf("%c", &choice);

	if (&choice == "y") {
		performance = TRUE;
	}else{
		performance = FALSE;
	}

	printf("Choose one of the following options:\n");
	printf(" 1) Capture image\n");
	printf(" 2) Show image\n");
	printf(" 3) Filter image\n");
	printf("> ");
	scanf("%d",&option);

	switch (option) {
	case 1:
	    /* capturing image */
		printf("Name of the file you want to capture:\n> ");
		scanf("%s", filename_input);
	    strcat(path_input, filename_input);
	    strcat(path_input, ".bmp");

	    if (performance) { start = clock(); }

	    ccd_capture_image(path_input);

	    if (performance) {
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			printf("Time used on capturing image: %f sec\n",cpu_time_used);
	    }

		/* transfering image from CCD to MP */
		if (performance) { start = clock(); }

		w = mp_get_height();
		h = mp_get_width();

		ccd_get_width(w);
		ccd_get_height(h);

		lcd_reset_pointer();
		ccd_reset_pointer();

		for (i = 0; i < w * h; i++) {
			mp_set_pixel(ccd_get_pixel());
		}

		if (performance) {
		    end = clock();
		    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		    printf("Time used on transfering from CCD to MP: %f sec\n",cpu_time_used);
		}
		break;
	case 2:
		if (performance) { start = clock(); }

		w = mp_get_height();
		h = mp_get_width();

		lcd_set_width(mp_get_width());
		lcd_set_height(mp_get_height());

		mp_reset_pointer();
		lcd_reset_pointer();

		for (i = 0; i < w * h; i++) {
			lcd_set_pixel(mp_get_pixel());
		}

		if (performance) {
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			printf("Time used on transfering from MP to LCD: %f sec\n",cpu_time_used);
		}

		/* compressing image if told and showing */


		strcat(path_output, "display");
		strcat(path_output, ".bmp");

		if (performance) { start = clock(); }

		lcd_show_image(path_output,TRUE);

		if (performance) {
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			printf("Time used on showing the compressed image: %f sec\n",cpu_time_used);
		}
		break;
	case 3:
		/* filtering image */


		printf("Choose one of the following filters:\n");
		printf(" 0) Black\n");
		printf(" 1) Normal\n");
		printf(" 2) Invert color\n");
		printf(" 3) Emboss (light)\n");
		printf(" 4) Blur (light)\n");
		printf(" 5) Find edges (all directions, light)\n");
		printf(" 6) Sharpen (light)\n");
		printf(" 7) Show edges (light)\n");
		printf(" 8) Mean (light)\n");
		printf(" 9) Find edges (horizontal, medium)\n");
		printf(" 10) Emboss (medium)\n");
		printf(" 11) Find edges (vertical, medium)\n");
		printf(" 12) Motion blur (medium)\n");
		printf(" 13) Find edges (45° direction, medium)\n");
		printf(" 14) Blur (medium)\n");
		printf(" 15) Mean (medium)\n");
		printf(" 16) Shatter (diagonal, medium)\n");
		printf(" 17) 'Fire' effect (medium)\n");
		printf(" 18) Motion blur (Very heavy)\n");
		printf("> ");
		scanf("%d",&option);

		if (performance) { start = clock(); }

		mp_filter_image(option);
		if (performance) {
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			printf("Time used on filtering: %f sec\n",cpu_time_used);
		}

		break;
	default:
		break;
	}
}

void test(char* filename_input, BOOL compressed, BOOL filter, int filter_ID) {
    int i, w, h;
    /* capturing image */

    start = clock();

    strcat(path_input, filename_input);
    strcat(path_input, ".bmp");
    ccd_capture_image(path_input);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used on capturing image: %f sec\n",cpu_time_used);

    /* transfering image from CCD to MP */
    start = clock();

    w = ccd_get_height();
    h = ccd_get_width();

    mp_set_width(w);
    mp_set_height(h);

    mp_reset_pointer();
    ccd_reset_pointer();

    for (i = 0; i < w * h; i++) {
    	mp_set_pixel(ccd_get_pixel());
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used on transfering from CCD to MP: %f sec\n",cpu_time_used);

    /* filtering image if told */

        start = clock();

        mp_filter_image(filter_ID);

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Time used on filtering: %f sec\n",cpu_time_used);


    /* transfering image from MP to LCD */

    start = clock();

    w = mp_get_height();
    h = mp_get_width();

    mp_reset_pointer();
    lcd_reset_pointer();

    lcd_set_width(w);
    lcd_set_height(h);

    for (i = 0; i < w * h; i++) {
        lcd_set_pixel(mp_get_pixel());
    }


    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used on transfering from MP to LCD: %f sec\n",cpu_time_used);

    /* compressing image if told and showing */
    start = clock();

    strcat(path_output, "display");
    strcat(path_output, ".bmp");
    if (compressed) lcd_show_image(path_output,TRUE);
    else lcd_show_image(path_output,FALSE);


    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (compressed)
        printf("Time used on showing the compressed image: %f sec\n",cpu_time_used);
    else
        printf("Time used on showing the uncompressed image: %f sec\n",cpu_time_used);

}



