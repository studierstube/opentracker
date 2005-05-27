set APPDIR=../setups/soundtable
set NUMBER=4

cd ..\..\workspace\
workspace -uk %APPDIR%/UserKit.iv -a %APPDIR%/soundtable_%NUMBER%.iv -tr %APPDIR%/tracking.xml -cl locale%NUMBER% -jl locale%NUMBER% -smo 132.181.96.177 -time
cd %APPDIR%
pause
