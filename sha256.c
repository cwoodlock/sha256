//Colm Woodlock G00341460
//Secure Hash Algorithm 256 bit version


//The usual input/output header file
#include <stdio.h>
// For using fixed bit length integers
#include <stdint.h>
//Used for error handling in file operations
#include <errno.h>
#include <string.h>

//unions store in the same memory address so in this instance we
//can access it as 64 8bit values or 16 32bit values
//or 8 64bit values this represents the message block
union msgblock {

  uint8_t e[64];
  uint32_t t[16];
  uint64_t s[8];

};

//This will define what status you are at when you are padding your message
enum status {READ, PAD0, PAD1, FINISH};

//See section 4.1.2 and 4.2.2 for definitions
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

//See section 3.2 for definitons
uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

//See Section 4.1.2 for definitions
uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

//See section 4.1.2 for definitions
uint32_t Ch(uint32_t x,uint32_t y,uint32_t z);
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z);

//Calculate the hash of a file
void sha256(FILE *msgf, uint32_t *H);

//Retrieve the next message block
int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits);

//Change from little to big endian 64
uint64_t changeEndian64(uint64_t x);

//Change from little to big endian 32
uint32_t changeEndian32(uint32_t x);

//Main method starts here
int main(int argc, char *argv[]){

  //File pointer
  FILE* msgf;

  //Decalre errnum
  int errnum;

  //Open file pointer
  msgf = fopen(argv[1], "r");
 
  //If the file pointer is null do this
  //Adapted from: https://www.youtube.com/watch?v=rMfp6gKR_kk
  if(msgf == NULL){

    //Whatever the rror number is will be assigned to errnum
    errnum = errno;

    //Print the error number value 
    fprintf(stderr, "Value of errno: %d\n", errno);

    //Perror will first show the message that the user has printed and after that will then 
    //show the internal error message will be then shown
    perror("Error printed by perror");

    ///This will print the string version of the error
    fprintf(stderr, "Error opening file: %s\n", strerror(errnum));

  } else {

    //The hash value (Section 6.2)
    //The values come from Section 5.3.3
    uint32_t H[8] = {
      0x6a09e667,
      0xbb67ae85,
      0x3c6ef372,
      0xa54ff53a,
      0x510e527f,
      0x9b05688c,
      0x1f83d9ab,
      0x5be0cd19
    };
    //Run the secure hash algorithm on the filme
    sha256(msgf, H);

    //Printing in main adapted from here: https://codeforwin.org/2017/12/pass-return-array-function-c.html 
    printf("%08x %08x %08x %08x %08x %08x %08x %08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    //Close the file
    fclose(msgf);
  }

  return 0;
}

//Function to call sha256 algorithm
void sha256(FILE *msgf, uint32_t *H){

  //Create an instance of message block
  union msgblock M;

  //Keep track and append onto the end of the message block
  uint64_t nobits = 0;

  //Create a variable for the enum and set it to read for the start
  enum status S = READ;

  //The K constants, defined in Section 4.2.2
   uint32_t K[] = {
     0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
     0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
     0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
     0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
     0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
     0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

  //Message schedule (Secion 6.2)
  uint32_t W[64];
  //Working variables  (Section 6.2)
  uint32_t a, b, c, d, e, f, g, h;
  //Two temp variable (Section 6.2)
  uint32_t T1, T2;



  //For looping
  int i, t; 

  //Loop through the message blocks per page 22
  while(nextmsgblock(msgf, &M, &S, &nobits)) {

    //From page 22 W[t] = M[t] for 0 <= t <= 15
    for(t = 0; t < 16; t++){
      W[t] = M.t[t];
    }

    //From page 22, W[t] = ...
    for(t = 16; t < 64; t++){
      W[t] =  sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
    }

    //Initialise a,b,c,d,e,f,g and h as per step 2, page 22
    a = H[0]; b = H[1]; c = H[2]; d = H[3];
    e = H[4]; f = H[5]; g = H[6]; h = H[7];

    //Step 3
    for(t = 0; t < 64; t++){
      T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
      T2 = SIG0(a) + Maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    //Step 4
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[6] = h + H[7];  

  }

  //printf("%08x %08x %08x %08x %08x %08x %08x %08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);

}


//See section 3.2 for definitons
//Rotate right
uint32_t rotr(uint32_t n, uint32_t x){
  return (x >> n) | (x << (32 - n));    
}

//Shift right
uint32_t shr(uint32_t n, uint32_t x){
  return (x >> n);
}


uint32_t sig0(uint32_t x){
  // See Sections 3.2 and 4.1.2 for definitions
  return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}


uint32_t sig1(uint32_t x){
  //See Section 3.2 and 4.1.2 for definitions
   return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}


//See Section 4.1.2 for definitions
uint32_t SIG0(uint32_t x){
  return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));
}
uint32_t SIG1(uint32_t x){
  return (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x));
}

//See section 4.1.2 for definitions
uint32_t Ch(uint32_t x,uint32_t y,uint32_t z){
  return ((x & y) ^ ((!x) & z));
}
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z){
  return ((x & y) ^ (x & z) ^ (y & z));
}


//Retrieve the next message block
int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits){

  //Store number of bytes read from fread
  uint64_t nobytes;

  //Decalre int i to be used for for loops
  int i;

  //If we have finished all the message blocks, then S should be FINISH
  if(*S == FINISH){
    return 0;
  }

  //If S equals PAD0 or PAD1 that means add a block of padding where the first 448 bits are 0s and in the last
  //8 bytes put the big endian integer representing the number of bits that were in the original message
  if( *S == PAD0 || *S == PAD1) {
    //Set the first 56 bytes to all zero bits
    for(i = 0; i < 56; i++){
      M->e[i] = 0x00;
    }
    //Set the las 64 bits to the number of bits in the file ***Should be big endian***
    M->s[7] = changeEndian32(*nobits);

    //Tell S we are finished
    *S = FINISH;

    //If S was PAD! set the first bit of M to one
    if(*S == PAD1){
      M->e[0] = 0x80;
    }
    //Keep the loop in sha256 for one more iteration
    return 1;
 }

  //If we get down here, we haven't finished reading the file S ==READ
                                    
  //Read 64 bytes from file f and store in message block M.e
  nobytes =  fread(M->e, 1, 64, msgf);
                                            
  //Anytime we read any number of bytes from the file add that number of bytes in bits onto nobits
  *nobits = *nobits + (nobytes * 8);
                                              
  //If there is room in this mesaage block to do all the padding, do this
  if(nobytes < 56) {

    //add padding to the end which is the value 10000000
    M->e[nobytes] = 0x80;
    
    //Add zero bits until the last 64 bits
    while(nobytes < 56) {
      //Add 1 onto nobytes as this will be the index into the block
      nobytes = nobytes + 1;

      //Set all of those bytes to 0 as you want to zero out everything in the 
      //block between the end of the message with the 1 at the end of it up the last 8 bytes which
      //have the 64 bits which have the integer that we need to append onto the end of
      M->e[nobytes] = 0x00;
    }

    //Set the last element to nobitsginal message ***Make sure it is a big endian int***
    M->s[7] = changeEndian32(*nobits);

    //Set S to Finish to exit loop
    *S = FINISH;

    //If I haven't read less than 56 bytes enter this, 
    //this is for when you need an extra message block full of padding occurs 56-64
    } else if (nobytes < 64){
      //Tell S we another message block, with padding but no one bit
      *S = PAD0;
      //Append one bit onto message block
      M->e[nobytes] = 0x80;

      //If there is room for some information at the end of the current message block but not all of the padding
      while (nobytes < 64){
        nobytes = nobytes + 1;
        M->e[nobytes] = 0x00;
      }
      //If finished reading the file and it happens to be 512bits or exactly the end of the file
    } else if (feof(msgf)){ 
        //Tell S that we need another message block with all the padding
        *S = PAD1;                                                                                                  
    }
      
    //If we get this far, then return 1 so that this function is called again
    return 1;

}

//Adapted from https://www.coders-hub.com/2013/04/convert-little-endian-to-big-endian-in-c.html
//Adapted from http://www.mit.edu/afs.new/sipb/project/merakidev/include/bits/byteswap.h
//Adapted from https://stackoverflow.com/questions/19275955/convert-little-endian-to-big-endian
//Adapted from https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func
//These will be used to swap from little endian to big endian
uint64_t changeEndian64(uint64_t x)
{
  x = (x & 0xffffffff00000000) >> 32 | (x & 0x00000000ffffffff) << 32;
  x = (x & 0xffff0000ffff0000) >> 16 | (x & 0x0000ffff0000ffff) << 16;
  x = (x & 0xff00ff00ff00ff00) >>  8 | (x & 0x00ff00ff00ff00ff) <<  8;
  return x;

}

uint32_t changeEndian32(uint32_t x){
  x = (x & 0xffff0000) >> 16 | (x & 0x0000ffff) << 16;
  x = (x & 0xff00ff00) >>  8 | (x & 0x00ff00ff) <<  8;
  return x;
}


