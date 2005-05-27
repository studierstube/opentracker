set APPDIR=../apps/crdemo

cd ..\..\workspace\
workspaced -uk %APPDIR%/UserKit.iv -a %APPDIR%/crdemo.iv -tr %APPDIR%/tracking.xml  -time
cd %APPDIR%
pause
