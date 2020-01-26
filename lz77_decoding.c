//
// Created by Mitchell Mancuso on 2019-03-06.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "lz77_decoding_function.h"

int main(int argc, char *argv[]){

    //Checks that the correct number of arguments have been provided by the user
    if(argc != 2 ){
        //If not, inform the user as to how to call the program
        printf("Invalid command, one argument is required.\nPlease call the function using ./lz77_decoding [lz77_file]\n");
        exit(0);
    }

    if(access(argv[1], F_OK) == -1){
      printf("File does not exist, please try again.\n");
      exit(0);
    }

    //Set the file name to be the first argument
    char *fileName = argv[1];

    //Create a function to measure the time
    clock_t decompression_time = clock();

    //Call the decoding function
    Decode_Using_LZ77(fileName);

    //Figure out the decompression time as the difference between now and the start
    decompression_time = clock() - decompression_time;

    //Inform the user the decompression has completed
    printf("---------Deompression Complete---------\n");
    printf("-----------Deompression Time-----------\n");
    //Print out the compression time in ms
    printf("%ld ms\n",(decompression_time * 1000)/CLOCKS_PER_SEC);
    printf("----------------------------------------\n");
}
