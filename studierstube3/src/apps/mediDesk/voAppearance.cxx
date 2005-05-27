// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voAppearance.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#include <vo/utPerfMeter.h>
#include "voAppearance.h"
#include "voAppearanceElement.h"
#include "voStatus.h"

SO_NODE_SOURCE(voAppearance);

voLookupTable *aLookupTable = NULL;

//extern voStatus *appvstatus;
 voStatus *appvstatus;

//-----------------------------------------------------------------------------
// This initializes the voAppearance class.

void voAppearance::initClass(voStatus  *s)
{
   // cout << "voAppearance::initClass running ..." << endl;

   SO_NODE_INIT_CLASS(voAppearance, SoNode, "Node");
   SO_ENABLE(SoGLRenderAction, voAppearanceElement);
   SO_ENABLE(SoCallbackAction, voAppearanceElement);

   appvstatus = s;
}
 
//-----------------------------------------------------------------------------
// setVoScale.

void voAppearance::setVoScale(float value)
{
   // cout << "voAppearance::setVoScale running ..." << endl;
   this->voScale.setValue(value);
}

//-----------------------------------------------------------------------------
// setVolumeFormatField.

void voAppearance::setVolumeFormatField(char* value)
{
   // cout << "voAppearance::setVolumeFormatField running ..." << endl;
   this->volumeFormatField.set(value);
}


//-----------------------------------------------------------------------------
// setDataFormatField.

void voAppearance::setDataFormatField(char* value)
{
   // cout << "voAppearance::setDataFormatField running ..." << endl;
   this->dataFormatField.set(value);
}


//-----------------------------------------------------------------------------
// setVolumeSizeXField.

void voAppearance::setVolumeSizeXField(int x)
{
   // cout << "voAppearance::setVolumeSizeXField running ..." << endl;
   this->volumeSizeXField.setValue(x);
}

//-----------------------------------------------------------------------------
// setVolumeSizeYField.

void voAppearance::setVolumeSizeYField(int y)
{
   // cout << "voAppearance::setVolumeSizeYField running ..." << endl;
   this->volumeSizeYField.setValue(y);
}

//-----------------------------------------------------------------------------
// setVolumeSizeZField.

void voAppearance::setVolumeSizeZField(int z)
{
   // cout << "voAppearance::setVolumeSizeZField running ..." << endl;
   this->volumeSizeZField.setValue(z);
}


//-----------------------------------------------------------------------------
// setHeaderLengthField.

void voAppearance::setHeaderLengthField(int hl)
{
   // cout << "voAppearance::setHeaderLength running ..." << endl;
   this->headerLengthField.setValue(hl);
}

//-----------------------------------------------------------------------------
// setSuffixLengthField.

void voAppearance::setSuffixLengthField(int sl)
{
   // cout << "voAppearance::setSuffixLength running ..." << endl;
   this->suffixLengthField.setValue(sl);
}



//-----------------------------------------------------------------------------
// set useLutStatus.

void voAppearance::setUseLutStatus()
{
   // cout << "voAppearance::setUseLutStatus running ..." << endl;
   this->useLutStatus.setValue( !(this->useLutStatus.getValue()) );
   if (this->useLutStatus.getValue()) {
      appvstatus->setUseLutStatus(true);
   } else {
      appvstatus->setUseLutStatus(false);
   }
}


//-----------------------------------------------------------------------------
// useLut.

bool voAppearance::useLut()
{
   // cout << "voAppearance::useLut running ..." << endl;
   return this->useLutStatus.getValue();
}


//-----------------------------------------------------------------------------
// set useMipStatus.

void voAppearance::setUseMipStatus()
{
   // cout << "voAppearance::setUseMipStatus running ..." << endl;

   this->useMipStatus.setValue( !(this->useMipStatus.getValue()) );
   appvstatus->setMipStatus(this->useMipStatus.getValue());
}


//-----------------------------------------------------------------------------
// lookup table

void voAppearance::lut(voIndexedTetraSet *, voBrickSetCollection * aVolume, \
    voRenderingMode renderingMode, int lutLength, int lutcenter, int lutwidth )
{

int myReadLut(char *inFileName, int &lutLength, void *lookupEntries, voDataType dataType);

    voLookupTableType lookupType = voLookupTableTypeScope::POST_INTERPOLATION;
    voDataType dataType = voDataTypeScope::DEFAULT;
    voInternalFormatType internalFormat;
    voExternalFormatType externalFormat;

    // cout << "----" << endl;
    // cout << "running lut ..." << endl;

    // have to run this method in order to get lookupType
    // all other data would have been stored already in the voStore object
    voLookupTable::getBestParameters(
                      aVolume, renderingMode, lutLength,
                      lookupType, internalFormat, externalFormat, dataType);

    // cout << "\trenderingMode\t\t<" << renderingMode << ">" << endl;
    // cout << "\tlutLength\t\t<" << lutLength << ">" << endl;
    // cout << "\tlookupType\t\t<" << lookupType << ">" << endl;
    // cout << "\tinternalFormat\t\t<" << internalFormat << ">" << endl;
    // cout << "\texternalFormat\t\t<" << externalFormat << ">" << endl;
    // cout << "\tdataType\t\t<" << dataType << ">" << endl;

    if (lookupType == voLookupTableTypeScope::PRE_INTERPOLATION) {
        // cout << "lut type <PRE_INTERPOLATION>" << endl;
        aLookupTable = new voLookupTablePre(internalFormat, lutLength, externalFormat, dataType);
    } else {
        // cout << "lut type <POST_INTERPOLATION>" << endl;
        aLookupTable = new voLookupTablePost(internalFormat, lutLength, externalFormat, dataType);
   }


   // generate a linear grayscale lookup table unless one was supplied
   // cout << "\t+++create linear lut ..." << endl;
   aLookupTable->set(voLookupTableShapeScope::LINEAR, lutcenter, lutwidth);


   if (this->useLut()) {
      // cout << "\tuseLut\t\ttrue" << endl;
      aLookupTable->load();
      aLookupTable->enable();

      if (aLookupTable->getType() == voLookupTableTypeScope::PRE_INTERPOLATION) {
         //cout << "\t+++set dirtyflag ..." << endl;
         this->aVolume->setDirtyFlag(VO_TRUE);
      }

      // build texture objects, interleave, and do other fancy stuff
      voAppearanceActions::volumeOptimize(
         this->aVolume,voOptimizeVolumeTypeScope::BEST_PERFORMANCE);

   } else {

      // cout << "\tuseLut\t\tfalse" << endl;
      aLookupTable->disable();

   }
}

//-----------------------------------------------------------------------------
// create inputFileNames depending on fileName, suffixLength and volumeSizeZ
// caution!!! this function uses a suffixLength 3!

char ** voAppearance::createRawFileNames(char * fileName, int sliceCount) {

   char ** fn;
   int i;

   fn = (char **) malloc(sliceCount*sizeof(char*));
  
   for (i=0;i<sliceCount;i++) {
      fn[i] = (char *) malloc((strlen(fileName)+3)*sizeof(char));
      sprintf(fn[i], "%s%03d",fileName,i);
      // cout << "fileName[" << i << "] <" << fn[i] << ">" << endl;
   }
  
   return fn;

}



//-----------------------------------------------------------------------------
// read volume RAW format

void voAppearance::readVolumeRAW(void)
{
   cout << "voAppearance::readVolumeRAW running ..." << endl;

int myReadBrickRaw(char **fileNames, void *data,        /* DataType */
                   int xBrickOrigin, int yBrickOrigin, int zBrickOrigin,
                   int xBrickSize, int yBrickSize, int zBrickSize,
                   int xVolumeSize, int yVolumeSize, int zVolumeSize,
                   int headerLength, int bytesPerVoxel);


   int    s = 0;                        // brick count 
   int    maxSamplesNumber;
   int    maxBrickCount;
   int    xVolumeSize;
   int    yVolumeSize;
   int    zVolumeSize;
   int    xBrickSize;
   int    yBrickSize;
   int    zBrickSize;
   int    headerLength;
   int    bytesPerVoxel;
   float  loValue = -1.0;               // do not scale the data range
   float  hiValue = -1.0;               // do not scale the data range
   voExternalFormatType diskDataFormat;
   voExternalFormatType externalFormat;
   voInternalFormatType internalFormat;
   voDataType dataType;
   voRenderingMode renderingMode = (voRenderingMode)renderingModeField.getValue();
   voInterpolationType interpolationType = (voInterpolationType)interpolationTypeField.getValue();


   // get dataType and diskDataFormat
   const char *dType = getDataFormatField();

   if (!strcmp(dType, "ubyte")) {
      dataType = voDataTypeScope::UNSIGNED_BYTE;
      bytesPerVoxel = 1;
      diskDataFormat = voExternalFormatTypeScope::LUMINANCE;
   } else if (!strcmp(dType, "ushort")) {
      dataType = voDataTypeScope::UNSIGNED_SHORT;
      bytesPerVoxel = 2;
      diskDataFormat = voExternalFormatTypeScope::LUMINANCE;
   } else if (!strcmp(dType, "ubyte4")) {
      dataType = voDataTypeScope::UNSIGNED_BYTE;
      bytesPerVoxel = 4;
      diskDataFormat = voExternalFormatTypeScope::RGBA;
   } else if (!strcmp(dType, "int8888")) {
      dataType = voDataTypeScope::UNSIGNED_INT_8_8_8_8_EXT;
      bytesPerVoxel = 4;
      diskDataFormat = voExternalFormatTypeScope::RGBA;
   } else if (!strcmp(dType, "float")) {
      dataType = voDataTypeScope::FLOAT;
      bytesPerVoxel = 4;
      diskDataFormat = voExternalFormatTypeScope::LUMINANCE;
   } else {
      fprintf(stderr, "Usage: dataFormat has to be \"ubyte\" or \"ushort\" or \"int8888\" or \"float\".\n");
      exit(1);
   }

   // get headerLength
   headerLength = getHeaderLengthField();

   // get volume sizes and voxel format
   xVolumeSize = getVolumeSizeXField();
   yVolumeSize = getVolumeSizeYField();
   zVolumeSize = getVolumeSizeZField();

   // init brick size
   xBrickSize = xVolumeSize;
   yBrickSize = yVolumeSize;
   zBrickSize = zVolumeSize;

   // get dataFileName
   char *fileName = (char *)fileNameField.getValue().getString();
   printf ("fileName <%s>\n",fileName);
   char **dataFileName = createRawFileNames(fileName, zVolumeSize);

   voAppearanceActions::getBestParameters(
                         interpolationType, renderingMode, dataType,    // In
                         diskDataFormat,
                         internalFormat, externalFormat,                // Out
                         xBrickSize, yBrickSize, zBrickSize);           // In/Out

   // create the bricks--this does not read or allocate any data--only computes
   // bricks' origins/sizes given the volume dimensions and brick sizes

   // possible voPartialBrickTypeScope:
   // enum voPartialBrickType {  
   //    DEFAULT,
   //    TRUNCATE,
   //    AUGMENT,
   //    AUGMENT_TIGHT
   // }; 
   voBrickSetCollection *curVolume = new voBrickSetCollection(
                                xVolumeSize, 
                                yVolumeSize, 
                                zVolumeSize,
                                xBrickSize, 
                                yBrickSize, 
                                zBrickSize,
                                voPartialBrickTypeScope::AUGMENT_TIGHT,
                                1,
                                internalFormat,
                                externalFormat,
                                dataType,
                                interpolationType);

   if (voError:: getErrorNumber() != voErrorTypeScope::NO_ERROR) {
      fprintf(stderr, "%s", voError::getErrorString());
      exit(1);
   }

   // get loading time
   voutTimer aTimer;
   aTimer.start();

   // Allocate storage for all the copis of the volume (voInterpolationTypeScope::_2D)
   voBrickSetCollectionIterator collectionIter(curVolume);
   for (voBrickSet * brickSet; brickSet = collectionIter();) {
      // iterate over all bricks within the brickCollection
      voBrickSetIterator brickSetIter(brickSet);
      for (voBrick * brick; brick = brickSetIter();)
         voAppearanceActions::dataAlloc(brick);
   }
   curVolume->setCurrentBrickSet(voPlaneOrientationScope::XY);

   voBrickSetIterator brickSetIter(curVolume->getCurrentBrickSet());
   for (voBrick * brick; brick = brickSetIter();) {
      int xBrickOrigin, yBrickOrigin, zBrickOrigin;
      int xBrickSize, yBrickSize, zBrickSize;
      void *vdata = brick->getDataPtr();

      // get brick sizes -- they may different than those requested
      brick->getBrickSizes(xBrickOrigin, yBrickOrigin, zBrickOrigin,
                           xBrickSize, yBrickSize, zBrickSize);

      // read the data; OK to use brick data area as a temporary buffer
      cout << "reading brick[" << s << "]" << endl;
      myReadBrickRaw(dataFileName, vdata,
                     xBrickOrigin, yBrickOrigin, zBrickOrigin,
                     xBrickSize, yBrickSize, zBrickSize,
                     xVolumeSize, yVolumeSize, zVolumeSize,
                     headerLength, 
                     bytesPerVoxel);

      // replicate to the desired externalFormat
      voAppearanceActions::dataConvert(brick, vdata, diskDataFormat);

      // expand the values to span the whole dymanic range
      voAppearanceActions::dataScaleRange(brick, loValue, hiValue);

      //fprintf(stderr, "+");
      s++;
   }
   //fprintf(stderr, "\n");

   if (curVolume->getInterpolationType() == voInterpolationTypeScope::_2D)
      voAppearanceActions::volumeMakeTransposed(curVolume);

   // print loading time
   aTimer.stop();
   cerr << "Load time " << aTimer << endl;

   // Determine the largest BrickCount over the BrickSetCollection.
   voBrickSetCollectionIterator aCollectionIterator(curVolume);
   voBrickSet *aBrickSet;
   for (int j1 = 0; aBrickSet = aCollectionIterator(); j1++)
      if (j1 == 0 || aBrickSet->getBrickCount() > maxBrickCount)
         maxBrickCount = aBrickSet->getBrickCount();

   // Set the number of sampling surafaces to the largest dimension
   float maxSize = xVolumeSize;
   if (maxSize < yVolumeSize)
      maxSize = yVolumeSize;
   if (maxSize < zVolumeSize)
      maxSize = zVolumeSize;
   //maxSamplesNumber = maxSize;
   maxSamplesNumber = 2*maxSize;

   
   aVolume       = curVolume;

    // this optimization has to be done once only at load time
   if(aVolume->getInterpolationType() == voInterpolationTypeScope::_2D)
      voAppearanceActions::volumeOptimize(aVolume, voOptimizeVolumeTypeScope::PRE_TRANSPOSE);

   // store data in appvstatus object
   appvstatus->setVolumeSize(xVolumeSize, yVolumeSize, zVolumeSize);
   appvstatus->setBrickSize(xBrickSize, yBrickSize, zBrickSize);
   appvstatus->setMaxBrickCount(maxBrickCount);
   appvstatus->setRenderingMode((int)renderingMode);
   appvstatus->setInterpolationType((int)interpolationType);
   appvstatus->setDataType((int)dataType);
   appvstatus->setBytesPerVoxel(bytesPerVoxel);
   appvstatus->setDiskDataFormat((int)diskDataFormat);
   appvstatus->setExternalDataFormat((int)externalFormat);
   appvstatus->setInternalDataFormat((int)internalFormat);
   appvstatus->setMaxSamplesNumber(maxSamplesNumber);
   appvstatus->setVolumeReadStatus(true);

   cout << "end voAppearance::readVolumeRAW." << endl;

}


//-----------------------------------------------------------------------------
// read volume TIFF format

void voAppearance::readVolume(void)
{
   cout << "voAppearance::readVolume running ..." << endl;

int myGetVolumeSizesIfl(char *fileName,
                    int &xVolumeSize, int &yVolumeSize, int &zVolumeSize,
                        voExternalFormatType & diskDataFormat,
                        voDataType & dataType);

int myReadBrickIfl(char *fileName, void *data,
                   int xBrickOrigin, int yBrickOrigin, int zBrickOrigin,
                   int xBrickSize, int yBrickSize, int zBrickSize,
                   int xVolumeSize, int yVolumeSize, int zVolumeSize);

   int    s = 0;                        // brick count
   int    maxSamplesNumber;
   int    maxBrickCount; 
   int    bytesPerVoxel; 
   int    xVolumeSize; 
   int    yVolumeSize; 
   int    zVolumeSize;
   int    xBrickSize; 
   int    yBrickSize; 
   int    zBrickSize;
   float  loValue = -1.0;               // do not scale the data range
   float  hiValue = -1.0;               // do not scale the data range
   voExternalFormatType diskDataFormat;
   voExternalFormatType externalFormat;
   voInternalFormatType internalFormat;
   voDataType dataType;
   voRenderingMode renderingMode = (voRenderingMode)renderingModeField.getValue();
   voInterpolationType interpolationType = (voInterpolationType)interpolationTypeField.getValue();


   char *dataFileName = (char *)fileNameField.getValue().getString();

   // get volume sizes and voxel format
   if (myGetVolumeSizesIfl(dataFileName,
			   xVolumeSize, yVolumeSize, zVolumeSize,
			   diskDataFormat,
			   dataType))	
      exit(1);

   // get bytes per voxel
   if (diskDataFormat == voExternalFormatTypeScope::LUMINANCE) {
      bytesPerVoxel = 1;
   } else if (diskDataFormat == voExternalFormatTypeScope::RGBA) {
      bytesPerVoxel = 4;
   }

   // init bricksize
   xBrickSize = xVolumeSize;
   yBrickSize = yVolumeSize;
   zBrickSize = zVolumeSize;

   voAppearanceActions::getBestParameters(
                        interpolationType, renderingMode, dataType, diskDataFormat, // in
                        internalFormat, externalFormat,                             // Out
                        xBrickSize, yBrickSize, zBrickSize);                        // In/Out

   // create the bricks--this does not read or allocate any data--only computes
   // bricks' origins/sizes given the volume dimensions and brick sizes
   
   // possible voPartialBrickTypeScope:
   // enum voPartialBrickType {  
   //    DEFAULT,
   //    TRUNCATE,
   //    AUGMENT,
   //    AUGMENT_TIGHT
   // }; 
   voBrickSetCollection *curVolume = new voBrickSetCollection(
				xVolumeSize, yVolumeSize, zVolumeSize,
				xBrickSize, yBrickSize, zBrickSize,
				voPartialBrickTypeScope::AUGMENT_TIGHT,
				1,
				internalFormat,
				externalFormat,
				dataType,
				interpolationType);

   if (voError:: getErrorNumber() != voErrorTypeScope::NO_ERROR) {
      fprintf(stderr, "%s", voError::getErrorString());
      exit(1);
   }

   // get loading time
   voutTimer aTimer;
   aTimer.start();

   // Allocate storage for all the copis of the volume (voInterpolationTypeScope::_2D)
   voBrickSetCollectionIterator collectionIter(curVolume);
   for (voBrickSet * brickSet; brickSet = collectionIter();) {
      // iterate over all bricks within the brickCollection
      voBrickSetIterator brickSetIter(brickSet);
      for (voBrick * brick; brick = brickSetIter();)
         voAppearanceActions::dataAlloc(brick);
   }
   curVolume->setCurrentBrickSet(voPlaneOrientationScope::XY);

   voBrickSetIterator brickSetIter(curVolume->getCurrentBrickSet());
   for (voBrick * brick; brick = brickSetIter();) {

      int xBrickOrigin, yBrickOrigin, zBrickOrigin;
      int xBrickSize, yBrickSize, zBrickSize;

      void *vdata = brick->getDataPtr();

      // get brick sizes -- they may different than those requested
      brick->getBrickSizes(xBrickOrigin, yBrickOrigin, zBrickOrigin,
                           xBrickSize, yBrickSize, zBrickSize);

      // read the data; OK to use brick data area as a temporary buffer
      cout << "reading brick[" << s << "]" << endl;
      myReadBrickIfl(dataFileName, vdata,
                     xBrickOrigin, yBrickOrigin, zBrickOrigin,
                     xBrickSize, yBrickSize, zBrickSize,
                     xVolumeSize, yVolumeSize, zVolumeSize);

      // replicate to the desired externalFormat
      voAppearanceActions::dataConvert(brick, vdata, diskDataFormat);

      // expand the values to span the whole dymanic range
      voAppearanceActions::dataScaleRange(brick, loValue, hiValue);

      //fprintf(stderr, "+");
      s++;
   }
   //fprintf(stderr, "\n");

   if (curVolume->getInterpolationType() == voInterpolationTypeScope::_2D)
      voAppearanceActions::volumeMakeTransposed(curVolume);

   // print loading time
   aTimer.stop();
   cerr << "Load time " << aTimer << endl;

   // Determine the largest BrickCount over the BrickSetCollection.
   voBrickSetCollectionIterator aCollectionIterator(curVolume);
   voBrickSet *aBrickSet;
   for (int j1 = 0; aBrickSet = aCollectionIterator(); j1++)
      if (j1 == 0 || aBrickSet->getBrickCount() > maxBrickCount)
         maxBrickCount = aBrickSet->getBrickCount();

   // Set the number of sampling surafaces to the largest dimension
   float maxSize = xVolumeSize;
   if (maxSize < yVolumeSize)
      maxSize = yVolumeSize;
   if (maxSize < zVolumeSize)
      maxSize = zVolumeSize;
   //maxSamplesNumber = maxSize;
   maxSamplesNumber = 2*maxSize;

   aVolume       = curVolume;

    // this optimization has to be done once only at load time
   if( aVolume->getInterpolationType() == voInterpolationTypeScope::_2D )
      voAppearanceActions::volumeOptimize(
         aVolume, voOptimizeVolumeTypeScope::PRE_TRANSPOSE );

   // store data into appvstatus object
   appvstatus->setVolumeSize(xVolumeSize, yVolumeSize, zVolumeSize);
   appvstatus->setBrickSize(xBrickSize, yBrickSize, zBrickSize);
   appvstatus->setMaxBrickCount(maxBrickCount);
   appvstatus->setRenderingMode((int)renderingMode);
   appvstatus->setInterpolationType((int)interpolationType);
   appvstatus->setDataType((int)dataType);
   appvstatus->setBytesPerVoxel(bytesPerVoxel);
   appvstatus->setDiskDataFormat((int)diskDataFormat);
   appvstatus->setExternalDataFormat((int)externalFormat);
   appvstatus->setInternalDataFormat((int)internalFormat);
   appvstatus->setMaxSamplesNumber(maxSamplesNumber);
   appvstatus->setVolumeReadStatus(true);

}


//-----------------------------------------------------------------------------
// fileNameChangeCB

static void fileNameChangedCB(void *data, SoSensor *)
{

   cout << "voAppearance::fileNameChangedCB running ..." << endl;

   //const char *volFormat;
   char *volFormat;

   voAppearance *volume = (voAppearance *)data;
   volFormat = (char*) volume->getVolumeFormatField();

   if (strcmp(volFormat,"TIFF") == 0) {
      cout << "volumeFormat <TIFF>" << endl;
      volume->readVolume();
      cout << "volumeFormat <TIFF>" << endl;
   } else if (strcmp(volFormat,"RAW") == 0) {
      cout << "volumeFormat <RAW>" << endl;
      volume->readVolumeRAW();
   } else {
      cout << "ERROR wrong volumeFormat!" << endl;
      exit(1);
   }
   cout << "end voAppearance::fileNameChangedCB." << endl;

}


//-----------------------------------------------------------------------------
// lutCenterCB

static void lutCenterCB(void *data, SoSensor *)
{

   // cout << "voAppearance::lutCenterCB running... "<< endl;

   voAppearance *volume = (voAppearance *)data;
   if (! appvstatus->getVolumeReadStatus()) {
      cout << "\tvoAppearance::volume does not exist yet!!" << endl;
      return;
   }

   volume->lut(appvstatus->getTetraSet(), \
               volume->aVolume, \
               voRenderingModeScope::COLOR, \
               256, \
               volume->lutCenter.getValue(), \
               volume->lutWidth.getValue());

}


//-----------------------------------------------------------------------------
// lutWidthCB

static void lutWidthCB(void *data, SoSensor *)
{

   // cout << "voAppearance::lutWidthCB running... "<< endl;

   voAppearance *volume = (voAppearance *)data;
   if (! appvstatus->getVolumeReadStatus()) {
      cout << "\tvoAppearance::volume does not exist yet!!" << endl;
      return;
   }

   volume->lut(appvstatus->getTetraSet(), \
               volume->aVolume, \
               voRenderingModeScope::COLOR, \
               256, \
               volume->lutCenter.getValue(), \
               volume->lutWidth.getValue());

}


//-----------------------------------------------------------------------------
// voScaleCB

static void voScaleCB(void *data, SoSensor *)
{
   //cout << "voAppearance::voScaleCB running... "<< endl;

   voAppearance *volume = (voAppearance *)data;

   AGETNODE(scaleNode,SoScale,"SCALE_VOLUME");
   scaleNode->scaleFactor.setValue( 
      volume->voScale.getValue(), 
      volume->voScale.getValue(), 
      volume->voScale.getValue());

}


//-----------------------------------------------------------------------------
// useLutCB

static void useLutCB(void *data, SoSensor *)
{

   // cout << "voAppearance::useLutCB running... "<< endl;

   voAppearance *volume = (voAppearance *)data;
   if (! appvstatus->getVolumeReadStatus()) {
      cout << "\tvoAppearance::volume does not exist yet!!" << endl;
      return;
   }

   if (volume->useLut()) {
      volume->lut(appvstatus->getTetraSet(), \
               volume->aVolume, \
               voRenderingModeScope::COLOR, \
               256, \
               volume->lutCenter.getValue(), \
               volume->lutWidth.getValue());
      aLookupTable->load();
      aLookupTable->enable();
   } else {
      if (aLookupTable == NULL) return;
      aLookupTable->disable();
   }

   if (aLookupTable->getType() == voLookupTableTypeScope::PRE_INTERPOLATION) {
      //cout << "\t+++set dirtyflag ..." << endl;
      volume->aVolume->setDirtyFlag(VO_TRUE);
   }

   // build texture objects, interleave, and do other fancy stuff
   voAppearanceActions::volumeOptimize(
             volume->aVolume,
             voOptimizeVolumeTypeScope::BEST_PERFORMANCE);

}

//-----------------------------------------------------------------------------
// useMipCB

static void useMipCB(void *data, SoSensor *)
{

   // cout << "voAppearance::useMipCB running... "<< endl;

   appvstatus->setMipStatus( true );
   appvstatus->setAttenuateStatus( false );
}

//-----------------------------------------------------------------------------
// useAttenuateCB

static void useAttenuateCB(void *data, SoSensor *)
{

   // cout << "voAppearance::useAttenuateCB running... "<< endl;

   appvstatus->setMipStatus( false );
   appvstatus->setAttenuateStatus( true );
}


//-----------------------------------------------------------------------------
// Constructor

voAppearance::voAppearance()
{
   // cout << "voAppearance::constructor running ..." << endl;

   SO_NODE_CONSTRUCTOR(voAppearance);
   SO_NODE_ADD_FIELD(fileNameField,(-1));
   SO_NODE_ADD_FIELD(volumeFormatField,(-1));
   SO_NODE_ADD_FIELD(dataFormatField,(-1));
   SO_NODE_ADD_FIELD(volumeSizeXField,(-1));
   SO_NODE_ADD_FIELD(volumeSizeYField,(-1));
   SO_NODE_ADD_FIELD(volumeSizeZField,(-1));
   SO_NODE_ADD_FIELD(suffixLengthField,(-1));
   SO_NODE_ADD_FIELD(headerLengthField,(-1));

   SO_NODE_DEFINE_ENUM_VALUE(voInterpolationType, voInterpolationTypeScope::DEFAULT);
   SO_NODE_DEFINE_ENUM_VALUE(voInterpolationType, voInterpolationTypeScope::_2D);
   SO_NODE_DEFINE_ENUM_VALUE(voInterpolationType, voInterpolationTypeScope::_3D);
   SO_NODE_SET_SF_ENUM_TYPE(interpolationTypeField,voInterpolationType);
   SO_NODE_ADD_FIELD(interpolationTypeField,(voInterpolationTypeScope::DEFAULT));

   SO_NODE_DEFINE_ENUM_VALUE(voRenderingMode, voRenderingModeScope::DEFAULT);
   SO_NODE_DEFINE_ENUM_VALUE(voRenderingMode, voRenderingModeScope::MONOCHROME);
   SO_NODE_DEFINE_ENUM_VALUE(voRenderingMode, voRenderingModeScope::COLOR);
   SO_NODE_SET_SF_ENUM_TYPE(renderingModeField,voRenderingMode);
   SO_NODE_ADD_FIELD(renderingModeField,(voRenderingModeScope::MONOCHROME));

   SO_NODE_ADD_FIELD(lutCenter,(-1));
   SO_NODE_ADD_FIELD(lutWidth,(-1));
   SO_NODE_ADD_FIELD(useLutStatus,(FALSE));
   SO_NODE_ADD_FIELD(voScale,(1.0));

   // install callback that will force data read
   SoFieldSensor *aSensor1 = new SoFieldSensor(fileNameChangedCB, this);
   aSensor1->attach(&fileNameField);

   SoFieldSensor *aSensor2 = new SoFieldSensor(lutCenterCB, this);
   aSensor2->attach(&lutCenter);

   SoFieldSensor *aSensor3 = new SoFieldSensor(lutWidthCB, this);
   aSensor3->attach(&lutWidth);

   SoFieldSensor *aSensor4 = new SoFieldSensor(useLutCB, this);
   aSensor4->attach(&useLutStatus);

   SoFieldSensor *aSensor5 = new SoFieldSensor(voScaleCB, this);
   aSensor5->attach(&voScale);

   SoFieldSensor *aSensor6 = new SoFieldSensor(useMipCB, this);
   aSensor6->attach(&useMipStatus);

}


//-----------------------------------------------------------------------------
// Destructor

voAppearance::~voAppearance()
{
}

void voAppearance::GLRender(SoGLRenderAction *action)
{
   // cout << "voAppearance::GLRenderAction running ..." << endl;

   voAppearance::doAction(action);
}


//-----------------------------------------------------------------------------
// callback
void voAppearance::callback(SoCallbackAction *action)
{
   // cout << "voAppearance::callback running ..." << endl;

   voAppearance::doAction((SoAction *)action);
}


//-----------------------------------------------------------------------------
// doAction

void voAppearance::doAction(SoAction *action)
{
   // cout << "voAppearance::doAction running ..." << endl;

   // Initialize only once (or when texture objects change)

   static int virgin = 1;  
   if(virgin)
      voAppearanceActions::volumeOptimize(this->getVolume(),
                   (voOptimizeVolumeType)
                   (voOptimizeVolumeTypeScope::BEST_PERFORMANCE ));
   virgin=0;
   voAppearanceElement::set(action->getState(), this, this);
}
