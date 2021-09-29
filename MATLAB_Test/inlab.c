/*
 * File: inlab.c
 * Purpose: Explore Usage of MATLAB Engine to Multiply Matrices.
 * Author: Muntakim Rahman
 * Date: September 9th 2021
 */

#define _CRT_SECURE_NO_WARNINGS

#include "inlab.h"

/*
 * Main Function Drives the Program.
 * PARAM: NONE
 * PRE: NONE
 * POST: matrix calculations performed in MATLAB engine.
 * RETURN: TRUE IF the program runs as intended
 *          OTHERWISE FALSE
 */
int main(void) {
	/* Start a MATLAB Process and Assign to a MATLAB Engine Object Pointer. */
	Engine *ep = engOpen(NULL);

	if (ep) runEngine(ep);
	else retrieveError("\nCan't Start MATLAB Engine.\n");

	system("pause");
	return TRUE;
}

/*
 * Run the MATLAB Engine and Perform Matrix Calculations.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened.
 * POST: matrix calculations performed in MATLAB.
 * RETURN: VOID
 */
void runEngine(Engine *ep) {
	/* Local Variables */

	/* Pointers to MATLAB Data Arrays. */
	mxArray *array_one = NULL;
	mxArray *array_two = NULL;
	mxArray *array_product = NULL;

	/* Pointers to First mxDouble Element in MATLAB Data Arrays. */
	mxDouble* data_array_one = NULL;
	mxDouble* data_array_two = NULL;

	size_t row = FALSE;
	size_t column = FALSE;

  	/* Allocates Memory for Correctly-Sized Matrices. */
	array_one = mxCreateDoubleMatrix(
		DIMENSION, // Number of Rows
		DIMENSION, // Number of Columns
		mxREAL // MATLAB Real Array
	);

	array_two = mxCreateDoubleMatrix(
		DIMENSION, // Number of Rows
		DIMENSION, // Number of Columns
		mxREAL // MATLAB Real Array
	);

	if (mxIsDouble(array_one)) data_array_one = mxGetPr(array_one);
	else retrieveError("\nArray One Does Not Store Double-Precision, Floating Point Numbers");

	if (mxIsDouble(array_two)) data_array_two = mxGetPr(array_two);
	else retrieveError("\nArray Two Does Not Store Double-Precision, Floating Point Numbers");

	/* Copy Matrices to Test Arrays in MATLAB Engine. */
	/* Note that MATLAB Stores Data Column-Wise Rather Than Row-Wise. */
	for (row = 0; row < DIMENSION; row++) {
		for (column = 0; column < DIMENSION; column++) {
			*(data_array_one + (column * DIMENSION) + row) = ((double)(DIMENSION) * row) + column + 1.0;
			*(data_array_two + (column * DIMENSION) + row) = (double)(DIMENSION) * ((double)(DIMENSION) - row) - column;
		}
	}

	/* Error Occurs When Accessing MATLAB Engine. */
	if (engPutVariable(ep, "arrayOne", array_one)) retrieveError("\nCannot Write Array One to MATLAB.\n");
	if (engPutVariable(ep, "arrayTwo", array_two)) retrieveError("\nCannot Write Array Two to MATLAB.\n");

	if (engEvalString(ep, "arrayProduct = arrayOne * arrayTwo")) retrieveError("\nError Multiplying Matrices.\n");

	array_one = engGetVariable(ep, "arrayOne");
	array_two = engGetVariable(ep, "arrayTwo");
	array_product = engGetVariable(ep, "arrayProduct");

	printf("\n****Retrieve First Matrix****\n");
	if (!array_one) retrieveError("\nFailed to Retrieve First Matrix.\n");
	else printMatrix(ep, array_one, sqrt(mxGetNumberOfElements(array_one)));

	printf("\n****Retrieve Second Matrix****\n");
	if (!array_two) retrieveError("\nFailed to Retrieve Second Matrix.\n");
	else printMatrix(ep, array_two, sqrt(mxGetNumberOfElements(array_two)));

	printf("\n****Retrieve Matrix Product****\n");
	if (!array_product) retrieveError("\nFailed to Retrieve Matrix Product.\n");
	else printMatrix(ep, array_product, sqrt(mxGetNumberOfElements(array_product)));

	retrieveVariables(ep);

	/* Deallocate Memory For mxArrays. */
	mxDestroyArray(array_one);
	mxDestroyArray(array_two);
	mxDestroyArray(array_product);

	/* Prevent Dangling Pointers. */
	array_one = NULL;
	array_two = NULL;
	array_product = NULL;

	data_array_one = NULL;
	data_array_two = NULL;

	/* Done Working With MATLAB Engine. */
	if (engClose(ep)) fprintf(stderr, "\nFailed to Close MATLAB Engine.\n");
}

/*
 * Print Matrix Values For MATLAB Array.
 * PARAM: ep is an Engine pointer to a MATLAB process;
 * 		  test_array is a pointer to a MATLAB data array;
 * 		  dimension is a size_t representing the row-size and
 * 		  column-size of the MATLAB array.
 * PRE: MATLAB engine successfully opened and array exists in process.
 * POST: values printed to screen in row-wise order.
 * RETURN: VOID
 */
void printMatrix(Engine *ep, mxArray *test_array, size_t dimension) {
	/* Local Variables */
	size_t column = 0;
	size_t row = 0;

	if (!ep) return;

	printf("\nMatrix Retrieved :\n\n");
	for (row = 0; row < dimension; row++) {
		/* Print Transpose of the Matrix from MATLAB Engine to Display in Row Wise Order. */
		for (column = 0; column < dimension; column++) printf("%f ", *(mxGetPr(test_array) + (column * dimension) + row));
		printf("\n");
	}
}

/*
 * Print Variable Data in MATLAB Process.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened.
 * POST: MATLAB variable information printed to screen.
 * RETURN: VOID
 */
void retrieveVariables(Engine *ep) {
	char buffer[SIZE_BUFF + 1];

	if (!ep) return;
	else if (engOutputBuffer(ep, buffer, SIZE_BUFF)) retrieveError("\nCan't Create Buffer For MATLAB Output.\n");

	printf("\n****Retrieve Engine Variables****\n");

	buffer[SIZE_BUFF] = '\0';

	printf("\nEngine Variables :\n\n");
	/* Generate a List of MATLAB Variables With Sizes and Types. */
	engEvalString(ep, "whos");
	printf("%s\n", buffer);
}

/*
 * Print Error Message to Screen and Exit Program.
 * PARAM: message is a char array to print to screen.
 * PRE: error in running MATLAB engine or retrieving data.
 * POST: error message printed to stderr.
 * RETURN: VOID
 */
void retrieveError(char* message) {
	fprintf(stderr, message);
	system("pause");

	exit(TRUE);
}