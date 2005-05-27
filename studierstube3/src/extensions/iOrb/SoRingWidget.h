/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoRingWidget.h contains the definition of SoRingWidget.
 * SoRingWdiget is the base class for all ring type widgets.
 *
 * @author  EMC
 *
 * $Id: SoRingWidget.h,v 1.7 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_RING_WIDGET_H_
#define _SO_RING_WIDGET_H_

#include <Inventor/fields/SoSFFloat.h>

#include "SoMenuWidget.h"

/** 
 * SoRingWdiget is the base class for all ring type widgets.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoRingWidget: public SoMenuWidget
{
	SO_KIT_HEADER(SoRingWidget);

public:
	// construction
	SoRingWidget(void);
	static void initClass(void);

	// fields
	SoSFFloat radius;
	SoSFFloat itemSize;

protected:
	// methods
	virtual ~SoRingWidget(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	// members
};

#endif // _SO_RING_WIDGET_H_
