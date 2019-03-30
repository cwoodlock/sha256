# Implementation of the SHA 256 Secure Hash Algorithm
Colm Woodlock G00341460

## Problem Statement
You must write a program in the C programming language that calculates
the SHA-256 checksum of an input. The algorithm is specified in the Secure
Hash Standard document supplied by the (United States) National Institute
of Standards and Technology. The only pre-requisite is that your program
performs the algorithm — you are free to decide what input the algorithm
should be performed on. I suggest you allow the user to input some free text
or a filename via the command line.

## What is the SHA 256 Secure Hash Algorithm
The SHA (Secure Hash Algorithm) is one of a number of cryptographic hash functions. A cryptographic hash is like a signature for a text or a data file. SHA-256 algorithm generates an almost-unique, fixed size 256-bit (32-byte) hash. Hash is a one way function – it cannot be decrypted back. This makes it suitable for password validation, challenge hash authentication, anti-tamper, digital signatures.

## How to run my program
To run my implementation of this algorithm you must first clone this repo. Navigate to your desired workspace then using the following command clone the repository.
```
git clone https://github.com/cwoodlock/sha256.git
```
Once you have cloned the repository you are can now compile the file by running the following command.

```
gcc -o sha256 sha256.c
```
Assuming the file compiles with no errors you can now run the file. To run the file use the following code and make sure to include the file you wish to hash in the command. In the example below I am hashing the padfile.c file.

```
./sha256 padfile.c
```
