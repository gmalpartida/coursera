/******************************************************************************
 * Copyright (C) 2017 by Alex Fosdick - University of Colorado
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are 
 * permitted to modify this and use it to learn about the field of embedded
 * software. Alex Fosdick and the University of Colorado are not liable for any
 * misuse of this material. 
 *
 *****************************************************************************/
/**
 * @file stats.c
 * @brief Implementation file for all the functions in the program.
 *
 * Reads an array of 40 numbers and performs the following calculations:
 * 	mean, median, maximum and minimum
 * Then it displays the statistics on the screen.
 *
 * @author Gino Malpartida
 * @date 10-22-2024
 *
 */

#include <stdio.h>
#include "stats.h"
#include <stdlib.h>

/* Size of the Data Set */
#define SIZE (40)

void main() {

  unsigned char test[SIZE] = { 34, 201, 190, 154,   8, 194,   2,   6,
                              114, 88,   45,  76, 123,  87,  25,  23,
                              200, 122, 150, 90,   92,  87, 177, 244,
                              201,   6,  12,  60,   8,   2,   5,  67,
                                7,  87, 250, 230,  99,   3, 100,  90};

  /* Other Variable Declarations Go Here */
  /* Statistics and Printing Functions Go Here */
    print_statistics(test, SIZE);
}

void print_array(unsigned char array[], int size)
{
    printf("Test data:\n");
    for (int i = 0; i < size; i++)
    {
       printf("%10.0d", array[i]);
       if (0 == ((i+1) % 10))
       {
           printf("\n");
       }
           
    }
    printf("\n");
}


void print_statistics(unsigned char array[], int size)
{
    print_array(array, size);
    sort_array(array, size);
    int median = find_median(array, size);
    int mean = find_mean(array, size);
    int maximum = find_maximum(array, size);
    int minimum = find_minimum(array, size);
    printf("Median: %-6d Mean: %-6d Maximum: %-6d Minimum: %-6d\n", median, mean, maximum, minimum);
}


int find_median(unsigned char array[], int size)
{
    int mid = size / 2;

    return array[mid];
}

int find_mean(unsigned char array[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum / size;
}

int find_maximum(unsigned char array[], int size)
{
    return array[0];
}

int find_minimum(unsigned char array[], int size)
{
    return array[size-1];
}

int cmp_ints(const void * p1, const void * p2)
{
    int result = *((unsigned char*)p2) - *((unsigned char*)p1); 
    return result;
}

void sort_array(unsigned char *array, int size)
{
    qsort(array, size, sizeof(unsigned char), cmp_ints);
}







