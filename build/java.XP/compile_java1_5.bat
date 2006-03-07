@echo off

rem 
rem Author: Michael Kalkusch
rem 
rem last change: 07-03-2006
rem
rem porpose: create class-file for OpenTracker GUI
rem
rem settings: 
rem    bin_path ........ point to folder were *.class files should be placed
rem    build_path ...... point to folder containing this batch file
rem    src_path ........ point to folder were *.java files are
rem
rem Note: in order to compile unpack the src from jgraph_src.zip and jgrapht_src.zip to src_java
rem

setlocal 

set bin_path=..\bin\bin_class
set build_path=..\build\java.XP
set src_path=..\..\src_java


cd  %src_path%
mkdir %bin_path%\
copy jgraph.jar %bin_path%\*.*  > MANIFEST_copy_info.txt
copy jgrapht.jar %bin_path%\*.* > MANIFEST_copy_info.txt
echo on
javac -d %bin_path% -target 1.5 -classpath .;jgraph.jar;jgrapht.jar org\studierstube\opentracker\JGraphOpenTrackerEditor.java
@echo off

del MANIFEST_copy_info.txt
cd %build_path%

endlocal 