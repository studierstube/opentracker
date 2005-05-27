Chromium Integration Demo
=========================

by Gerhard Reitmayr, 2003

This demo shows the ouput of an OpenGL application together with OpenIventor
driven geometry. It was build for the HITLabNZ opening in February 2003.

The application shows an aquarium in which a school of fish swims. The fish
are rendered by the OpenGL demo application atlantis.exe which is part of
the Chromium distribution. The aquarium is tracked by an ARToolkit marker 
showing the Y (similar to the Aeki model) and it is overlayed over the
video background.

Prerequisites :
* working ARToolkit with Studierstube
* Chromium 1.0.1 or 1.1 
* Chromium extension in src/extensions/Chromium

Use the crdemo.conf configuration file for the mothership. Then start 
everything in the following order :

1) start the mothership with the supplied crdemo.conf configuration and
atlantis as demo application :
> python crdemo.conf atlantis

2) start a crappfaker to run atlantis demo
> crappfaker

3) start the run.bat file from this directory
> run.bat
