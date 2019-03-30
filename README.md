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
## Testing
To make sure I was getting the correct value as a result from my algorithm I first went to two websites that offered SHA 256 hashing. Below was the result from each website when I hashed the padfile.c file. I chose this file as I would not be changing it so the tests could be consistent.

![Website 1](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/emnWebsite.png)
----------------------------------------------------------------------------------------------------
![Website 2](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/md5fileWebsite.png)

As you can see both website returned the same hash value for the padfile.c.

As of writing this README I have not matched my output to the expected output, this is due to the linux system being little endian   and the spec requires a big endian hash. To solve this I wrote two different functions for testing. This was both a 32 and 64 function to swap the bytes. Below are my functions

![Functions](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/functions.png)

### Test 1
In this test I used the changeEndian64 function and as you can see the output does not match the expected output.

![Test 1](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/First%20test%2064.png)

### Test 2
In this test I used the changeEndian32 function and as you can see the output does not match the expected output.

![Test 2](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/Second%20test%2032.png)

### Test 3
In this test I used the changeEndian32 function and used the changeEndian32 function on the H array as you can see the output does not match the expected output.

![Test 3](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/Third%20test%2032%20and%20H%20values.png)

### Test 4
In this test I used the changeEndian64 function and used the changeEndian64 function on the H array as you can see the output does not match the expected output.

![Test 4](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/Fourth%20test%20and%2064%20H%20values.png)

### Test 5
In this test I will test the error handling when the algorithm is called on a file that does not exist. As you can see below it displays the error.

![Test 5](https://raw.githubusercontent.com/cwoodlock/sha256/master/Images/Error%20handling.png)

## Resources
Error handling:
https://www.youtube.com/watch?v=rMfp6gKR_kk

Return array from function:
https://codeforwin.org/2017/12/pass-return-array-function-c.html

Little Endian to big Endian conversion:
https://www.coders-hub.com/2013/04/convert-little-endian-to-big-endian-in-c.html#.XJ_HdVX7RhE
http://www.mit.edu/afs.new/sipb/project/merakidev/include/bits/byteswap.h
https://stackoverflow.com/questions/19275955/convert-little-endian-to-big-endian
https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func

