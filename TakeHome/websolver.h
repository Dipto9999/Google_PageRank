/*
 * File: websolver.h
 * Purpose: Contains Constants, Prototypes, Globals
 * Author: Muntakim Rahman
 * Date: September 28th 2021
 */

#pragma once

/*******************/
/* Include Headers */
/*******************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"

/***************************/
/* Preprocessor Directives */
/***************************/

#define WEB "web.txt"

#define INPUT_BUFF 128
#define OUTPUT_BUFF 1024

#define OMIT_SPACES 2

#define TRUE 1
#define FALSE 0
#define ERROR -1

/***********************/
/* Function Prototypes */
/***********************/

size_t getDimension(FILE* web_file);

void parseMatrix(FILE* web_file, size_t dimension, double** matrix);
void printMatrix(Engine* ep, mxArray* test_array, size_t numrows, size_t numcols);
