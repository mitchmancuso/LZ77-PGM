//
// Created by Mitchell Mancuso on 2019-03-06.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "lz77_encoding_function.h"

int main(int argc, char *argv[]){

    //Checks that the correct number of arguments have been provided by the user
    if(argc != 3 ){
        //If not, inform the user as to how to call the program
        printf("Invalid command, two arguments are required.\nPlease call the function using ./lz77_encoding [pgm_file] [searching_buffer_size]\n");
        exit(0);
    }
    if(access(argv[1], F_OK) == -1){
		    printf("File does not exist, please try again.\n");
		    exit(0);
    }

    //Set the file name to be the first argument
    char *fileName = argv[1];
    //Set the searching buffer size to be the second argument
    unsigned int searching_buffer_size = atoi(argv[2]);

    //If that argument for the buffer size is 0, produce an error and exit
    if(searching_buffer_size == 0){
        printf("The searching buffer size cannot be 0, please try again.\n");
        exit(0);
    }

    //Create float values to store the averages and std deviations
    float avg_offset, avg_length, std_offset, std_length;

    //Create a function to measure the time
    clock_t compression_time = clock();

    //Call the encoding function
    Encode_Using_LZ77(fileName, searching_buffer_size, &avg_offset,&std_offset, &avg_length, &std_length);

    //Figure out the compression time as the difference between now and the start
    compression_time = clock() - compression_time;

    //Inform the user the compression has completed and print out the averages and standard deviations
    printf("----------Compression Complete----------\n");
    printf("Offset Average: \t\t\t%f\n",avg_offset);
    printf("Match Length Average: \t\t\t%f\n",avg_length);
    printf("Offset Standard Deviation: \t\t%f\n",std_offset);
    printf("Match Standard Deviation: \t\t%f\n",std_length);
    printf("----------------------------------------\n");
    printf("------------Compression Time------------\n");
    //Print out the compression time in ms
    printf("%ld ms\n",(compression_time * 1000)/CLOCKS_PER_SEC);
    printf("----------------------------------------\n");
}
