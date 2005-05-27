/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** extension.h
 *
 * @author  EMC
 *
 * $Id: extension.h,v 1.2 2005/01/22 20:10:27 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @defgroup iorb iOrb interaction widgets
 * @ingroup extensions
 *
 *
 * @author EMC, Gerhard Reitmayr
 */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXTENSION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IORB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef IORB_EXPORTS
#define IORB_API __declspec(dllexport)
#else
#define IORB_API __declspec(dllimport)
#endif

