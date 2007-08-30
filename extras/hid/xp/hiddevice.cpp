//cWiimote 0.2 by Kevin Forbes (http://simulatedcomicproduct.com)
//This code is public domain, and comes with no warranty. The user takes full responsibility for anything that happens as a result from using this code.

//  This was based in part on Alan Macek <www.alanmacek.com>'s USB interface library

#include "hiddevice.h"

#ifndef WIN32 // linux

///linux code 
#else // win code

#include <windows.h>

namespace ot {

extern "C" 
{
	#include "hidsdi.h"
	#include <Setupapi.h>
}
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")


cHIDDevice::cHIDDevice() : mConnected(false), mHandle(NULL), mEvent(NULL)
{
	
}


cHIDDevice::~cHIDDevice()
{
	if (mConnected)
	{
		Disconnect();
	}
}

bool cHIDDevice::Disconnect()
{
	bool retval = false;
	if (mConnected)
	{
		retval = (CloseHandle(mHandle) == TRUE && CloseHandle(mEvent) == TRUE);
	
		mConnected = false;
	}

	return retval;
}

bool cHIDDevice::Connect(unsigned short device_id, unsigned short vendor_id, int index)
{
	if (mConnected)
	{
		if (!Disconnect())
		{
			return false;
		}
	}

	bool retval = false;
	HANDLE handle = NULL;
	int dev_index = 0;
	int matching_devs_found = 0;
	for (;;)
	{
		HIDD_ATTRIBUTES attrib;

		if (!OpenDevice(dev_index) ||
			!HidD_GetAttributes (mHandle, &attrib))
		{
			break;
		}
		if (attrib.ProductID == device_id &&
			attrib.VendorID == vendor_id)
		{
			if (matching_devs_found == index)
			{
				mConnected = true;
				break;
			}
			matching_devs_found++;
		}
		CloseHandle(mHandle);
		dev_index++;
	}
	return mConnected;
}

bool cHIDDevice::OpenDevice(int index)
{
	bool retval = false;

	GUID guid;
    HidD_GetHidGuid (&guid);
	HDEVINFO devinfo = SetupDiGetClassDevs(&guid,NULL,NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	SP_DEVICE_INTERFACE_DATA dev_int_data;
	dev_int_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	if (SetupDiEnumDeviceInterfaces(devinfo, NULL, &guid, index, &dev_int_data) == TRUE)
	{
		DWORD size;
		SetupDiGetDeviceInterfaceDetail (devinfo, &dev_int_data, NULL, 0, &size,0);
		
		PSP_INTERFACE_DEVICE_DETAIL_DATA detail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(size);
		detail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		
		if (SetupDiGetDeviceInterfaceDetail(devinfo, &dev_int_data, detail, size, NULL, NULL))
		{
			mHandle = CreateFile (detail->DevicePath, 
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING, 
								FILE_FLAG_OVERLAPPED, 
								NULL);
			mEvent = CreateEventA(NULL, TRUE, TRUE, "");
			mOverlapped.Offset = 0;
			mOverlapped.OffsetHigh = 0;
			mOverlapped.hEvent = mEvent;
			retval = true;
		}
		free(detail);
	}
	
	SetupDiDestroyDeviceInfoList (devinfo);
	return retval;
}

bool cHIDDevice::WriteToDevice(unsigned const char * buffer, int num_bytes)
{
	bool retval = false;
	if (mConnected)
	{
		DWORD bytes_written;
		retval = (WriteFile( mHandle, buffer,num_bytes,&bytes_written,&mOverlapped) == TRUE); 
		retval = retval && bytes_written == num_bytes;
	}
	return retval;

}

bool cHIDDevice::ReadFromDevice(unsigned const char * buffer, int max_bytes, int & bytes_read, int timeout)
{
	bool retval = false;
	if (mConnected)
	{
		ReadFile( mHandle, (LPVOID)buffer,max_bytes,(LPDWORD)&bytes_read,&mOverlapped); 
		DWORD Result = WaitForSingleObject(mEvent, timeout);
		if (Result == WAIT_OBJECT_0) 
		{		
			retval = true;
		}
		else 
		{
			CancelIo(mHandle);			
		}
		ResetEvent(mEvent);
	}
	return retval;
}

} // end namespace ot

#endif //end win code
