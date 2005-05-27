Step 1: Edit the configuration files to include the right camera parameter files.
---------------------------------------------------------------------------------

ARToolkitConfig nodes in all xml files
stereoCameraKit nodes in all *User.iv files



Step 2: Starting the application:
---------------------------------

There are 2 possible setups: the standalone setup, using 2 mobile setups tracking each other, and the "table" setup, with 2 mobile setups plus a common reference locale, for example the table with overhead tracking.


Starting the Standalone Setup:
------------------------------
On the primary setup, start in this order:

sessionmanager.bat
start_user1.bat

On the other setup, you have to configure the ip of the the first setup in start_user2.bat (-smo <ip-address of setup 1>). Then start

start_user2.bat


Starting the Table Setup:
-------------------------
On the tracking host for the table, start in this order:

sessionmanager.bat
start_table.bat

On mobile setup #1, edit start_table_user1.bat (-smo <ip-address of table host>) and start

start_table_user1.bat

On mobile setup #2, edit start_table_user2.bat (-smo <ip-address of table host>) and start

start_table_user2.bat



Table 1: Standard Station Numbers:
----------------------------------

0xx:	Local devices

000:	Camera
001:	PIP
002:	Primary Hand Pointer
003:	Secondary Hand Pointer
004:	PIP Pointer
00(1+n):Pointing Device n

1xx:	Markers used in Application

2xx:	Locales

200:	Locale of Table
201:	Local of User 1
...



Table 2: Standard Network & Port Numbers:
-----------------------------------------

Multicast-Address 224.100.200.100 used for multicasting tracking data

Port 12344:		Tracking Table
Port 12345:		User 1
Port 12346:		User 2
Port 1234(4+n): User n
