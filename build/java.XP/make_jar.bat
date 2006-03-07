@echo off

rem 
rem Author: Michael Kalkusch
rem 
rem last change: 07-03-2006
rem
rem porpose: create jar-file for OpenTracker GUI
rem
rem settings: 
rem    bin_path ........ point to folder containing *.class files 
rem    build_path ...... point to folder containing this batch file
rem    jar_name ........ name of the jar file
rem

setlocal 

set build_path=..\..\build\java.XP
set bin_path=..\..\bin\bin_class
set src_path=..\..\src_java
set jar_name=OpenTrackerGraph_GUI.jar

mkdir %bin_path%\
copy MANIFEST.MF %bin_path%\ot_MANIFEST.MF
copy %src_path%\jgraph.jar %bin_path%\
copy %src_path%\jgrapht.jar %bin_path%\
cd  %bin_path%
echo on

dir
pause

jar -xf jgraph.jar 
jar -xf jgrapht.jar

rmdir /S /Q META-INF

dir 
pause
jar cfm %jar_name% ot_MANIFEST.MF org/studierstube/opentracker org/_3pq/jgrapht org/jgraph

@echo off
rmdir /S /Q META-INF
del jgraph.jar 
del jgrapht.jar 
del ot_MANIFEST.MF
cd %build_path%


endlocal 

