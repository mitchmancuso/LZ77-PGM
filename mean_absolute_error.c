//
// Created by Mitchell Mancuso on 2019-03-06.
//

#include "mean_absolute_error.h"

//Calculate the mean absolute error between two images and scale if necessary
float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr){
    //Create two PGM Image structures
    struct PGM_Image pgm_image_1;
    struct PGM_Image pgm_image_2;
    //Now load the PGM Image structures with where the files point to
    load_PGM_Image(&pgm_image_1, file_name_1_ptr);
    load_PGM_Image(&pgm_image_2, file_name_2_ptr);
    
    //Set imageWidths and imageHeights based on values from the pgm file
    int image1Width = pgm_image_1.width;
    int image1Height = pgm_image_1.height;
    int image2Width = pgm_image_2.width;
    int image2Height = pgm_image_2.height;
    //Initialize a variable to store the mean abs error
    float mean_abs_error = 0;
    
    //Also get the max gray values and store them
    int image1MaxGrayValue = pgm_image_1.maxGrayValue;
    int image2MaxGrayValue = pgm_image_2.maxGrayValue;
    
    //If the images do not have the same dimensions, print an error on screen and exit the program
    if(image1Width != image2Width || image1Height != image2Height){
        printf("Error: The images do not have the same dimensions, please try again.\n");
        return -1;
    }
    
    //If image 1 has a MaxGrayValue that is less than image 2, scaling must be applied
    if(image1MaxGrayValue < image2MaxGrayValue) {
        for (int row = 0; row < image1Height; row++) {
            for (int col = 0; col < image1Width; col++) {
                //For each pixel in the image, multiply the value of the pixel by the factor of difference between the two, and store
                pgm_image_1.image[row][col] = (unsigned char) ((int) pgm_image_1.image[row][col] *
                                                               (image2MaxGrayValue / image1MaxGrayValue));
            }
        }
    }
    //If image 2 has a MaxGrayValue that is less than image 1, scaling must be applied
    if(image2MaxGrayValue < image1MaxGrayValue){
        for (int row = 0; row < image2Height; row++) {
            for (int col = 0; col < image2Width; col++) {
                //For each pixel in the image, multiply the value of the pixel by the factor of difference between the two, and store
                pgm_image_2.image[row][col] = (unsigned char) ((int) pgm_image_2.image[row][col] *
                                                               (image1MaxGrayValue / image2MaxGrayValue));
            }
        }
    }
    //Now start calculating the mean absolute error total pixel by pixel and store as a running sum
    for(int row = 0; row < image1Height; row++){
        for(int col = 0; col < image1Width; col++){
            //Stores as a running sum that will be calculated next
            mean_abs_error += (pgm_image_1.image[row][col] - pgm_image_2.image[row][col]);
        }
    }
    //Now calculate the final value of the mean abs error based on the formula
    mean_abs_error = mean_abs_error * (1/(image1Height * image1Width));
    
    //Return the mean_abs_error
    return mean_abs_error;
}