 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** SoSampler.cxx contains the implementation of class SoSampler
  *
  * @author Rainer Splechtna
  *
  * $Id: SoSampler.cxx 4187 2005-04-28 06:28:41Z bornik $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoTransform.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include "SoSampler.h"

SoSampler::SoSampler()
:
dataSensor(NULL), 
buttonSensor(NULL), 
buttonStation(0),
buttonNumber(0),
buttonState(1),
numberOfSamples(0),
continousSamplingCB(NULL),
continousSamplingCBData(NULL)
{
  int i;
    for (i=0;i<MAX_STATION_NUMBER;i++)
        validStations[i] = FALSE;
}

void SoSampler::samplingCB(void *data,SoSensor *)
{
    SoSampler *s = (SoSampler *)data;
	s->sample();
}

int
SoSampler::getFirstValidStation()
{
    for (int i=0;i<MAX_STATION_NUMBER;i++)
    {
        if (validStations[i] == TRUE)
            return i;
    }
    return -1;
}

void
SoSampler::startContinousSampling(void (*f)(void *), void *data)
{
    int first;
    if ((first = getFirstValidStation()) != -1)
    {
        dataSensor = new SoFieldSensor(samplingCB, this);
        dataSensor->setPriority(0);
        dataSensor->attach(Stb3DEventGenerator::getTranslationField(first)); 
        continousSamplingCB = f;
        continousSamplingCBData = data;
    }
}

void 
SoSampler::setStationsToSample(const SoMFInt32 &stations)
{
    int i;
    for (i=0;i<MAX_STATION_NUMBER;i++)
        validStations[i] = FALSE;
    for (i=0;i<stations.getNum();i++)    
        validStations[stations[i]] = TRUE;
}

void 
SoSampler::stopContinousSampling()
{
    if (dataSensor)
    {
	    dataSensor->detach();
	    delete(dataSensor);
	    dataSensor = NULL;
        continousSamplingCB = NULL;
        continousSamplingCBData = NULL;
    }
}

void 
SoSampler::clearSamples(int index)
{
    if (index < 0)
        return;
    for (int i=0;i<MAX_STATION_NUMBER;i++)
	{
        if (!validStations[i])
            continue;
	    samples[i].translations.deleteValues(index);
	    samples[i].rotations.deleteValues(index);
    }
}

void 
SoSampler::clearAllSamples()
{
    numberOfSamples = 0;
    clearSamples(0);
}

void 
SoSampler::stepBack()
{
    if (numberOfSamples < 1)
        return;

    clearSamples(--numberOfSamples);
}


void 
SoSampler::sample()
{
    for (int i=0;i<MAX_STATION_NUMBER;i++)
	{
        if (!validStations[i])
            continue;
	    samples[i].translations.set1Value(numberOfSamples,
            Stb3DEventGenerator::getTranslationField(i)->getValue());
	    samples[i].rotations.set1Value(numberOfSamples,
            Stb3DEventGenerator::getRotationField(i)->getValue());
    }
    numberOfSamples++;

    if (continousSamplingCB)
        continousSamplingCB(continousSamplingCBData);
}

                              
void
SoSampler::initButtonTriggering(int station, 
                                int buttonNumber, 
                                SbBool triggerOnce, 
                                SbBool sampleWhenTriggered,
                                void (*f)(void *),
                                void *data)
{
	buttonStation = station;
	this->buttonNumber = buttonNumber;
    this->triggerOnce = triggerOnce;
    this->sampleWhenTriggered = sampleWhenTriggered;
	buttonTriggered = f;
    buttonTriggeredData = data;
	buttonSensor = new SoFieldSensor(buttonCB, this);
    buttonSensor->setPriority(0);
}


void
SoSampler::startButtonTriggering()
{
    if (buttonSensor)
        buttonSensor->attach(Stb3DEventGenerator::getButtonField(buttonStation,buttonNumber)); 
}	

void
SoSampler::stopButtonTriggering()
{
    if (buttonSensor)
	    buttonSensor->detach();
}	


void 
SoSampler::logSampledData(char *dataFile, SbBool append) 
{
    FILE *datalog = NULL;
    
    if (append)
        datalog = fopen(dataFile, "a");
    else
        datalog = fopen(dataFile, "w");        
    assert(datalog);

    printf("SAMPLER: writing sampled data to: \n%s\n",dataFile);
    fprintf(datalog, "number of samples: %d\n", numberOfSamples);

    float x,y,z,rx,ry,rz,rw;
    
    for (int i=0;i<MAX_STATION_NUMBER;i++)
	{
        if (!validStations[i])
            continue;
        fprintf(datalog, "\nsamples from station %d: \n", i);
        for (int j=0; j< numberOfSamples; j++)
        {
	        samples[i].translations[j].getValue(x,y,z);
	        samples[i].rotations[j].getValue(rx,ry,rz,rw);
			fprintf(datalog, "T %3.20f %3.20f %3.20f \nR %1.8f %1.8f %1.8f %1.8f\n",
			                  x,y,z,rx,ry,rz,rw);
        }
    }

    fclose(datalog);
}


void
SoSampler::buttonCB(void *data, SoSensor *) 
{
    SbBool newState;
	SoSampler *sam = (SoSampler *)data;
    
    newState = Stb3DEventGenerator::getButtonField(
		           sam->buttonStation,sam->buttonNumber)->getValue() > 0; 
    
    // if state changed
    if (newState != sam->buttonState)
    {
        sam->buttonState = newState;
        // if button pressed
        if (newState == TRUE) {
            if (sam->sampleWhenTriggered)
                sam->sample();
            if (sam->triggerOnce)
			    sam->stopButtonTriggering();
            if (sam->buttonTriggered)
			    sam->buttonTriggered(sam->buttonTriggeredData);
        }
    }
}

//----------------------------------------------------------------------------
/** The destructor of the kit does nothing. */

SoSampler::~SoSampler()
{
    if (buttonSensor)
    {
        buttonSensor->detach();
        delete(buttonSensor);
    }
    if (dataSensor)
    {
        dataSensor->detach();
        delete(dataSensor);
    }
}

//----------------------------------------------------------------------------

//eof

