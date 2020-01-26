//
// Created by Mitchell Mancuso on 2019-03-06.
//

#include "lz77_decoding_function.h"

#define MAX_SMALL_SIZE 1000
#define MAX_LINE_SIZE 1000000

void Decode_Using_LZ77(char *in_compressed_filename_Ptr){
    //Used as temporary character storage when counting the number of lines in a file
    char c_temp;
    //Initialize the couner for the number of lines in a file
    int line_count = 1;
    //Since we're not sure of the size of things until they are pulled from the file, create arrays of reasonable size to store everything in
    //This will store the image type which is the first line of the file
    char imageType[MAX_SMALL_SIZE];
    //This will store the comment line (if there is one)
    char imageComment[MAX_SMALL_SIZE];
    //This will store the next line of the lz file which should be the dimensions
    char imageDimensions[MAX_SMALL_SIZE];
    //This will store the gray value which is the next line of the file
    char imageMaxGrayValueTemp[MAX_SMALL_SIZE];
    //This will store the searching buffer size, which is the next line of the file
    char imageSearchingBufferSizeTemp[MAX_SMALL_SIZE];
    //This will store the number of tokens in the lz file
    char imageNumberOfTokensTemp[MAX_SMALL_SIZE];
    //This will store the offset values portion of the token
    char imageOffsetValues[MAX_LINE_SIZE];
    //This will store the matching lengths portion of the token
    char imageMatchingLengths[MAX_LINE_SIZE];
    //This will store the next values portion of the token
    char imageNextValues[MAX_LINE_SIZE];
    
    //These variables will be used to hold converted properties loaded from the lz file
    int imageWidth, imageHeight, imageSize, imageMaxGrayValue;
    int imageNumberOfTokens;
    //int imageSearchingBufferSize;
    
    //Open the file, just to count the number of lines
    FILE *file_to_count = fopen(in_compressed_filename_Ptr, "r");
    
    //While we're not at the end of file, search through the characters
    for (c_temp = getc(file_to_count); c_temp != EOF; c_temp = getc(file_to_count))
        //If the newline is found
        if (c_temp == '\n')
            //Increment the counter and store
            line_count = line_count + 1;
        
    //Close the file, the counting is done
    fclose(file_to_count);
    
    //Now create a new file, this will be for the main read of the image
    FILE *lz_file = fopen(in_compressed_filename_Ptr, "r");
    
    //If there are 9 lines in the read file, that means it was encoded with a comment, therefore it will have to be accounted for
    if(line_count == 9){
        //The first line is the image size, store that in the imageType variable
        fgets(imageType,MAX_SMALL_SIZE,lz_file);
        //The next line is the image comment, store that in the imageComment variable
        fgets(imageComment,MAX_SMALL_SIZE,lz_file);
        //The next line is the image dimensions, store that in the imageDimensions variable
        fgets(imageDimensions,MAX_SMALL_SIZE,lz_file);
        //The next line is the gray value, store that in the imageMaxGrayValue variable
        fgets(imageMaxGrayValueTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the gray value to an integer
        //Create a char * to temporarily hold the working string
        char *imageGrayValueEval;
        //Use strtol to convert the string value of maxGrayValue to an integer and store it
        imageMaxGrayValue = (int) strtol(imageMaxGrayValueTemp,&imageGrayValueEval,10);
        //The next line is the searchingBuffer
        fgets(imageSearchingBufferSizeTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the searching buffer size to an integer
        //Create a char * to temporarily hold the working string
        //char *imageSearchingBufferSizeEval;
        //Use strtol to convert the string value of searchingBufferSize to an integer and store it
        //imageSearchingBufferSize = (int) strtol(imageSearchingBufferSizeTemp,&imageSearchingBufferSizeEval,10);
        //The next line is the numberOfTokens
        fgets(imageNumberOfTokensTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the numberOfTokens to an integer
        //Create a char * to temporarily hold the working string
        char *imageNumberOfTokensEval;
        //Use strtol to convert the string value of numberOfTokens to an integer and store it
        imageNumberOfTokens = (int) strtol(imageNumberOfTokensTemp,&imageNumberOfTokensEval,10);
        //The next line contains all of the offset values in the token
        fgets(imageOffsetValues,MAX_LINE_SIZE,lz_file);
        //The next line contains all of the matching length values in the token
        fgets(imageMatchingLengths,MAX_LINE_SIZE,lz_file);
        //The next line contains all of the next values in the token
        fgets(imageNextValues,MAX_LINE_SIZE,lz_file);
    }
    //Otherwise, there is no comment present, change the read structure
    else {
        //The first line is the image size, store that in the imageType variable
        fgets(imageType,MAX_SMALL_SIZE,lz_file);
        //The next line is the image dimensions, store that in the imageDimensions variable
        fgets(imageDimensions,MAX_SMALL_SIZE,lz_file);
        //The next line is the gray value, store that in the imageMaxGrayValue variable
        fgets(imageMaxGrayValueTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the gray value to an integer
        //Create a char * to temporarily hold the working string
        char *imageGrayValueEval;
        //Use strtol to convert the string value of maxGrayValue to an integer and store it
        imageMaxGrayValue = (int) strtol(imageMaxGrayValueTemp,&imageGrayValueEval,10);
        //The next line is the searchingBuffer
        fgets(imageSearchingBufferSizeTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the searching buffer size to an integer
        //Create a char * to temporarily hold the working string
        //char *imageSearchingBufferSizeEval;
        //Use strtol to convert the string value of searchingBufferSize to an integer and store it
        //imageSearchingBufferSize = (int) strtol(imageSearchingBufferSizeTemp,&imageSearchingBufferSizeEval,10);
        //The next line is the numberOfTokens
        fgets(imageNumberOfTokensTemp,MAX_SMALL_SIZE,lz_file);
        //Now, we'll convert the numberOfTokens to an integer
        //Create a char * to temporarily hold the working string
        char *imageNumberOfTokensEval;
        //Use strtol to convert the string value of numberOfTokens to an integer and store it
        imageNumberOfTokens = (int) strtol(imageNumberOfTokensTemp,&imageNumberOfTokensEval,10);
        //The next line contains all of the offset values in the token
        fgets(imageOffsetValues,MAX_LINE_SIZE,lz_file);
        //The next line contains all of the matching length values in the token
        fgets(imageMatchingLengths,MAX_LINE_SIZE,lz_file);
        //The next line contains all of the next values in the token
        fgets(imageNextValues,MAX_LINE_SIZE,lz_file);
    }
    //Close the file, nothing else to be read
    fclose(lz_file);
    
    //Since the dimensions need to be separated in terms of width and height, they must be tokenized
    //Create an array that will hold 2 dimensions of max length 10
    char dimension[2][10];
    //Declare integers that will be used to help count
    int word_ctr,dim_ctr;
    //Set them to 0 to start
    word_ctr=0; dim_ctr=0;
    //While we have not reached the end of the line
    for(int i = 0;i <= (strlen(imageDimensions));i++) {
        //If there is a space then store an EOL char and increment the counter (as the "word" is done)
        if(imageDimensions[i]==' '||imageDimensions[i]=='\0') {
            //Store an EOL as the word (token) is complete
            dimension[dim_ctr][word_ctr]='\0';
            //Increment to hold the next 
            dim_ctr++;
            //Reset the index of the "word" (to find the numerical value after the space)
            word_ctr = 0;
        }
        else {
            //Otherwise, store it in the current position in the array
            dimension[dim_ctr][word_ctr]=imageDimensions[i];
            //Increment the dimension of the array in order to store integers more than length of 1 in the same line
            word_ctr++;
        }
    }
    
    //Create two temporary working spaces for imageWidth and imageHeight evaluations
    char *imageWidthEval;
    char *imageHeightEval;
    
    //Now call the strtol function in order to store the values of the array at positions 0 and 1 as width and height respectively in integer value
    imageWidth = (int) strtol(dimension[0],&imageWidthEval,10);
    imageHeight = (int) strtol(dimension[1],&imageHeightEval,10);
    //Store the imageSize as an integer representation of width x height
    imageSize = imageWidth * imageHeight;
    
    //For testing that the integer values were read and stored correctly
    //printf("Image Width: %d | Image Height: %d | Max Gray Value Is: %d\n",imageWidth, imageHeight,imageMaxGrayValue);
    //printf("Searching Buffer Size Is: %d | Number Of Tokens Is: %d\n",imageSearchingBufferSize,imageNumberOfTokens);
    
    //Now we'll start storing the offset values
    char imageOffsetsTemp[imageNumberOfTokens][10];
    //Reset the counters to 0 in order to have them ready to tokenize the string (that is, each token)
    word_ctr=0; dim_ctr=0;
    //While we have not reached the end of the line
    for(int i = 0;i <= (strlen(imageOffsetValues));i++) {
        //If there is a space then store an EOL char and increment the counter (as the "word" is done)
        if(imageOffsetValues[i]==' '||imageOffsetValues[i]=='\0') {
            //Store an EOL as the word (token) is complete
            imageOffsetsTemp[dim_ctr][word_ctr]='\0';
            //Increment to hold the next 
            dim_ctr++;
            //Reset the index of the "word" (to find the numerical value after the space)
            word_ctr = 0;
        }
        else {
            //Otherwise, store it in the current position in the array
            imageOffsetsTemp[dim_ctr][word_ctr]=imageOffsetValues[i];
            //Increment the dimension of the array in order to store integers more than length of 1 in the same line
            word_ctr++;
        }
    }
    
    //Create a temporary working space for imageOffsets
    char *imageOffsetValuesEval;
    //Create an array of ints that will store each tokenized offset value
    int offsets[imageNumberOfTokens];
    //Since we know how many tokens there are, traverse the array at each spot and convert the string to an integer
    for(int i = 0; i < imageNumberOfTokens; i++) {
        //Store the converted string representation of the number into the offsets array
        offsets[i] = (int) strtol(imageOffsetsTemp[i], &imageOffsetValuesEval, 10);
    }
    /*
    //For testing that the tokens stored in the offsets array are correct
    for(int i = 0; i< imageNumberOfTokens; i++){
        printf("%d ",offsets[i]);
    }
    */
    
    //Now we'll start storing the match length values
    char imageMatchLengthsTemp[imageNumberOfTokens][10];
    //Reset the counters to 0 in order to have them ready to tokenize the string (that is, each token)
    word_ctr=0; dim_ctr=0;
    //While we have not reached the end of the line
    for(int i = 0;i <= (strlen(imageMatchingLengths));i++) {
        //If there is a space then store an EOL char and increment the counter (as the "word" is done)
        if(imageMatchingLengths[i]==' '||imageMatchingLengths[i]=='\0') {
            //Store an EOL as the word (token) is complete
            imageMatchLengthsTemp[dim_ctr][word_ctr]='\0';
            //Increment to hold the next 
            dim_ctr++;
            //Reset the index of the "word" (to find the numerical value after the space)
            word_ctr=0;
        }
        else {
            //Otherwise, store it in the current position in the array
            imageMatchLengthsTemp[dim_ctr][word_ctr]=imageMatchingLengths[i];
            //Increment the dimension of the array in order to store integers more than length of 1 in the same line
            word_ctr++;
        }
    }
    
    //Create a temporary working space for matchingLengths
    char *imageMatchingLengthsEval;
    //Create an array of ints that will store each tokenized matching length value
    int matching_lengths[imageNumberOfTokens];
    //Since we know how many tokens there are, traverse the array at each spot and convert the string to an integer
    for(int i = 0; i < imageNumberOfTokens; i++) {
        //Store the converted string representation of the number into the matching lengths array
        matching_lengths[i] = (int) strtol(imageMatchLengthsTemp[i], &imageMatchingLengthsEval, 10);
    }
    /*
    //For testing that the tokens stored in the matching lengths array are correct
    printf("\n");
    for(int i = 0; i< imageNumberOfTokens; i++){
        printf("%d ",matching_lengths[i]);
    }
    */
    
    //Now we'll start storing the next values (mismatches)
    char imageNextValuesTemp[imageNumberOfTokens][10];
    //Reset the counters to 0 in order to have them ready to tokenize the string (that is, each token)
    word_ctr=0; dim_ctr=0;
    //While we have not reached the end of the line
    for(int i = 0;i <= (strlen(imageNextValues));i++) {
        //If there is a space then store an EOL char and increment the counter (as the "word" is done)
        if(imageNextValues[i]==' '||imageNextValues[i]=='\0') {
            //Store an EOL as the word (token) is complete
            imageNextValuesTemp[dim_ctr][word_ctr]='\0';
            //Increment to hold the next 
            dim_ctr++;
            //Reset the index of the "word" (to find the numerical value after the space)
            word_ctr=0;
        }
        else {
            //Otherwise, store it in the current position in the array
            imageNextValuesTemp[dim_ctr][word_ctr]=imageNextValues[i];
            //Increment the dimension of the array in order to store integers more than length of 1 in the same line
            word_ctr++;
        }
    }
    
    //Create a temporary working space for nextValues
    char *imageNextValuesEval;
    //Create an array of ints that will store each tokenized next value
    int next_values[imageNumberOfTokens];
    //Since we know how many tokens there are, traverse the array at each spot and convert the string to an integer
    for(int i = 0; i < imageNumberOfTokens; i++) {
        //Store the converted string representation of the number into the next values array
        next_values[i] = (int) strtol(imageNextValuesTemp[i], &imageNextValuesEval, 10);
    }
    /*
    //For testing that the tokens stored in the next values array are correct
    printf("\n");
    for(int i = 0; i< imageNumberOfTokens; i++){
        printf("%d ",next_values[i]);
    }
    printf("\n");
    */
     
    //Now everything is read and stored in its proper format, start building the PGM image
    struct PGM_Image* pgm_image = malloc(sizeof(struct PGM_Image));
    //Creat the PGM image using the MaxGrayValue, Width, and Height
    create_PGM_Image(pgm_image, imageWidth, imageHeight, imageMaxGrayValue);
    
    //Like in the encoder, the image values will be stored in a single dimension array to set values
    unsigned char *imageArray = malloc(sizeof(unsigned char)*imageSize);
    
    //Create pointers to help traverse the arrays and determine which values to read, write, and start from
    int readPos = 0;
    int writePos = 0;
    int startPos = 0;
    //currPos will be used to construct the 1D array that will represent the PGM Image pixel value
    int currPos = 0;
    
    //For each number of tokens in the file
    for(int i = 0;i < imageNumberOfTokens;i++) {
        //The reading position will be related to the writePos - the offsetValue at the current token
        readPos = writePos - offsets[i];
        //The start of the array will be the writing position
        startPos = writePos;
        
        //This writes the matching length portion by expansion
        for(int j = 0;j < matching_lengths[i];j++) {
            //The pointer goes past, this falls under the the scenario that things are repeated (like the shshshshsh example from lecture)
            if(startPos == readPos) {
                //Now change the reading position
                readPos = startPos - offsets[i];
            }
            //Write the results of what should be matched at the positon
            *(imageArray + writePos) = *(imageArray + readPos);
            //Increment the read and write position and continue the loop until the full length of the match has been recorded
            readPos++;
            writePos++;
        }
        
        //Now write the mismatch (next_value) to the image array
        *(imageArray + writePos) = (unsigned char) next_values[i];
        //Increment the write pointer and move on to the next token
        writePos++;
    }
    
    //Now that all tokens have been read, expanded upon, and stored in the 1D imageArray, start building the PGM image
    //Go pixel by pixel for the entire dimensions of the image
    for(int i = 0;i < imageHeight;i++) {
        for(int j = 0;j < imageWidth;j++) {
            //Set the current pixel to be what is stored in the 1D image array
            pgm_image->image[i][j]= imageArray[currPos++];
        }
    }
    //Now, create the final fileName that will be saved back (just the original filename + .pgm)
    char finalFileName[MAX_LINE_SIZE];
    //Create the new finalFileName from the combination of these two things (original filename + .pgm)
    sprintf(finalFileName, "%s.pgm", in_compressed_filename_Ptr);
    //Now save the pgm image
    save_PGM_Image(pgm_image, finalFileName, 1);
    
    //Free the various memory
    free(pgm_image);
}