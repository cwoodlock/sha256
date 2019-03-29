#include <stdio.h>
#include <stdint.h>

//unions store in the same memory address so in this instance we
//can access it as 64 8bit values or 16 32bit values
//or 8 64bit values
union msgblock { 
  uint8_t e[64];
  uint32_t t[16];
  uint64_t s[8];
};

//This will define what status you are at when you are padding your message
enum status {READ, PAD0, PAD1, FINISH};

int main(int argc,char *argv[]){

  //Create an instance of message block
  union msgblock M;

  //Store number of bytes read
  uint64_t nobytes;

  //Keep track and append onto the end of the message block
  uint64_t nobits = 0;

  //Create a variable for the enum and set it to read for the start
  enum status S = READ;
  
  //File pointer
  FILE* f;

  //Open file pointer
  f = fopen(argv[1], "r");

  //Decalre int i to be used for for loops
  int i;

  //While loop that loops while the status is READ
  while (S == READ){
    
    //Read 64 bytes from file f and store in message block M.e
    nobytes =  fread(M.e, 1, 64, f);

    //Print out bytes I have read for debugging
    printf("Read %2llu bytes\n", nobytes);
    
    //Anytime we read any number of bytes from the file add that number of bytes in bits onto nobits
    nobits = nobits + (nobytes * 8);
  
    //If there is room in this mesaage block to do all the padding, do this
    if(nobytes < 56) {
      printf("I've found a block with less than 55 bytes.\n");

      //add padding to the end which is the value 10000000
      M.e[nobytes] = 0x80;

      while(nobytes < 56) {
        //Add 1 onto nobytes as this will be the index into the block
        nobytes = nobytes + 1;

        //Set all of those bytes to 0 as you want to zero out everything in the 
        //block between the end of the message with the 1 at the end of it up the last 8 bytes which
        //have the 64 bits which have the integer that we need to append onto the end of
        M.e[nobytes] = 0x00;
      }
      //Set the last element to nobitsginal message ***Make sure it is a big endian int***
      M.s[7] = nobits;

      //Set S to Finish to exit loop
      S = FINISH;

      //If I haven't read less than 56 bytes enter this, 
      //this is for when you need an extra message block full of padding occurs 56-64
    } else if (nobytes < 64){
        S= PAD0;
        //Append 1 onto message block
        M.e[nobytes] = 0x80;

        //If there is room for some information at the end of the current message block but not all of the padding
        while (nobytes < 64){
          nobytes = nobytes + 1;
          M.e[nobytes] = 0x00;
        }
      //If finished reading the file and it happens to be 512bits or exactly the end of the file
    } else if (feof(f)){
        S = PAD1;
    }

  }

  //If S equals PAD0 or PAD1 that means add a block of padding where the first 448 bits are 0s and in the last
  //8 bytes put the big endian integer representing the number of bits that were in the original message
  if( S == PAD0 || S == PAD1) {
    for(i = 0; i < 56; i++){
      M.e[i] = 0x00;
    }
    M.s[7] = nobits;
  }

  if(S == PAD1){
    M.e[0] = 0x80;
  }

  fclose(f);

  for(int i = 0; i < 64; i++){
    printf("%x ", M.e[i]);
  }
  printf("\n");

  return 0;
}


