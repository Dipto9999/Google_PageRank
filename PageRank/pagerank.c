/*
 * File: pagerank.c
 * Purpose: Calculates PageRank Algorithm For
 *          Connectivity Matrix From Text File.
 * Author: Muntakim Rahman
 * Date: September 28th 2021
 */

#include "pagerank.h"

/* Static Variables */
static char buffer[SMALL_STRING_SIZE];
static char message_console[LARGE_STRING_SIZE];

/*
 * Main Function Drives the Program.
 * PARAM: NONE
 * PRE: NONE
 * POST: matrix calculations performed in MATLAB engine.
 * RETURN: TRUE IF the program runs as intended
 *          OTHERWISE FALSE
*/
int main(void) {
	int restatus;

	/* Start a MATLAB Process and Assign to a MATLAB Engine Object Pointer. */
	Engine *ep = engOpenSingleUse(NULL, NULL, &restatus);

	if (ep && restatus == 0) runEngine(ep);
	else handleError("\nCan't Start MATLAB Engine.\n");

	system("pause");
	return TRUE;
}

/*
 * Run the MATLAB Engine and Perform Matrix Calculations.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened.
 * POST: matrix calculations performed in MATLAB.
 * RETURN: TRUE IF the program runs as intended
 *          OTHERWISE FALSE
*/
void runEngine(Engine *ep) {
	if (!ep) return;

	/* Local Variables */
	size_t row = FALSE;
	size_t column = FALSE;
	size_t dimension = FALSE;

	FILE *web_file = NULL;

	/* Pointers to MATLAB Arrays. */
	mxArray *connectivity_array = NULL;
	mxArray *pagerank_result = NULL;

	/* Pointers to First mxDouble Element in MATLAB Data Array. */
	mxDouble* data_connectivity_array = NULL;

	double **connectivity_matrix = NULL;

	/* Try to Open and Parse the Web File Containing the Matrix. */
	if (fopen_s(&web_file, WEB, "r")) handleError("\nUnable To Open Web File:\n");
	else if (!web_file) handleError("\nUnable To Parse Web File:\n");

	dimension = getDimension(web_file);

	/* Allocates Memory for Correctly-Sized Matrix. */
	connectivity_matrix = (double**)calloc(dimension, sizeof(double*));
	for (row = 0; row < dimension; row++) connectivity_matrix[row] = (double*)calloc(dimension, sizeof(double));

	parseMatrix(web_file, dimension, connectivity_matrix);

  	/* Allocates Memory for Correctly-Sized Matrix. */
	connectivity_array = mxCreateDoubleMatrix(
		dimension, // Number of Rows
		dimension, // Number of Columns
		mxREAL // MATLAB Real Array
	);

	/* Store Connectivity Matrix in MATLAB Engine. */
	if (mxIsDouble(connectivity_array)) data_connectivity_array = mxGetPr(connectivity_array);
	else handleError("\nConnectivity Array Does Not Store Double-Precision, Floating Point Numbers");

	/* Initialize MATLAB Array for Connectivity Matrix. */
	for (row = 0; row < dimension; row++) {
		for (column = 0; column < dimension; column++) {
			*(data_connectivity_array + (column * dimension) + row) = connectivity_matrix[row][column];
		}
	}

	/* Error Occurs When Accessing MATLAB Engine. */
	if (engPutVariable(ep, "M", connectivity_array)) handleError("\nCannot Write Array to MATLAB.\n");
	else connectivity_array = engGetVariable(ep, "M");

	fprintf(stdout, "___________________________________\n");
	fprintf(stdout, "\nRetrieve Connectivity Matrix...\n");
	fprintf(stdout, "___________________________________\n");

	if (!connectivity_array) handleError("\nFailed to Retrieve Connectivity Matrix.\n");
	else printMatrix(ep, connectivity_array, sqrt(mxGetNumberOfElements(connectivity_array)), sqrt(mxGetNumberOfElements(connectivity_array)));

	promptPageRank(ep);

	pagerank_result = engGetVariable(ep, "x");

	if (!pagerank_result) handleError("\nFailed to Retrieve PageRank Calculation.\n");
	else printPageRank(ep, pagerank_result, mxGetNumberOfElements(pagerank_result));

	retrieveVariables(ep);

	/* Deallocate Memory for Matrix. */
	free(connectivity_matrix);

	/* Deallocate Memory For mxArrays. */
	mxDestroyArray(connectivity_array);
	mxDestroyArray(pagerank_result);

	/* Prevent Dangling Pointers. */
	connectivity_array = NULL;
	pagerank_result = NULL;

	/* Clear MATLAB Engine. */
	if (engClose(ep)) fprintf(stderr, "\nFailed to Close MATLAB Engine.\n");
}

/*
 * Print Variable Data in MATLAB Engine.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened.
 * POST: MATLAB variable information printed to screen.
 * RETURN: VOID
 */
void retrieveVariables(Engine *ep) {
	char buffer[OUTPUT_BUFF + 1];

	if (!ep) return;
	else if (engOutputBuffer(ep, buffer, OUTPUT_BUFF)) handleError("\nCan't Create Buffer For MATLAB Output.\n");

	fprintf(stdout, "____________________________\n");
	fprintf(stdout, "\nRetrieve Engine Variables...\n");
	fprintf(stdout, "____________________________\n");

	buffer[OUTPUT_BUFF] = '\0';

	fprintf(stdout, "\nEngine Variables :\n\n");
	/* Generate a List of MATLAB Variables With Types and Sizes. */
	engEvalString(ep, "whos");
	fprintf(stdout, "%s\n", buffer);
}

/*
 * Prompt the User for the PageRank Calculation Method.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened.
 * POST: user is prompted for pagerank calculation configuration
 * 		 in standard output terminal; pagerank calculations performed
 * 		 in MATLAB according to chosen calculation method.
 * RETURN: VOID
*/
void promptPageRank(Engine* ep) {
	if (!ep) return;

	int calculation_config = FALSE;

	strcpy_s(message_console, LARGE_STRING_SIZE, "\n\nEnter the Configuration Type: (");

	buffer[0] = '0' + INITIAL_APPROXIMATION;
	strncat_s(message_console, LARGE_STRING_SIZE, buffer, sizeof(char));
	strcat_s(message_console, LARGE_STRING_SIZE, " - ");

	buffer[0] = '0' + PRINCIPAL_EIGENVECTOR;
	strncat_s(message_console, LARGE_STRING_SIZE, buffer, sizeof(char));
	strcat_s(message_console, LARGE_STRING_SIZE, ") : ");

	fprintf(stdout, message_console);

	if (fgets(buffer, sizeof(char) + sizeof('\n'), stdin)) buffer[1] = '\0';
	if (sscanf_s(buffer, "%d", &calculation_config) != TRUE) handleError("\nPageRank Initialization Failed.\n");

	calculatePageRank(ep, calculation_config);
}

/*
 * Select the PageRank Method to Calculate Based on User Configuration.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PARAM: calculation_method is an int representing the configuration for
 * 		  the pagerank calculation to perform in the MATLAB engine.
 * PRE: MATLAB engine successfully opened.
 * POST: appropriate calculation method is performed in MATLAB engine.
 * RETURN: VOID
*/
void calculatePageRank(Engine *ep, int calculation_method) {
	if (!ep) return;

	else if (calculation_method == INITIAL_APPROXIMATION) calculateApproximation(ep);
	else if (calculation_method == POWER_METHOD) calculatePowerMethod(ep);
	else if (calculation_method == PRINCIPAL_EIGENVECTOR) calculatePrincipalEigenVector(ep);
	else handleError("\nInvalid PageRank Calculation.\n");

	fprintf(stdout, "_______________________________\n");
	fprintf(stdout, "\nPageRank Ready For Retrieval...\n");
	fprintf(stdout, "_______________________________\n");
}

/*
 * Calculate the Initial PageRank Approximation in the MATLAB Engine.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened; configuration is specified
 * 		for pagerank approximation.
 * POST: appropriate calculation method is performed to yield pagerank
 *       in MATLAB array x.
 * RETURN: VOID
*/
void calculateApproximation(Engine *ep) {
	if (!ep) return;

	fprintf(stdout, "\nInitial PageRank Approximation...\n");

	/* Calculate Size of Connectivity Matrix. */
	if (engEvalString(ep, "[rows, cols] = size(M);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Row and Column Sizes of Connectivity Matrix to Variables.\n");
	if (engEvalString(ep, "dim = rows;"))
		handleError("\nPAGERANK ERROR: Could Not Assign Dimension Size to Row Variable.\n");

	/* Calculate Typical Probability That Random Walk Follows a Link. */
	if (engEvalString(ep, "p = 0.85;"))
		handleError("\nPAGERANK ERROR: Could Not Initialize Typical Probability Factor of 0.85.\n");

	/* Calculate Total Number of Outgoing Links From Each Page. */
	if (engEvalString(ep, "colSums = sum(M, 1);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Sum of Connectivity Matrix Columns to a Row Vector.\n");
	if (engEvalString(ep, "numLinks = find(colSums ~= 0);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Indices of Columns With Non-Zero Sums.\n");

	/* Get Inverse Matrix of Number of Outgoing Links from Webpages. */
	if (engEvalString(ep, "D = sparse(numLinks, numLinks, 1./ colSums(numLinks), rows, cols);"))
		handleError("\nPAGERANK ERROR: Could Not Calculate Element-Wise Right Divison To Yield Inverse Vector of Column Sums.\n");

	if (engEvalString(ep, "I = speye(rows, cols);"))
		handleError("\nPAGERANK ERROR: Could Not Generate Sparse Matrix With Ones on Diagonals.\n");
	if (engEvalString(ep, "e = ones(cols, 1);"))
		handleError("\nPAGERANK ERROR: Could Not Generate a Ones Array.\n");
	/* Calculate Initial PageRank Approximation. */
	if (engEvalString(ep, "x = mldivide((I - p * M * D), e);"))
		handleError("\nPAGERANK ERROR: Could Not Calculate PageRank Approximation.\n");

	/* Normalize PageRank Vector Values. */
	if (engEvalString(ep, "x = x/ sum(x);"))
		handleError("\nPAGERANK ERROR: Could Not Normalize PageRank Vector Values.\n");
}

/*
 * Calculate the PageRank With the Power Method in the MATLAB Engine.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened; configuration is specified
 * 		for power method.
 * POST: appropriate calculation method is performed to yield pagerank
 *       in MATLAB array x.
 * RETURN: VOID
*/
void calculatePowerMethod(Engine *ep) {
	if (!ep) return;

	fprintf(stdout, "\nPower Method Calculation...\n");

	/* Calculate Size of Connectivity Matrix. */
	if (engEvalString(ep, "[rows, cols] = size(M);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Row and Column Sizes of Connectivity Matrix to Variables.\n");
	if (engEvalString(ep, "dim = rows;"))
		handleError("\nPAGERANK ERROR: Could Not Assign Dimension Size to Row Variable.\n");

	/* Calculate Typical Probability That Random Walk Follows a Link. */
	if (engEvalString(ep, "p = 0.85;"))
		handleError("\nPAGERANK ERROR: Could Not Initialize Typical Probability Factor of 0.85.\n");

	/* Calculate Total Number of Outgoing Links From Each Page. */
	if (engEvalString(ep, "colSums = sum(M, 1);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Sum of Connectivity Matrix Columns to a Row Vector.\n");
	if (engEvalString(ep, "numLinks = find(colSums ~= 0);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Indices of Columns With Non-Zero Sums.\n");

	/* Get Inverse Matrix of Number of Outgoing Links from Webpages. */
	if (engEvalString(ep, "D = sparse(numLinks, numLinks, 1./ colSums(numLinks), rows, cols);"))
		handleError("\nPAGERANK ERROR: Could Not Calculate Element-Wise Right Divison To Yield Inverse Vector of Column Sums.\n");

	/* Calculate Stochastic Matrix. */
	if (engEvalString(ep, "[zeroRows, zeroCols] = find(colSums == 0);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Rows and Columns of Columns With Zero-Sums in Connectivity Matrix.\n");
	if (engEvalString(ep, "S = M * D;"))
		handleError("\nPAGERANK ERROR: Could Not Calculate Stochastic Matrix.\n");
	/* If No Outgoing Link to a Page, Calculate Probability of Visiting Page. */
	if (engEvalString(ep, "S(:, zeroCols) = 1./ dim;"))
		handleError("\nPAGERANK ERROR: Could Not Index All Elements in Zero-Sum Columns To Yield Stochastic Matrix.\n");

	/* Calculate Transition Matrix. */
	if (engEvalString(ep, "z = ((1 - p) * (colSums ~= 0) + (colSums == 0) )/ dim;"))
		handleError("\nPAGERANK ERROR: Could Not Calculate Probability Coefficient.\n");
	if (engEvalString(ep, "e = ones(cols, 1);"))
		handleError("\nPAGERANK ERROR: Could Not Generate a Ones Array.\n");
	if (engEvalString(ep, "A = p * S + e * z;"))
		handleError("\nPAGERANK ERROR: Could Not Calculate Transition Matrix.\n");

	/* Iterate to Calculate PageRank With Power Method Until PageRank Stops Changing. */
	if (engEvalString(ep, "xPrev = zeros(cols, 1); xCurr = ones(cols, 1);"))
		handleError("\nPAGERANK ERROR: Could Not Generate a Zeros and Ones Array.\n");
	if (engEvalString(ep, "while norm(ldivide(dim, (xCurr - xPrev))) > 0.01  xPrev = xCurr; xCurr = A * xCurr; end;"))
		handleError("\nPAGERANK ERROR: Could Not Iteratively Calculate PageRank With Power Method.\n");

	/* Normalize PageRank Vector Values. */
	if (engEvalString(ep, "x = xCurr/ sum(xCurr);"))
		handleError("\nPAGERANK ERROR: Could Not Normalize PageRank Vector Values.\n");
}

/*
 * Calculate the Principal EigenVector of Connectivity Matrix in the MATLAB Engine.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PRE: MATLAB engine successfully opened; configuration is specified
 * 		for principal eigenvector method.
 * POST: appropriate calculation method is performed to yield pagerank
 *       in MATLAB array x.
 * RETURN: VOID
*/
void calculatePrincipalEigenVector(Engine *ep) {
	if (!ep) return;

	fprintf(stdout, "\nPrincipal EigenVector Calculation...\n");

	/* Calculate Size of Connectivity Matrix. */
	if (engEvalString(ep, "[rows, cols] = size(M);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Row and Column Sizes of Connectivity Matrix to Variables.\n");
	if (engEvalString(ep, "dim = rows;"))
		handleError("\nPAGERANK ERROR: Could Not Assign Dimension Size to Row Variable.\n");

	/* Calculate EigenVectors and EigenValues of Connectivity Matrix. */
	if (engEvalString(ep, "[V, D] = eig(M);"))
		handleError("\nPAGERANK ERROR: Could Not Assign EigenVectors and Right EigenValues of Connectivity Matrix to Variables.\n");

	/* D is assigned the Non-Zero Values of the Sparse Matrix. */
	if (engEvalString(ep, "D = diag(D);"))
		handleError("\nPAGERANK ERROR: Could Not Assign Non-Zero Diagonal Values of Diagonal Matrix.\n");

	/* Assign PageRank as Principal EigenVector for Real Positive EigenValue. */
	if (engEvalString(ep, "if D(1) == real(D(1)) x = V(:, 1); else x = zeros(cols, 1); end;"))
		handleError("\nPAGERANK ERROR: Could Not Iterate Through EigenValues of Connectivity Matrix.\n");

	/* Normalize PageRank Vector Values. */
	if (engEvalString(ep, "x = x / sum(x);"))
		handleError("\nPAGERANK ERROR: Could Not Normalize PageRank Vector Values.\n");
}


/*
 * Print PageRank To Standard Output.
 * PARAM: ep is an Engine pointer to a MATLAB process.
 * PARAM: pagerank is a pointer to an mxArray with the calculated values.
 * PRE: MATLAB engine successfully opened; pagerank data retrieved from engine.
 * PRE: pagerank calculation performed in MATLAB engine.
 * POST: pagerank calculation results printed to stdout.
 * RETURN: VOID
 */
void printPageRank(Engine *ep, mxArray *pagerank, size_t size_pagerank) {
	if (!ep) return;

	/* Local Variables */
	size_t page_index = 0;
	mxDouble* data_pagerank = mxGetPr(pagerank);

	fprintf(stdout, "\nPageRank Retrieved :\n\n");
	for (page_index = 0; page_index < size_pagerank; page_index++) {
		fprintf(stdout, "PAGE : %i ", page_index + 1);
		fprintf(stdout, "RANK : %.4f\n", data_pagerank[page_index]);
	}
	fprintf(stdout, "\n");
}

/*
 * Print Error Message to Standard Output and Exit Program.
 * PARAM: message is a char array to print to screen.
 * PRE: error in running MATLAB engine or retrieving data.
 * POST: error message printed to stderr.
 * RETURN: VOID
 */
void handleError(char* message) {
	fprintf(stderr, message);
	system("pause");

	exit(TRUE);
}
