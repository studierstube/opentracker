#!/usr/bin/awk
## ===========================================================================
##  Copyright (c) 1996,1997,1998,1999,2000
##  Vienna University of Technology
##  
##  This software is provided AS IS, without any express or implied
##  warranty.  In no event will the authors or the Vienna University of
##  Technology be held liable for any damages or loss of profit arising
##  from the use or non-fitness for a particular purpose of this software.
## ===========================================================================
##  NAME:       interpolate
##  TYPE:       awk script
##  PROJECT:    Studierstube - Undistortion tools
##  CONTENT:    Linear interpolator of two raw data lines
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  11-Jan-01 15:04:53  jp      last modification
##  07-Nov-00 18:34:01  jp      created
## ===========================================================================
BEGIN {
  FS = "=";
  a  = 0.0;
  b  = 0.0;
  c  = 0.0;
  d  = 0.0;
  e  = 0.0;
  f  = 0.0;
  g  = 0.0;
}
{
  a += $2;
  b += $3;
  c += $4;
  d += $5;
  e += $6;
  f += $7;
  g += $8;
}
END {
  printf "Records: %d\n",NR;
  printf "X=%9.5f Y=%9.5f Z=%9.5f, A=%9.5f B=%9.5f C=%9.5f D=%9.5f\n",
         a/NR,b/NR,c/NR,d/NR,e/NR,f/NR,g/NR;
  printf "%9.5f %9.5f %9.5f %9.5f %9.5f %9.5f %9.5f\n",
         a/NR,b/NR,c/NR,d/NR,e/NR,f/NR,g/NR;
}
