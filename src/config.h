 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Configuration file for OpenTracker. There are a lot of defines that
  * include or exclude certain device drivers etc. These are collected here
  * to provide a single file to edit.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/Attic/config.h,v 1.2 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENTRACKER_CONFIG_H
#define _OPENTRACKER_CONFIG_H

// the following options can only be used under windows
#ifdef WIN32

/** define this, if you want to compile in InterTrax support */
#define USE_INTERTRAX 1

/** define this, if you want to compile in ARToolKit support */
#define USE_ARTOOLKIT 1

/** define this, if you want to compile in Wacom Graphire Tablet support */
#define USE_WACOMGRAPHIRE 1

/** define this, if you want to compile in support for Cyber Mouse */
#define USE_CYBERMOUSE 1

#endif // WIN32

#endif
