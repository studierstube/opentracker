//cWiimote 0.2 by Kevin Forbes (http://simulatedcomicproduct.com)
//This code is public domain, and comes with no warranty. The user takes full responsibility for anything that happens as a result from using this code.

#ifndef HIDDEVICE_H
#define HIDDEVICE_H

#ifndef WIN32 // linux

///linux code 
#else // win code

#include <WinSock2.h> // should be befor winsock.h otherwise they conflict!
#include <windows.h>


namespace ot {
   class cHIDDevice
   {
   public:
      cHIDDevice();
      ~cHIDDevice();
      bool Disconnect();
      bool Connect(unsigned short device_id, unsigned short vendor_id, int index=0);
      bool IsConnected() const {return mConnected;}

      bool WriteToDevice(unsigned const char * buffer, int num_bytes);
      bool ReadFromDevice(unsigned const char * buffer, int max_bytes, int & bytes_read, int timeout=50);
   private:

      bool OpenDevice(int index);

      HANDLE mHandle;
      HANDLE mEvent;
      OVERLAPPED mOverlapped;
      bool mConnected;
   };

} // end namespace ot

#endif //end win code


#endif //end HIDDEVICE_H
