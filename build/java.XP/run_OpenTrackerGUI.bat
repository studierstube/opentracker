@echo off

rem 
rem Author: Michael Kalkusch
rem 
rem last change: 07-03-2006
rem
rem porpose: run OpenTracker GUI
rem
rem settings: 
rem    bin_path ........ point to folder containing *.class files 
rem    build_path ...... point to folder containing this batch file
rem    jar_path ........ point to folder the files jgraph.jar and jgrapht.jar are
rem

setlocal 

set bin_path=..\..\bin\bin_class
set build_path=..\..\build\java.XP
set jar_path=..\..\src


cd %bin_path%
java -classpath .;jgraph.jar;jgrapht.jar org.studierstube.opentracker.JGraphOpenTrackerEditor 
cd %build_path%

endlocal 
