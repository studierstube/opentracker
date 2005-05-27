/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** This file contains a facade class for the IMGR library.
  * It hides internals like extensive use of templates from the user.
  * The .cxx file also contains a structure with the variables needed for
  * gesture processing.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: IMGR.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include "IMGR.h"

#include <stbapi/misc/GRT_defs.h>

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#endif


#ifdef GRT_IMGR  // else: compiles without IMGR library

// includes for the IMGR library:
#define ALL_INLINE  // needed by the IMGR library
#include "../../../extras/IMGR/feature_list_pool.h"
#include "../../../extras/IMGR/feature_space.h"
#include "../../../extras/IMGR/property_list_pool.h"
#include "../../../extras/IMGR/recognizer_pool.h"
#include "../../../extras/IMGR/sample_list_pool.h"
#include "../../../extras/IMGR/sample_pool.h"
#undef ALL_INLINE


/** This structure conceals the variables needed for gesture processing.
  * Please see the manual of the IMGR libaray for help on those.
  * Inside the Studierstube, don't use this! Include IMGR.h!
  */
struct IMGR_var
{
    DEFINED_SAMPLE_TYPE s;
    DEFINED_PROPERTY_LIST_TYPE pl;
    DEFINED_FEATURE_TYPE f;
    DEFINED_FEATURE_LIST_TYPE fl;
    feature_space<DEFINED_PROPERTY_LIST_TYPE, DEFINED_FEATURE_TYPE,
        DEFINED_FEATURE_LIST_TYPE> fs;
    DEFINED_RECOGNIZER_TYPE* r;
    float factor;
};


/// Constructor.

IMGR::IMGR()
{
    var= new IMGR_var;
    var->r= new DEFINED_RECOGNIZER_TYPE(0);
    var->factor= 1.0f;
}


/// Destructor.

IMGR::~IMGR() 
{
    delete var;
}


/// Resets the gesture library.

void IMGR::libNew()
{
    var->fs.init();  // WARNING: in this version of IMGR not implemented!!!
}


/// Loads a gesture library file.

bool IMGR::libLoad(const char* lib)
{
    return var->r->load(lib);
}


/// Saves the current recognition data into a gesture library file.

bool IMGR::libSave(const char* lib)
{
    return var->r->save(lib);
}


/// Starts recognition of a new gesture.

void IMGR::gstNew(float x, float y, float factor)
{
    var->factor= factor;
    build_property_list(var->pl);
    build_feature_list(var->fl);
    var->s.initialize(0, x*var->factor);
    var->s.initialize(1, -y*var->factor);
    var->pl.initialize(var->s);
    var->fl.initialize(var->pl);
}


/// Continues a gesture with a new sample.

void IMGR::gstCont(float x, float y)
{
    var->s.new_sample();
    var->s.set(0, x*var->factor);
    var->s.set(1, -y*var->factor);
    var->pl.update(var->s);
}


/// Returns the number of samples used in the current gesture.

int IMGR::gstSamp()
{
    return (int) var->pl.look_up(PROPERTY_NUMBER_OF_SAMPLES).get();
}


/// Recognizes the current gesture using the current gesture library.

int IMGR::gstRecog()
{
    var->fl.calculate(var->pl);
    return var->r->recognize(var->fl);
}


/// Adds the current gesture to the current gesture library.

void IMGR::gstLearn(int id)
{
    var->fl.calculate(var->pl);
    var->r->learn(id, var->fl);
}


#else  // this produces a minimal build without the IMGR library!


struct IMGR_var { };
IMGR::IMGR() { }
IMGR::~IMGR() { }
void IMGR::libNew() { }
bool IMGR::libLoad(const char* lib) { return false; }
bool IMGR::libSave(const char* lib) { return false; }
void IMGR::gstNew(float x, float y, float factor) { }
void IMGR::gstCont(float x, float y) { }
int IMGR::gstSamp() { return 0; }
int IMGR::gstRecog() { return 0; }
void IMGR::gstLearn(int id) {}


#endif
