/*
 * File: pagerank.h
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
#include <math.h>

#include "websolver.h"

/***************************/
/* Preprocessor Directives */
/***************************/

#define SMALL_STRING_SIZE 10
#define LARGE_STRING_SIZE 100

/***********************/
/**** Enumerations ****/
/***********************/

enum PAGERANK_METHODS {
    INITIAL_APPROXIMATION = 1,
    POWER_METHOD = 2,
    PRINCIPAL_EIGENVECTOR = 3
};

/***************************/
/**** Declare Variables ****/
/***************************/

extern char buffer[];
extern char message_console[];

/***********************/
/* Function Prototypes */
/***********************/

void runEngine(Engine *ep);
void retrieveVariables(Engine* ep);

void promptPageRank(Engine* ep);
void calculatePageRank(Engine *ep, int calculation_method);

/* MATLAB Calculations */
void calculatePrincipalEigenVector(Engine *ep);
void calculateApproximation(Engine *ep);
void calculatePowerMethod(Engine *ep);

void printPageRank(Engine *ep, mxArray *pagerank, size_t size_pagerank);

void handleError(char* message);
