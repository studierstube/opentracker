#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

main (int argc, char* argv[]) {

  char *inName;
  FILE *infptr;
  char *outName;
  FILE *outfptr;
  int s;
  long  header;
  char b,b1,b2,b3,b4,b5,b6;
  int  sizeX, sizeY, sizeZ;

  if (argc == 0) {
     printf ("usage: seek <header> <infilename> <outfilename>\n");
     return 1;
  } 
  
  header = atol(argv[1]);
  inName = argv[2];
  outName = argv[3];
 
  printf ("inName        : <%s>\n", inName);
  printf ("outName       : <%s>\n", outName);
  printf ("header        : <%d>\n", header);

  infptr = fopen(inName, "rb"); 
  outfptr = fopen(outName, "wb"); 

  fgets(&b,5,infptr);

  fgets(&b1,2,infptr);
  fgets(&b2,2,infptr);
  sizeX = (b2 << 8) + b1; 

  fgets(&b3,2,infptr);
  fgets(&b4,2,infptr);
  sizeY = (b4 << 8) + b3; 

  fgets(&b5,2,infptr);
  fgets(&b6,2,infptr);
  sizeZ = (b6 << 8) + b5; 

  printf ("volume size X : <%d>\n",sizeX);
  printf ("volume size Y : <%d>\n",sizeY);
  printf ("volume size Z : <%d>\n",sizeZ);

  // copy all values behind header into outfile!
  if (fseek (infptr,header+1,SEEK_SET) != 0)  {
     printf("ERROR: fseek error\n");     
     return 1;
  }
  
  while (1==1) {
    s = fgetc(infptr);
    fputc(s, outfptr);
    if (s == EOF ) return 0 ;
  }


}
