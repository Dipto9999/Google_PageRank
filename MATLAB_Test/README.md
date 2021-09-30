# Matrix Multiplication : MATLAB Engine for C

## Contents

* [Overview](#Overview)
* [MATLAB Engine API](#MATLAB-Engine-API)
    * [Datatypes](#Datatype)
        * [Engine](#Engine)
        * [Arrays](#Arrays)
            * [Elements](#Elements)
    * [Commands](#Commands)
        * [Display](#Display)
        * [Variables](#Variables)
    * [Memory Management](#Memory-Management)
* [Demonstration](#Demonstration)
* [Credit](#Credit)
    * [Policy](#Policy)

## Overview

I have developed a **C** program to multiply two matrices in the **MATLAB Engine**.

## MATLAB Engine API

The matrices data is initialized in the [(`matrix_multiply.c`)](matrix_multiply.c) source file. After processing in the **MATLAB Engine**, it is received by the **C** program and printed to the standard output terminal.

### Datatypes

The **MATLAB Engine API for C** includes built-in datatypes which provides access to the **MATLAB Process** and the array variables.

#### Engine

The `Engine*` pointer datatype is used to start the **MATLAB Process** with the following syntax:</br>

```c
Engine *ep = engOpen(NULL);
```

For our purposes, we have implemented functions with sanity checks to ensure that our `Engine*` pointer variable `ep` is not *NULL*. To do this, we include the line `if (!ep) return;`.

`ep` is also passed as a parameter in the other **API** function calls. This provides us access to our **MATLAB Engine** variables.

After completing our matrix calculations, we shut down the **MATLAB Process** by calling `engClose(ep)`. If this goes as intended, the function should return *NULL*.

#### Arrays

The `mxArray*` pointer datatype is used to provide access to the **MATLAB** array. We allocate memory for the array in **MATLAB** with the following syntax :</br>

```c
mxArray* array_one = mxCreateDoubleMatrix(
                        DIMENSION, // Number of Rows
                        DIMENSION, // Number of Columns
                        mxREAL // MATLAB Real Array
                      );
```

We also use it to provide sanity checks that the **MATLAB** arrays store data as double-precision, floating-point numbers, with the line :</br>
`if (mxIsDouble(array_one)) ...`

#### Elements

The `mxDouble*` pointer datatype is used to provide access to the first `mxDouble` element in the **MATLAB** data arrays. Considering our previous example, this is accomplished with the syntax: `mxDouble* data_array_one = mxGetPr(array_one);`</br>

This provides the ability to initialize the data array in the **C** program. We iterate through the data array elements, as shown below:</br>

```c
for (row = 0; row < DIMENSION; row++) {
	for (column = 0; column < DIMENSION; column++) {
		*(data_array_one + (column * DIMENSION) + row) = ((double)(DIMENSION) * row) + column + 1.0;
        ...
        ...
    }
}
```

Since <b>MATLAB</b> stores its two-dimensional array data in a column-wise rather than row-wise arrangement, we must account for this in our data initialization.

### Commands

After, we initialize the **MATLAB** data arrays in our **C** program,
we put variables in the **MATLAB Engine** with the line: </br>
`engPutVariable(ep, "arrayOne", array_one);`.

Assuming, we have another similarly defined and initialized **MATLAB** array with the variable name `arrayTwo`, we can perform matrix multiplication by executing **MATLAB** commands from **C** program and then receive the results. This procedure is shown below:</br>

```c
engEvalString(ep, "arrayProduct = arrayOne * arrayTwo");
array_product = engGetVariable(ep, "arrayProduct");
```

Note that we cannot pass the `\` character in the parameter for the **MATLAB** command, when calling the `engEvalString(...)` function.

In our **PageRank Algorithm**, we must execute the **MATLAB** command to iteratively calculate the <b>Power Method</b>. To do this, it must be written in code as :</br>

```c
while norm(ldivide(dim, (xCurr - xPrev))) > 0.01  xPrev = xCurr; xCurr = A * xCurr; end;
```

Our **C** program won't execute the following command in the **MATLAB Engine**.

```c
while norm((xCurr - xPrev).\ dim) > 0.01  xPrev = xCurr; xCurr = A * xCurr; end;
```

### Variables

The `whos` **MATLAB** command allows us to generate a list of the variables along with their sizes and types. This requires specifying an output buffer to receive the variable data. Afterwards, we can print the buffer contents with the following **C** syntax.

```c
engOutputBuffer(ep, buffer, SIZE_BUFF);
...
engEvalString(ep, "whos");
...
printf("%s\n", buffer);
```

### Display

We wish to display the array data retrieved from the **MATLAB Engine** in a row-wise arrangement. In order to do this, we print the matrix transpose instead, as shown below:</br>

```c
for (row = 0; row < dimension; row++) {
	for (column = 0; column < dimension; column++) {
        printf("%f ", *(mxGetPr(arrayProduct) + (column * dimension) + row));
    }
	printf("\n");
}
```

## Memory Management

After we've concluded our work with the **MATLAB** arrays, we deallocate the memory for the `mxArrays` in **MATLAB**.
We must also assign the `mxArray*` and `mxDouble*` pointers to *NULL*, with the following lines of code:</br>

```c
mxDestroyArray(array_one);
array_one = NULL;
data_array_one = NULL;
```

This prevents dynamic memory issues (e.g. danging pointers).

## Demonstration

A video in the [`Demonstrations`](Demonstrations) directory shows the matrix multiplication calculation when running the program on <b>Visual Studio</b>. I have embedded a low resolution compressed version below.

https://user-images.githubusercontent.com/52113009/135350441-df83eab4-6f10-44e2-b53d-90ce41e05b6e.mp4

## Credit

This was originally completed as a final project for <b>CPSC 259 - Data Structures and Algorithms for Electrical Engineers</b>, which is a course at the <b>University of British Columbia</b>. Unlike the other labs for this course, this program was created entirely from scratch.

### Policy

I have read and understood the plagiarism policies at <a href = "http://www.cs.ubc.ca/~tmm/courses/cheat.html">http://www.cs.ubc.ca/~tmm/courses/cheat.html</a> and I understand that no excuse for plagiarism will be accepted.
