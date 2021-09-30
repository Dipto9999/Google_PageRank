/*
 * File: matrix_multiply.h
 * Purpose: Contains Constants, Prototypes, Globals
 * Author: Muntakim Rahman
 * Date: September 9th 2021
 */

#pragma once

/***********************/
/*** Include Headers ***/
/**********************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "engine.h"

#define SIZE_BUFF 512
#define DIMENSION 3

#define TRUE 1
#define FALSE 0
#define ERROR -1

/*****************************/
/**** Function Prototypes ****/
/*****************************/

void runEngine(Engine *ep);

void printMatrix(Engine *ep, mxArray *testArray, size_t dimension);
void retrieveVariables(Engine *ep);

void retrieveError(char* message);