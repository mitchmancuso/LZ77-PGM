//
// Created by Mitchell Mancuso on 2019-03-06.
//

#include <stdio.h>
#include <stdlib.h>
#include "libpnm.h"

#include "mean_absolute_error.h"

int main(int argc, char *argv[]) {
//Checks that the correct number of arguments have been provided by the user
    if (argc != 3) {
//If not, inform the user as to how to call the program
        printf("Invalid command, two arguments are required.\nPlease call the function using ./compare_pgm_images [pgm_file1] [pgm_file2]\n");
        exit(0);
    }
//Store the arguments as the image file pointers
    char *imageFile1 = argv[1];
    char *imageFile2 = argv[2];

//Call the function to determine the mean absolute error
    float mean_abs_error = mean_absolute_error(imageFile1, imageFile2);
//Print the result to the user
    printf("The Mean Absolute Error is: %f\n", mean_abs_error);
}