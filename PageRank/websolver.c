/*
 * File: websolver.c
 * Purpose: To Copy Data From a Matrix Provided From a Text File and
 * 			Retrieve its Dimension Size.
 * Author: Muntakim Rahman
 * Date: September 28th 2021
 */

#define _CRT_SECURE_NO_WARNINGS

/* Include Header */
#include "websolver.h"
#include "pagerank.h"

/*
 * Acquires and Returns the Web Size.
 *  Note: The Matrix is Always a Square. We Only Need to
 *        Find the Length of the Top Row.
 * PARAM: web_file is a pointer to a filestream
 * PRE: web_file is an initialized pointer to a correctly-formatted text file
 * POST: web_file's internal pointer is set to beginning of stream
 * RETURN: length of the first line of text in the web file EXCLUDING any EOL characters
 *         ('\n' or '\r') and EXCLUDING the string-terminating null ('\0') character.
 */
size_t getDimension(FILE* web_file) {
	/* Local Variables */
	size_t dimension = 0;
	char line_buffer[INPUT_BUFF];

	dimension = strlen(fgets(line_buffer, INPUT_BUFF, web_file));

	/* Reset the File's Internal Pointer
     to the Beginning of File. */
	fseek(
    	web_file, // File Pointer
    	FALSE, // Number of Bytes to Offset From Origin
    	SEEK_SET // Origin Set as Beginning of File
  	);

	return dimension / OMIT_SPACES;
}

/*
 * Parses and Stores Matrix as a 2D Array of Double.  This Requires Copying the File
 * to the Allocated Space Using the Double Pointer Called matrix.
 *************************************************************************************************
 * PARAM: web_file pointer to an open filestream
 * PARAM: dimension is a variable of type size_t representing the size of the matrix.
 * PARAM: matrix, a double pointer to type double values.
 * PRE: web_file is a pointer to a correctly-formatted matrix file.
 * PRE: dimension contains the correct size of the square matrix.
 * POST: matrix contains a dynamically allocated copy of the matrix stored in the web_file
 * RETURN: VOID
 */
void parseMatrix(FILE* web_file, size_t dimension, double** matrix) {
	/* Variables */
  	char line_buffer[INPUT_BUFF];
  	char file_char = '\0';

	size_t line_index = FALSE;

	size_t row = FALSE;
	size_t column = FALSE;

  	/* Copies Web File to Matrix Line By Line */
  	while (fgets(line_buffer, INPUT_BUFF, web_file)) {
		for (column = 0, line_index = 0; column < dimension; line_index++) {
			file_char = line_buffer[line_index];
		  	if (file_char != ' ') {
				file_char -= '0';
			  	matrix[row][column++] = (double)file_char;
		  	}
	  	}
	row++;
  	}

	return matrix;
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
void printMatrix(Engine *ep, mxArray *test_array, size_t numrows, size_t numcols) {
	if (!ep) return;

	/* Local Variables */
	size_t column = 0;
	size_t row = 0;

	fprintf(stdout, "\nMatrix Retrieved :\n\n");
	for (row = 0; row < numrows; row++) {
		/* Print Transpose of the Matrix from MATLAB Engine to Display in Row Wise Order. */
		for (column = 0; column < numcols; column++) fprintf(stdout, "%f ", *(mxGetPr(test_array) + (column * numrows) + row));
		fprintf(stdout, "\n");
	}
}
