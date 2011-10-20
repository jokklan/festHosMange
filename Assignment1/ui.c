/*
 * ui.c
 *
 *  Created on: 20/10/2011
 *      Author: Johan
 */

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int ui_start_screen() {
    int option;
    printf("Choose one of following options:\n");
    printf(" 1) Capture image\n");
    printf(" 2) Compress image\n");
    printf(" 3) Filter image\n");
    printf(" 4) Show image\n");
    printf("> ");
    scanf("%d",&option);
    return option;
}

int ui_filter_screen() {
    int option;
    printf("Choose the filter you want to use:\n");
    printf("1) Soft blur\n");
    printf("2) Hard blur\n");
    printf("3) Motion blur right\n");
    printf("4) Edge filter\n");
    printf("5) Emboss 1\n");
    printf("6) Emboss 2\n");
    printf("7) Emboss 3\n");
    printf("8) Sharpen edges\n");
    printf("> ");
    scanf("%d",&option);
    return option - 1;
}

int ui_showimage_screen() {
    printf("This is a shown image\n");
    return 0;
}
