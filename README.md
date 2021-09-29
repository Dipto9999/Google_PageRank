# Google Pagerank

## Contents

* [Overview](#Overview)
* [Installations](#Installations)
    * [VS-2019](#Visual-Studio-2019)
    * [MATLAB-2021a](#MATLAB-2021a)
    * [Demonstration](#Demonstration)
* [PageRank Algorithm](#PageRank-Algorithm)
    * [Markov Chain](#Markov-Chain)
    * [Connectivity Matrix](#Connectivity-Matrix)
    * [Initial Approximation](#Initial-Approximation)
    * [Power Method](#Power-Method)
        * [Transition Matrix](#Transition-Matrix)
    * [Principal EigenVector](#Principal-EigenVector)
* [Credit](#Credit)
    * [UBC Policy](#UBC-Policy)

## Overview

We are implementing a simplified version of the <b>Google PageRank</b> algorithm. This program was developed using the <a href = "https://www.mathworks.com/help/matlab/calling-matlab-engine-from-c-programs-1.html">**MATLAB Engine API for C**</a>.

## Installations

The source and header files were written to be compiled and run in the <b>Visual Studio 2019 IDE</b>. <b>MATLAB 2021a</b> was installed in order to perform matrix calculations in the <b>MATLAB Engine</b>.

### Visual Studio 2019

The [(`CPSC259_Lab5.sln`)](CPSC259_Lab5.sln) solution file can be opened in <b>Visual Studio</b>.

Viewing the <b>Configuration Properties</b> in the <b>Solution Explorer</b> reveals that the system path for (`...\MATLAB\R2021a\extern\include;%(AdditionalIncludeDirectories)`) is specified for
<b>C/C++ Additional Include Directories</b>.

Similarly, we specified the <b>Linker Additional Library Directories</b> to include (`...\MATLAB\R2021a\extern\lib\win64\microsoft;%(AdditionalLibraryDirectories)`).

The <b>Debugging Configuration</b> has the <b>Environment</b> set to (`PATH=...\MATLAB\R2021a\bin\win64\$(LocalDebuggerEnvironment)`)

The <b>Configuration Window</b> is shown below.

<p align="center">
    <img src="Figures/VS_Configuration_Properties.JPG" width="40%" height="40%" title="Configuration Properties Window." >
</p>

### MATLAB 2021a

We install the latest version of the <b>MATLAB</b> Computing Platform in order to run the <b>MATLAB Engine</b> in our **C** program. We make use of the <b>MATLAB</b> commands in our [(`pagerank.c`)](TakeHome/pagerank.c) source file in order to calculate the <b>PageRank Algorithm</b>.

This allows us to ensure the intended performance of our **C** program, we test the <b>PageRank Algorithm</b> in the <b>MATLAB Command Window</b>.

### Demonstration

The video in the [`Demonstration`](Demonstration) directory shows the <b>Conventional Power Method</b> calculation when building the project on <b>Visual Studio</b>. I have embedded a low resolution compressed version below.

https://user-images.githubusercontent.com/52113009/130340062-9a8ce61d-4a7d-486b-936f-363cfbcd9d2f.mp4

## PageRank Algorithm

We have designed a simplified version of <b><a href = "https://en.wikipedia.org/wiki/Google">Google</a></b>'s <b>PageRank Algorithm</b>. This was developed by <a href = "https://en.wikipedia.org/wiki/Larry_Page">Larry Page</a> as a means of identifying the relevant webpages on the Internet.

### Markov Chain

The <b>PageRank</b> theory is based on the assumption that a hypothetical web surfer randomly clicks on links to jump between webpages. This theoretical random walk is referred to as a <b>Markov Chain</b>.

If a "high ranking" webpage <i>j</i> contains an outgoing hyperlink to another page <i>i</i>, its <b>PageRank</b>, is equal to the score of page <i>i</i> divided by the number of outbound links.

We have mathematically accounted for the probability that the random walk will follow the hyperlinks through a variable <i>p</i>. This is pivotal to calculate the <b>PageRank</b> through <b>Power Method</b> in the <b>MATLAB Engine</b>.

### Connectivity Matrix

The connectivity matrix for the <b>PageRank</b> algorithm is parsed from the [(`web.txt`)](TakeHome/web.txt) file. The input contains a number of lines of binary values, where outgoing links from webpages are arranged in column-wise fashion.

In Essence :
    *1* values indicate the presence of an outgoing link from page <i>j</i> to page <i>i</i>.</br>
    *0* values indicate the absence of an outgoing link from page <i>j</i> to page <i>i</i>.</br>

We perform the parsing functionality in the [(`websolver.c`)](TakeHome/websolver.c) source file.

### Initial Approximation

We calculate the <b>PageRank</b> approximation by running the <b>MATLAB</b> command `x = mldivide((I - p * M * D), e);`.

Considering the <b>MATLAB</b> Variables :
    `I` represents the identity matrix.</br>
    `p` represents the probability of following a hyperlink.</br>
    `M` represents the connectivity matrix.</br>
    `D` represents the sparse matrix with the probabilities of following an individual link.</br>
    `e` represents a 6x1 ones array which we call `mldivide` on to left divide the matrix result of `(I - p * M * D)`.</br>

In mathematical representation, we are solving for the variable <i>x</i> where <i>(I - p * M * D) * x = e</i>.

After normalizing x with the <b>MATLAB</b> command `x = x/ sum(x);`, this yields the <b>Initial PageRank</b> which approximately calculates the probability of a random web surfer accessing each of the webpages.

Here is an excerpt of the <b>MATLAB Command Window</b> when performing the initial approximation.

<p align="center">
    <img src="Figures/Approximation.JPG" width="40%" height="40%" title="Appro Calculation." >
</p>

 The entire output can be view in the [(`Approximation.pdf`)](Figures/Approximation.pdf) file.

### Power Method

In the <b>Power Method</b> we also account for the probability <i>1-p</i> that the web surfer comes across the webpage without accessing a hyperlink. Even though our model is composed of a finite number of webpages, <i>n</i> (i.e. <i>n = 6</i>), this is the practical approach to calculating the <b>PageRank</b> for a very large number of pages (i.e. <i>n >> 6</i>)

This involves multiplying a <b>Transition Matrix</b> by the <b>PageRank</b>. We have represented the <b>Transition Matrix</b> by the <b>MATLAB</b> array `A`.

### Transition Matrix

The <b>Transition Matrix</b> is calculated in the <b>MATLAB Engine</b> through the command :

` A = p * M * D + e * (((1 - p) * (colSums ~= 0) + (colSums == 0) )/ dim); `

Considering the <b>MATLAB</b> Variables :
    `p` represents the probability of following a hyperlink.</br>
    `M` represents the connectivity matrix.</br>
    `D` represents the sparse matrix with the probabilities of following an individual link.</br>
    `e` represents a 6X1 ones array which we use to calculate the probability of randomly choosing a webpage.</br>
    `colSums` represents the total number of outgoing links from each webpage.</br>
    `dim` represents the row-size and column-size of the square connectivity matrix.</br>

The <b>Power Method</b> accounts for the <b>Markov Chain</b> in the calculation by including the probability of choosing a random webpage. We iteratively perform this multiplication until the <b>PageRank</b> stops changing to yield the limiting probability that an infinitely dedicated web surfer accesses a given webpage.

In order to do this, we execute the <b>MATLAB</b> command :
` while norm(ldivide(dim, (xCurr - xPrev))) > 0.01  xPrev = xCurr; xCurr = A * xCurr; end; `

We keep track of the current and previous <b>PageRank</b> in order to perform the necessary number of <b>PageRank</b> iterations. Here is an excerpt of the <b>MATLAB Command Window</b> when performing the <b>Power Method</b>.

<p align="center">
    <img src="Figures/Power_Method.JPG" width="40%" height="40%" title="Power Method Calculation." >
</p>

 The entire output can be view in the [(`Power_Method.pdf`)](Figures/Power_Method.pdf) file.

### Principal EigenVector

Repeatedly multiplying the <b>Transition Matrix</b> by the <b>PageRank</b> can be mathematically represented as :

` A * x = A * A * x = A * A * A * x = lambda * x = x' `

This is the mathematical formula for an eigenvector. From this, we deduce that the <b>PageRank Algorithm</b> as calculated by the <b>Power Method</b> represents a system that evolves step by step. The <b>Transition Matrix</b> pulls the <b>PageRank</b> towards the principal eigenvector.

This is best viewed in this visual from the <a href = "https://setosa.io/ev/eigenvectors-and-eigenvalues/">Eigenvectors and Eigenvalues</a> web post.

<p align="center">
    <img src="Figures/Eigenvector_Visual.JPG" width="40%" height="40%" title="Eigenvector." >
</p>

To evaluate this, we calculate the prinicpal eigenvector in our <b>MATLAB Engine</b>. Here is an excerpt of the <b>MATLAB Command Window</b> when calculating the principal eigenvector.

<p align="center">
    <img src="Figures/Principal_Eigenvector.JPG" width="40%" height="40%" title="Principal Eigenvector Calculation." >
</p>

The entire output can be view in the [(`Principal_Eigenvector.pdf`)](Figures/Principal_Eigenvector.pdf) file.

## Credit

This was originally completed as a final project for <b>CPSC 259 - Data Structures and Algorithms for Electrical Engineers</b> course at the <b>University of British Columbia</b>. Unlike the other labs for this course, this program was created entirely from scratch.

However, this project was heavily modified to deviate from the original submission and to respect course policies. In its current form, it is largely a personal exploration of the **C** language features as well as the mathematics of the <b>PageRank Algorithm</b>.

The <b>MATLAB</b> commands executed to perform the <b>PageRank</b> calculations were heavily inspired by the <b>CPSC 259</b> course instructions, as well as from the following links:

<ul>
    <li>[MATHWORKS PageRank Document](https://www.mathworks.com/content/dam/mathworks/mathworks-dot-com/moler/exm/chapters/pagerank.pdf)</li>
    <li>[Samuli Siltanen Youtube Demonstration](https://www.youtube.com/watch?v=J_q2XjWyyGI&ab_channel=SamuliSiltanen)</li>
</ul>

The mathematical rationality behind eigenvectors powering the <b>PageRank</b> algorithm was further explored from the following links:

<ul>
    <li>[Dhruv On Math PageRank](https://www.dhruvonmath.com/2019/03/20/pagerank/)</li>
    <li>[Eigenvectors and Eigenvalues](https://setosa.io/ev/eigenvectors-and-eigenvalues/)</li>
    <li>[PageRank Wikipedia](https://en.wikipedia.org/wiki/PageRank#Simplified_algorithm)</li>
</ul>

### UBC Policy

We have read and understood the plagiarism policies at <a href = "http://www.cs.ubc.ca/~tmm/courses/cheat.html">http://www.cs.ubc.ca/~tmm/courses/cheat.html</a> and we understand that no excuse for plagiarism will be accepted.