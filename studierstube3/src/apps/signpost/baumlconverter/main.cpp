/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** BAUML Converter
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: main.cpp 3427 2004-07-14 12:20:49Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stdlib.h>
#include <stdio.h>

#include "bauml.h"

void makeiv(ARBuilding *building, const char *filename);
void makeot(ARBuilding *building, const char *outputfilename, const char *templatefilename);

int main(int argc, char *argv[])
{
    if ((argc >= 5) && (argc <= 6))
    {
        const char *input_type = argv[1];
        const char *input_file = argv[2];
        const char *output_file_iv = argv[3];
        const char *output_file_ot = argv[4];
        const char *template_file_ot = argv[5];

        BAUML bauml;
        if (input_type[0] == 'b') bauml.read_bauml(input_file);
        if (input_type[0] == 'o') bauml.read_ocar(input_file);
        makeot(bauml.getBuilding(), output_file_ot, (argc >= 6) ? template_file_ot : NULL);
        makeiv(bauml.getBuilding(), output_file_iv); // important: after makeot(), because makeiv() makes modifactions needed by makeiv()
    }
    else
    {
        printf("Usage: %s input-type input-file output-file-iv output-file-ot [template-file-ot]\n", argv[0]);
        printf("\n");
        printf("input-type = 'o' for ocar, 'b' for bauml\n");
    }


    return 0;
}