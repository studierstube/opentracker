rem set pathbak=%PATH%
rem set PATH=d:\studierstube.demo\lib;%PATH%
rem d:
rem cd D:\studierstube.demo\bin\workspace





cd ..\..\workspace
workspaced -uk ..\setups\showcase\config\ShowcaseUser.iv -tr ..\setups\showcase\config\showcase.xml -a ..\setups\showcase\showcase_stb.iv
pause





rem workspace  -a ../../bin/apps/chess/chess_stb.dll -ttype ot -ot ../apps/showcase/stb10.xml -uk ../apps/showcase/UserKit.iv 
rem set PATH=%pathbak%