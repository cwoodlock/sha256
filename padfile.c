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
  
  //File pointer
  FILE* f;

  //Open file pointer
  f = fopen(argv[1], "r");

  //While loop that loops until it reaches the end of file
  while (!feof(f)){
    
    //Read 64 bytes from file f and store in M.e
    nobytes =  fread(M.e, 1, 64, f);
    //prints the number of bytes that have been read, llu stands for long long unsigned
    printf("%llu\n", nobytes);

  }

  fclose(f);

  return 0;
}


