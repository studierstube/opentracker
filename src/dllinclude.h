 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Include file to make using DLL simpler.
  * The following ifdef block is the standard way of creating macros which
  * make exporting from a DLL simpler. All files within this DLL are
  * compiled with the OPENTRACKER_EXPORTS symbol defined on the command
  * line. this symbol should not be defined on any project that uses this
  * DLL. This way any other project whose source files include this file see
  * OPENTRACKER_API functions as being imported from a DLL, wheras this DLL sees
  * symbols defined with this macro as being exported.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/dllinclude.h,v 1.3 2001/03/27 06:08:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENTRACKER_H_INCLUDED
#define _OPENTRACKER_H_INCLUDED

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4251)
//#pragma warning(disable:4275)
//#pragma warning(disable:4305)
    #ifdef OPENTRACKER_STATIC
        #define OPENTRACKER_API
        #define OPENTRACKER_EXTERN extern
    #else
        #ifdef OPENTRACKER_EXPORTS
            #define OPENTRACKER_API __declspec(dllexport)
            #define OPENTRACKER_EXTERN
        #else
            #define OPENTRACKER_API __declspec(dllimport)
            #define OPENTRACKER_EXTERN extern
        #endif
    #endif
#else
    #define OPENTRACKER_API
#endif

#endif
