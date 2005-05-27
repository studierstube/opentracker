//fsp2lut.cxx  - convert .fsp (full sample) files to .lut (lookup table) files
//(C) 1999 Dieter Schmalstieg

#include <stdio.h>
#include <stdlib.h>
#include <stbapi/util/option.h>

void
readFile(const char* fileName, int noX, int noY, int noZ)
{
//--variables and inits
    FILE* fi = fopen(fileName,"r");
    int i,j,k;
    float x,y,z,a,b,c,d;
    int dummy, res;
    int size = noX * noY * noZ;
    char dummyBuffer[1000];

//--main loop
    printf("%d\n",size);
    for(j=0; j<noX; j++)
    {
        for(i=0; i<noY; i++)
        {
            fscanf(fi,"%s\n",dummyBuffer); //skip measure set heading
            fscanf(fi,"%s\n",dummyBuffer); //skip measure set heading
            for(k=0; k<noZ; k++)
            {
                res = fscanf(fi,"%d: X=%f Y=%f Z=%f, A=%f B=%f C=%f D=%f\n",
                    &dummy,&x,&y,&z,&a,&b,&c,&d);
                if(!res) { printf("wrong format\n"); exit(-1); }
            }
        }
    }
    fclose(fi);
}

void
main(int argc, char* argv[])
{
//--paramters
    OptionString optFileName("file","f","data/all.raw");
    OptionLong   optNoX("noX","nx",10);
    OptionLong   optNoY("noY","ny",10);
    OptionLong   optNoZ("noZ","nz",10);
    OptionDouble optSpacingX("spacingX","sx",1.0);
    OptionDouble optSpacingY("spacingY","sy",1.0);
    OptionDouble optSpacingZ("spacingZ","sz",1.0);

    if(!Option::parse(argc,argv)) { printf("syntax error\n"); exit(-1); }
    const char* fileName = optFileName.getValue();
    int   noX      = optNoX.getValue();
    int   noY      = optNoY.getValue();
    int   noZ      = optNoZ.getValue();
    float spacingX = (float) optSpacingX.getValue();
    float spacingY = (float) optSpacingY.getValue();
    float spacingZ = (float) optSpacingZ.getValue();

    readFile(fileName, noX, noY, noZ);

}
