#ifndef __FILEGROUP
#define __FILEGROUP

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFEngine.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif
#include "../ippy.h" 

//----------------------------------------------------------------------------

class IPPY_API FileGroup : public SoGroup
{
	SO_NODE_HEADER(FileGroup);

public:
	SoSFString name;
	SoMFEngine enginesIn;
	SoMFNode nodesIn;
	SoMFString enginesOut;
	SoMFString nodesOut;

	static void initClass();
	FileGroup();

protected:
	SoFieldSensor *nameSensor;

	virtual ~FileGroup();
	virtual SbBool readInstance(SoInput *input, unsigned short flags);
	static void nameChangedCB(void *data, SoSensor *);
};

//----------------------------------------------------------------------------

#endif
