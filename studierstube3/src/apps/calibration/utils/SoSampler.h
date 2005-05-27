#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

const int MAX_STATION_NUMBER=100;
const int MAX_BUTTONS=8;

class SoSampler {
public:
    SoSampler();
    ~SoSampler();
    void setStationsToSample(const SoMFInt32 &stations);
    void sample();
    void clearAllSamples();
    void stepBack();
    void logSampledData(char *fileName, SbBool append = FALSE);
    void startContinousSampling(void(*func)(void *) = NULL, void *data = NULL);
    void stopContinousSampling();
    
    void initButtonTriggering(int station, 
                              int buttonNumber,
                              SbBool triggerOnce = TRUE,
                              SbBool sampleWhenTriggered = TRUE,
                              void(*func)(void *) = NULL,
                              void *data = NULL);                             
    
    void stopButtonTriggering();
    void startButtonTriggering();

    struct __samples
    {
        SoMFVec3f    translations;
        SoMFRotation rotations;
    } samples[MAX_STATION_NUMBER];

    
private:
    int getFirstValidStation();
    void clearSamples(int index);
    int numberOfSamples;
    SbBool validStations[MAX_STATION_NUMBER];    
	static void samplingCB(void *,SoSensor *);
	static void buttonCB(void *,SoSensor *);
	SoFieldSensor *dataSensor;
	SoFieldSensor *buttonSensor;
	int buttonStation;
	int buttonNumber;
    int buttonState;
    SbBool triggerOnce;
    SbBool sampleWhenTriggered;
	void (*buttonTriggered)(void *);
    void (*continousSamplingCB)(void *);
	void *buttonTriggeredData;
    void *continousSamplingCBData;
};
