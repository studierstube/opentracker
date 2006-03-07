@echo off

rem 
rem Author: Michael Kalkusch
rem 
rem last change: 07-03-2006
rem
rem porpose: run OpenTracker GUI from jar-file
rem
rem settings: 
rem    bin_path ........ point to folder containing *.class files 
rem    build_path ...... point to folder containing this batch file
rem    jar_name ........ name of the jar file
rem

setlocal 

set bin_path=..\..\bin\bin_class
set build_path=..\..\build\java.XP
set jar_name=OpenTrackerGraph_GUI.jar


cd %bin_path%
java -jar %jar_name% 
cd %build_path%

endlocal 
