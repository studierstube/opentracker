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
##  NAME:       negatepos
##  TYPE:       awk script
##  PROJECT:    Studierstube - Undistortion tools
##  CONTENT:    Negates position values of data in FSP format
## ===========================================================================
##  AUTHORS:    jp      Jan Prikryl
## ===========================================================================
##  HISTORY:
##
##  18-Jan-01 16:36:02  jp      last modification
##  07-Nov-00 18:34:01  jp      created
## ===========================================================================

{
  if (NR < 3) 
    print $0 ;
  else
    printf "%10.5f %10.5f %10.5f, " \
           "%10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n", \
           $2-100,120-$1,$3-200,-$4,-$5,-$6,$7,$8,$9,$10 ;
}

