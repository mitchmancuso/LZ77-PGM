//
// Created by Mitchell Mancuso on 2019-03-06.
//

#include "lz77_encoding_function.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr){
    //Create a PGM Image structure
    struct PGM_Image* pgm_image = malloc(sizeof(struct PGM_Image));
    //Load into the structure the image referenced by the function parameter
    load_PGM_Image(pgm_image, in_PGM_filename_Ptr);
    //Store the width of the PGM Image in a variable
    int width = pgm_image->width;
    //Store the height of the PGM Image in a variable
    int height = pgm_image->height;
    //Create a variable for the dimensions of the image (size)
    int size = width * height;
    //The PGM images defined may have a comment, so the first 4 or 5 lines are not directly image data. Use 4 as the max (0-4)
    int lineNumber = 4;
    //Set up a pointer to an array of char * to hold the read information
    char **imageInformation = (char**)malloc(4*sizeof(char*));
    //Count will be used to keep track of the header information being pulled
    int count = 0;

    //Open the file to start reading the PGM Image contents
    FILE *input_pgm = fopen(in_PGM_filename_Ptr, "r");
    if ( input_pgm != NULL ) {
        //Create enough space to hold the line
        char line[256];
        //While there are still lines in the file
        while (fgets(line, sizeof line, input_pgm) != NULL) {
            //And if we haven't reached the end of the metadata header information
            if (count < lineNumber) {
                //Store the metadata in the imageInformation array at location count
                imageInformation[count] = (char*) malloc (256 * sizeof(char));
                //Write the line to the array
                sprintf(imageInformation[count],"%s",line);
                //Increase the count, ie before moving on to the next line, ready the imageInformation array to store the next portion of data
                count++;
            }
            //Otherwise, all metadata has been pulled - break
            else {
                break;
            }
        }
        //Close the read
        fclose(input_pgm);
    }
    //If the file cannot be read for whatever reason, report back to the user and exit
    else {
        perror("Cannot open the source image.");
        exit(0);
    }

    //Set up the variables that will store the header or metadata information
    //The first line should always be P5, so get and store that as the image type
    char *imageType = imageInformation[0];
    //Set the rest of the variables to NULL, until they are determined
    char *imageComment = NULL;
    char *imageDimensions = NULL;
    char *imageMaxGrayValue = NULL;

    //If the next line begins with a #, this image has a comment
   if(strncmp(imageInformation[1],"#",1) == 0){
        //Therefore, the comment will be in position 1 in the image information array. Store it as the imageComment.
        imageComment = imageInformation[1];
        //The imageDimensions will therefore be in position 3 in the image information array. Store it as the imageDimensions.
        imageDimensions = imageInformation[2];
        //The maxGrayValue will then be at position 3 in the image information array. Store it as imageMaxGrayValue.
        imageMaxGrayValue = imageInformation[3];
   }
   //Otherwise, this means that there is no comment in the image
   else{
       //So, the dimensions of the image will be in position 1 in the image information array. Store it as the imageDimensions.
       imageDimensions = imageInformation[1];
       //And as well, the maxGrayValue will then be in position 2 in the image information array. Store it as the imageGrayValue.
       imageMaxGrayValue = imageInformation[2];
   }
   //Create an array that will be used to store the pixels read from the PGM Image
    int imageArray[size];
   //Keep track of the positioning in the 1D array with a variable
    int pos = 0;
    //Traverse through the height and width of the pgm image
    for(int row = 0; row < pgm_image->height; row++){
        for(int col = 0; col < pgm_image->width; col++){
            //Save the pixels in the 1D imageArray as it will be used to calculate the LZ77
            imageArray[pos] = pgm_image->image[row][col];
            //Increment the position
            pos++;
        }
    }
    //This creates an array to store all of the offsets (that will be added to the lz file)
    int *offsets = malloc(size * sizeof(int));
    //This creates an array to store all of the matching lengths (that will be added to the lz file)
    int *matching_lengths = malloc(size * sizeof(int));
    //This creates an array to store all of the next_symbols (that will be added to the lz file)
    int *next_symbols = malloc(size * sizeof(int));

    //Create an integer that stores the position of the longest match (start)
    int longest_match_pos = 0;
    //Create an integer that holds the length of the longest match
    int longest_match_length = 0;
    //Current position and current value are used for traversing through the array
    int curr_pos = 0;
    int curr_value = 0;
    //Keep track of the total number of token (x,y,z) sets
    int token_count = 0;
    int new_pos = 0;

    //Keeps the position pointer within the confines of the 1D array size
    //After completion of the loop, sets the new pointer current position pointer to be after the longest string found
    for(;curr_pos < size - 1;curr_pos += longest_match_length + 1){
        //Resets the longest_match_pos and longest_match_length on each call of the loop.
        longest_match_pos = 0;
        longest_match_length = 0;

        //Create an integer that represents the max searching_buffer_size - 1 (to account for 0 - searching_buffer_size)
        int maxSize = searching_buffer_size - 1;

        //Get the current pixel value to encode
        curr_value = imageArray[curr_pos];
        //Start the process to search for a match, taking into account there is a max searching_buffer_size, take the min of the two values
        for(int i = 1; i <= MIN(curr_pos, maxSize); i++){
            //If the current position = the current value (as it initially should)
            if(imageArray[curr_pos] == curr_value){
                //Start declaring the variables for the match_len, position, and the next_ptr, which points to the next element after all the match
                int match_len = 0;
                int match_pos = curr_pos-i;
                int next_ptr = curr_pos;
                //If the imageArray at the matchPosition also has a match at the next pointer (and the array has not been exceeded)
                while(imageArray[match_pos] == imageArray[next_ptr] && match_pos < size && next_ptr < size){
                    //Increment counters for the match length, position, and position of the next pointer
                    match_len++;
                    match_pos++;
                    next_ptr++;
                }
                //Keep track of the longest element in total against the match_len which is the current match
                if(match_len > longest_match_length){
                    longest_match_pos = i;
                    longest_match_length = match_len;
                }
            }
        }
        //Set the new_pos where the pointer ends at due to the longest match length
        new_pos = curr_pos + longest_match_length;

        //Get the offset array and add the longest_match_pos as the offset portion of the token
        offsets[token_count] = longest_match_pos;
        //Get the longest_match_length and store it in the array
        matching_lengths[token_count] = longest_match_length;
        //If the new_pos is the size of the array
        if(new_pos == size){
            //Store an EOF char to let the token know that it should contain the last digit
            next_symbols[token_count++] = EOF;
        }
        else{
            //Otherwise, the mismatch symbol should be what is at the new position
            next_symbols[token_count++] = imageArray[new_pos];
        }
    }
    //Print the tokens for testing
    /*
    for(int i = 0;i<token_count;i++){
        printf("(%d,%d,%d)\n",offsets[i],matching_lengths[i],next_symbols[i]);
    }
    printf("Token Count Is: %d\n",token_count);
     */


    //Saving the file as a compressed file (.lz)
    //Create a char array of appropriate size to store the filename
    char newFileName [200];
    //Use sprintf to start building the filename, since .[searching_buffer_size] will be part of the filename, save it for easy access
    sprintf(newFileName,"%s.%d.lz",in_PGM_filename_Ptr,searching_buffer_size);

    //Create char arrays of appropriate size to store the csv filenames
    char newFileNameOffsetsCSV [200];
    char newFileNameLengthsCSV [200];

    //Build and save the filenames for the csv files in their respective variables
    sprintf(newFileNameLengthsCSV,"%s.%d.lengths.csv",in_PGM_filename_Ptr,searching_buffer_size);
    sprintf(newFileNameOffsetsCSV,"%s.%d.offsets.csv",in_PGM_filename_Ptr,searching_buffer_size);

    //Create a new file object that will be for the lz compressed file
    FILE *output_lz = fopen(newFileName, "wb");
    //This prints the header information starting with the imageType
    fprintf(output_lz,"%s",imageType);
    //If there's a comment, put it in the file
    if(imageComment != NULL){
        fprintf(output_lz,"%s",imageComment);
    }
    //Next, add the dimensions and maxGrayValue
    fprintf(output_lz,"%s",imageDimensions);
    fprintf(output_lz,"%s",imageMaxGrayValue);
    //Now add the searching buffer size
    char searching_buffer_size_string[sizeof(int) * 4 + 1];
    sprintf(searching_buffer_size_string,"%d\n",searching_buffer_size);
    fprintf(output_lz,"%s",searching_buffer_size_string);
    //Now add the number of tokens
    char token_count_string[sizeof(int) * token_count + 1];
    sprintf(token_count_string,"%d\n",token_count);
    fprintf(output_lz,"%s",token_count_string);
    //The first line after the header will have the offsets separated by spaces
    for(int i = 0; i < token_count;i++) {
        fprintf(output_lz,"%d ",offsets[i]);
    }
    //Add a new line (this will help the decoder to realize that we've finished reading the offsets
    fprintf(output_lz,"\n");
    //The second line after the header will have the match_lengths separated by spaces
    for(int i = 0; i < token_count;i++) {
        fprintf(output_lz,"%d ",matching_lengths[i]);
    }
    //Add a new line (this will help the decoder to realize that we've finished reading the match lengths
    fprintf(output_lz,"\n");
    //The third line after the header will have the next_symbols as integer values separated by spaces
    for(int i = 0; i < token_count;i++) {
        fprintf(output_lz,"%d ",next_symbols[i]);
    }
    //That's it, close the file
    fclose(output_lz);

    //Now create a new file that will be used for the CSV related to the offsets
    FILE *output_offsets_csv = fopen(newFileNameOffsetsCSV, "wb");
    //Create a new array that will be used to count the frequencies
    int *offsets_frequency = malloc(size * sizeof(int));
    //For each token, count the frequencies and increment by 1 each time
    for(int i = 0; i < token_count; i++){
        offsets_frequency[offsets[i]]++;
        //printf("Position %d is %d\n",offsets[i],offsets_frequency[offsets[i]]);
    }
    //Now, for each offset frequency, write to file in csv format
    for(int i = 0; i < (int) searching_buffer_size; i++){
        //If the frequency is greater than 0, write it to the file, otherwise ignore
        if(offsets_frequency[i] > 0){
            //Store the offset,frequency as the csv
            fprintf(output_offsets_csv,"%d,%d\n",i,offsets_frequency[i]);
        }
    }
    //That's it, close the file
    fclose(output_offsets_csv);

    //Now create a new file that will be used for the CSV related to the match lengths
    FILE *output_lengths_csv = fopen(newFileNameLengthsCSV, "wb");
    //Create a new array that will be used to count the frequencies
    int *match_length_frequency = malloc(size * sizeof(int));
    //For each token, count the frequencies and increment by 1 each time
    for(int i = 0; i < token_count; i++){
        match_length_frequency[matching_lengths[i]]++;
    }

    //Now, for each match length frequency, write to file in csv format
    for(int i = 0; i < size; i++){
        //If the frequency is greater than 0, write it to the file, otherwise ignore
        if(match_length_frequency[i] > 0){
            //Store the match_length,frequency as the csv
            fprintf(output_lengths_csv,"%d,%d\n",i,match_length_frequency[i]);
        }
    }
    //That's it, close the file
    fclose(output_lengths_csv);

    //Start by calculating the average
    //The offset_sum and match_length sum will be used to keep a running total of the sum
    float offset_sum = 0.0;
    float match_length_sum = 0.0;
    //For each token that exists, calculate the running total
    for(int i = 0; i < token_count; i++){
        //Add the offset values together for each token
        offset_sum = offset_sum + offsets[i];
        //Add the match length values together for each token
        match_length_sum = match_length_sum + matching_lengths[i];
    }
    //Find the averages using the sum / number of items
    *avg_offset_Ptr = offset_sum / token_count;
    *avg_length_Ptr = match_length_sum / token_count;
    //printf("Average offset is: %f\n",*avg_offset_Ptr);
    //printf("Average length is: %f\n",*avg_length_Ptr);



    //Create variables that will store the variance (standard deviation ^2)
    float offset_var = 0.0;
    float match_length_var = 0.0;
    //For each token, calculate the variance
    for(int i = 0; i < token_count; i++){
        //Keep track of the offset variance in the offset_var total
        offset_var = offset_var + pow(offsets[i] - *avg_offset_Ptr,2);
        //Keep track of the match length variance in the match_length_var total
        match_length_var = match_length_var + pow(matching_lengths[i] - *avg_length_Ptr,2);
    }
    //Find the standard deviation by taking the square root of the variance / n
    *std_offset_Ptr = sqrt(offset_var / token_count);
    *std_length_Ptr = sqrt(match_length_var / token_count);
    //printf("Standard Deviation of offset is: %f\n",*std_offset_Ptr);
    //printf("Standard Deviation of length is: %f\n",*std_length_Ptr);


    //Free all objects that were malloc'ed along the way
    free(pgm_image);
    free(imageInformation);
    free(offsets);
    free(matching_lengths);
    free(next_symbols);
    free(offsets_frequency);
    free(match_length_frequency);
}
