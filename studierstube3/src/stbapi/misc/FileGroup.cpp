#include <util/FileGroup.h>
#include <generic/Log.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>


//----------------------------------------------------------------------------

SO_NODE_SOURCE(FileGroup);

//----------------------------------------------------------------------------

void FileGroup::initClass()
{
	logfile << HEAD << "FileGroup: init" << ENDL;
	SO_NODE_INIT_CLASS(FileGroup, SoGroup, "Group");
}

//----------------------------------------------------------------------------

FileGroup::FileGroup()
{
	SO_NODE_CONSTRUCTOR(FileGroup);
    SO_NODE_ADD_FIELD(enginesIn,(NULL));
    SO_NODE_ADD_FIELD(nodesIn,(NULL));
    SO_NODE_ADD_FIELD(enginesOut,(""));
    SO_NODE_ADD_FIELD(nodesOut,(""));
    SO_NODE_ADD_FIELD(name,(""));

	enginesIn.setNum(0);
	nodesIn.setNum(0);
	enginesOut.setNum(0);
	nodesOut.setNum(0);

    //--- add field sensor to 'name' field

    nameSensor = new SoFieldSensor(nameChangedCB,this);
    nameSensor->setPriority(0);
    nameSensor->attach(&name);
}

//----------------------------------------------------------------------------

FileGroup::~FileGroup()
{
    delete nameSensor;
}

//----------------------------------------------------------------------------

SbBool FileGroup::readInstance(SoInput *input, unsigned short flags)
{
	if (!SoNode::readInstance(input,flags)) return FALSE;

	//--- search for the specified engines and add it to the namespace of SoInput

	for (int i=0; i<enginesOut.getNum(); i++)
	{
		const SbName &name = enginesOut[i];
		if (name.getLength()>0)
		{
			SoEngine *engine = SoEngine::getByName(name);
			if (engine) input->addReference(name,engine,FALSE);
		}
	}

	SoSearchAction sa;
	sa.setInterest(SoSearchAction::LAST);
	sa.setSearchingAll(TRUE);

	//--- search for the specified nodes and add it to the namespace of SoInput

	for (i=0; i<nodesOut.getNum(); i++)
	{
		const SbName &name = nodesOut[i];
		if (name.getLength()>0)
		{
			sa.setName(name);
			sa.apply(this);
			SoFullPath *path = (SoFullPath *)sa.getPath();
			if (path) input->addReference(name,path->getTail(),FALSE);
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------

void FileGroup::nameChangedCB(void *data, SoSensor *)
{
    FileGroup *fg = (FileGroup *)data;

	//--- open the specified file

    SoInput input;
    const char *str = fg->name.getValue().getString();
    if(!input.openFile(str))
    {
	    SoError::post("FileGroup::nameChangedCB()\ncannot open file %s\n",str);
        return;
    }

	//--- add the specified engines to the namespace of SoInput

	for (int i=0; i<fg->enginesIn.getNum(); i++)
	{
		SoEngine *engine = fg->enginesIn[i];
		if (engine && engine->getName().getLength()>0)
			input.addReference(engine->getName(),engine,FALSE);
	}

	//--- add the specified nodes to the namespace of SoInput

	for (i=0; i<fg->nodesIn.getNum(); i++)
	{
		SoNode *node = fg->nodesIn[i];
		if (node && node->getName().getLength()>0)
			input.addReference(node->getName(),node,FALSE);
	}

	//--- read the file

	SoSeparator *root = SoDB::readAll(&input);
    if (!root)
    {
	    SoError::post("FileGroup::nameChangedCB()\ncannot read file %s\n",str);
        return;
    }

	//--- replace content of the node

    fg->removeAllChildren();
	for (i=0; i<root->getNumChildren(); i++) fg->addChild(root->getChild(i));
	for (i=0; i<root->getNumChildren(); i++) root->removeChild(i);
}

//----------------------------------------------------------------------------
