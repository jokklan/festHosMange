/********************************************************************************
 * Test (main)
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccd.h"
#include "lcd.h"
#include <time.h>
#include "mp.h"
#include "ui.h"

void go_ui();

int main(int argc, char *argv[]) {
	WORD i, w, h;
	BOOL ui = TRUE;

	if (ui) {
		go_ui();
	} else {

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
		for (i = 0; i < w * h; i++)
			lcd_set_pixel(ccd_get_pixel());

		/* show image on lcd */
		lcd_show_image();
	}

	return 0;
}

void go_ui() {

	BOOL performance = FALSE;
	char filename[20];
	char filename2[20];
	clock_t start, end;
	char yn[1];
	double cpu_time_used;
	/*do {*/
	printf("Do you want performance analysis? (y/n)\n> ");
		/*scanf("%s", yn);
	} while (strncmp(yn, "y", 1) != 0 && strncmp(yn, "n", 1) != 0);*/
	/*performance = (strncmp(yn, "y", 1) == 0);*/

	int currentScreen = ui_start_screen();

	while (currentScreen != -1) {
		switch (currentScreen) {
		case 0: // CHOOSE FUNCTION
			// the menu
			currentScreen = ui_start_screen();
			break;
		case 1: // CAPTURE IMAGE
			// capturing image
			do {
				printf("Name of the file you want to capture:\n> ");
				scanf("%s", filename);
			} while (strncmp(filename, "-1", 1) != 0 && !mp_get_image(filename));

			if (strncmp(filename, "-1", 1) != 0) {
				// give name to captured image
				printf("Give a name to the captured image:\n> ");
				scanf("%s", filename2);
				printf("Filename2 before = %s\n", filename2);

				// compress image?
				do {
					printf("Do you want compress the picture? (y/n)\n> ");
					scanf("%s", yn);
				} while (strncmp(yn, "y", 1) != 0 && strncmp(yn, "n", 1) != 0);

				printf("Filename2 after = %s\n", filename2);
				if (strncmp(yn, "y", 1) == 0) {
					// saving compressed
					if (!mp_compress_image(filename2)) {
						printf("Failed compressing captured image");
					}
				} else {
					// saving uncompressed
					if (!mp_save_image(filename2)) {
						printf("Failed saving captured image");
					}
				}
			}

			currentScreen = 0;
			break;
		case 2: // COMPRESS IMAGE
			// loading the picture
			start = clock();
			do {
				printf("Name of the file you want to compress:\n> ");
				scanf("%s", filename);
			} while (strncmp(filename, "-1", 1) != 0
					&& !mp_load_image(filename, FALSE));
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			// performance check ?
			if (performance)
				printf("CPU time used on loading image: %f sec\n",
						cpu_time_used);

			start = clock();
			if (strncmp(filename, "-1", 1) != 0) {
				// compressing chosen image. give name to compressed image
				do {
					printf("Give a name to the compressed picture:\n> ");
					scanf("%s", filename);
				} while (!mp_compress_image(filename) && *filename != -1);
			}
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			// performance check ?
			if (performance)
				printf("CPU time used on saving compressing image: %f sec\n",
						cpu_time_used);

			currentScreen = 0;
			break;
		case 3: // FILTER IMAGE
			// getting the image
			do {
				printf("Name of the file you want to filter:\n> ");
				scanf("%s", filename);
			} while (strncmp(filename, "-1", 1) != 0
					&& !mp_load_image(filename, FALSE));

			if (strncmp(filename, "-1", 1) != 0) {
				// choose filter
				int filter_id = ui_filter_screen();
				int extra_filter_id = ui_filter_screen();

				// filter the image
				start = clock();
				if (!mp_filter_image(filter_id, extra_filter_id)) {
					printf("ERROR: occured when trying to filter with UI");
				}
				end = clock();
				cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

				// performance check ?
				if (performance)
					printf("CPU time used on filtering: %f sec\n",
							cpu_time_used);

				// name of new filtered image
				printf("Give a name to the filtered image:\n> ");
				scanf("%s", filename2);
				start = clock();
				if (!mp_save_image(filename2)) {
					printf("Failed saving captured image");
				}
				end = clock();
				cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

				// performance check ?
				if (performance)
					printf("CPU time used on saving filtered image: %f sec\n",
							cpu_time_used);
			}
			currentScreen = 0;
			break;
		case 4: // SHOW IMAGE
			// show image
			currentScreen = ui_showimage_screen();
			break;
		default:
			// try again
			printf("Function not recognized. Try again\n");
			currentScreen = 0;
			break;
		}
	}
}
