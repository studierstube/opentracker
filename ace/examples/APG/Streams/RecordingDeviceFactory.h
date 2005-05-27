/* -*- C++ -*- */
// RecordingDeviceFactory.h,v 1.1 2004/01/01 21:01:00 shuston Exp

#ifndef RECORDING_DEVICE_FACTORY_H
#define RECORDING_DEVICE_FACTORY_H

class RecordingDevice;

/*
 * A factory class that creates an appropriate RecordingDevice
 * derivative based on command-line parameters.
 */
class RecordingDeviceFactory
{
public:

  // Instantiate the appropriate RecordingDevice implementation
  static RecordingDevice *instantiate (int argc, ACE_TCHAR *argv[]);
};

#endif /* RECORDING_DEVICE_FACTORY_H */

