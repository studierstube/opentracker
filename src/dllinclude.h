 /* ========================================================================
  * (C) 2000 Vienna University of Technology
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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/dllinclude.h,v 1.1 2001/03/26 22:11:20 reitmayr Exp $
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