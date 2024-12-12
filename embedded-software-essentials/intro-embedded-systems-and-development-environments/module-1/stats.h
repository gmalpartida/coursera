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
 * @file stats.h 
 * @brief header file containing the function declarations used in the program.
 
 * @author Gino Malpartida
 * @date 10-22-2024
 *
 */
#ifndef __STATS_H__
#define __STATS_H__

/* Size of the Data Set */
#define SIZE (40)


/**
 * @brief print statistics of the array
 *
 * @param array array of integer values
 * @param size size of the array
 *
 * @return none
 */
void print_statistics(unsigned char array[], int size);

/**
 * @brief prints the contents of the array on the screen
 *
 * @param array	array of integers to be printed on the screen.
 * @param size size of the array
 *
 * @return none
 */
void print_array(unsigned char array[], int size);

/**
 * @brief calculates the median of the array of integer values
 *
 * @param array	array of integer values
 * @param size size of the array
 * @return median of the array of integer values
 */
int find_median(unsigned char array[], int size);

/**
 * @brief calculates the mean of the array of integer values
 *
 * @param array	array of integer values
 * @param size size of the array

 * @return mean of the array of integer values
 */
int find_mean(unsigned char array[], int size);

/**
 * @brief find the maximum value in the array of integer values
 *
 * @param array array of integer values
 * @param size size of the array
 * @return maximum value in the array
 */
int find_maximum(unsigned char array[], int size);

/**
 * @brief finds the minimum value in the array of integer values
 *
 * @param array array of integer values
 * @param size size of the array
 * @return minimum value in the array
 */
int find_minimum(unsigned char array[], int size);

/**
 * @brief sorts the values from largest to smallest
 *
 * @param array array of integer values
 * @param size size of the array
 * @return sorted array
 */
void sort_array(unsigned char * array, int size);

#endif /* __STATS_H__ */
