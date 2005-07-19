======================================================================
The DLL included in this distribution provides an easy to use API 
that can detect, configure and retrieve data from all models of 
InterSense trackers. A sample application is provided to illustrate 
the use of most functions. See isense.h for explanations and function 
prototypes. Significant portion of the API and the sample application 
is not relevant when used with InterTrax. 


COMPATIBILITY
======================================================================
Windows version supports all InterSense trackers. Irix, Linux and 
Mac OS X versions are limited to serial port models. USB support on 
Linux is planned for future releases.


SERIAL PORT MAPPING (UNIX)
======================================================================
Device names for serial ports vary on different versions of Unix. 
The library uses default names for each of the supported operating 
system. That, however, is not always sufficient, particularly when 
USB to Serial converters are used. To solve this, a configuration 
file isports.ini can be used to specify port number to device string 
mapping. The included sample file was produced for Mac OS X and 
contains lines for Keyspan and InterSense USB converters. Please
check your system for specific device names. This option is not
supported on Irix at this time.


INSTALATION
======================================================================

1. Windows9x
   Copy isense.dll to C:\Windows\System

2. Windows NT/2000/XP
   Login as Administrator
   Copy isense.dll to C:\WINNT\SYSTEM32

3. Linux
   Login as root
   Copy Linux/libisense.so to /lib directory

4. IRIX
   Login as root
   Copy Irix/libisense.so to /usr/lib32 directory

5. Mac OS X
   Login as root
   Copy libisense.dylib to /usr/lib directory


CONTACT INFORMATION
======================================================================

InterSense Inc.
1 North Avenue
Burlington, MA
01803 USA

Tel. 781-270-0090
Fax. 781-229-8995

www.isense.com