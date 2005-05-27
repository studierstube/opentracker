/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoArrayWidget.h contains the definition of SoArrayWidget.
 * SoArrayWidget is the the base class for all array (list) type widgets.
 *
 * @author  EMC
 *
 * $Id: SoArrayWidget.h,v 1.8 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_ARRAY_WIDGET_H_
#define _SO_ARRAY_WIDGET_H_

#include <Inventor/fields/SoSFInt32.h>

#include "SoMenuWidget.h"

/** 
 * SoArrayWidget is the the base class for all array (list) type widgets.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoArrayWidget: public SoMenuWidget
{
	SO_KIT_HEADER(SoArrayWidget);

public:
	// construction
	SoArrayWidget(void);
	static void initClass(void);

	// fields
	SoSFFloat	itemSize;
	SoSFInt32	displayLines;

protected:
	// methods
	virtual ~SoArrayWidget(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	void createGeometryArrowUp(float lineheight, SoGroup* parent);
	void createGeometryArrowDown(float lineheight, SoGroup* parent);

	// members
	int		firstVisibleLine;
};

#endif // _SO_ARRAY_WIDGET_H_
