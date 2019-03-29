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

int main(int argc,char *argv[]){

  //Create an instance of message block
  union msgblock M;

  //Store number of bytes read
  uint64_t nobytes;

  //Keep track and append onto the end of the message block
  uint64_t nobits = 0;
  
  //File pointer
  FILE* f;

  //Open file pointer
  f = fopen(argv[1], "r");

  //While loop that loops until it reaches the end of file
  while (!feof(f)){
    
    //Read 64 bytes from file f and store in M.e
    nobytes =  fread(M.e, 1, 64, f);
    
    //Anytime we read any number of bytes from the file add that nmber of bytes in bits onto nobits
    nobits = nobits + (nobytes * 8);
  
    //If there is room in this mesaage bliock to do all the padding do this
    if(nobytes < 56) {
      printf("I've found a block with less than 55 bytes.\n");

      //add padding to the end which is 10000000
      M.e[nobytes] = 0x80;

      while(nobytes < 56) {
        //Add 1 onto nobytes as this will be the index into the block
        nobytes = nobytes + 1;

        //Set all of those bytes to 0 as you want to zero out everything in the 
        //block between the end of the message with the 1 at the end of it up the last 8 bytes which
        //have the 64 bits which have the integer that we need to append onto the end of
        M.e[nobytes] = 0x00;
      }
      //Set the last element to nobits
      M.s[7] = nobits;
    }

  }

  fclose(f);

  for(int i = 0; i < 64; i++){
    printf("%x ", M.e[i]);
  }
  printf("\n");

  return 0;
}


