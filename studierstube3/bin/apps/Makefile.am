## Makefile.am -- Process this file with automake to produce Makefile.in

if USE_VTK
vtkdir = vtk
else
vtkdir =
endif

if USE_BOOST
ocardir = ocar
else
ocardir = 
endif

if USE_ACIS
construct3ddir = construct3d
else
construct3ddir =
endif

if USE_SIMVOLEON
simvoleon = vacuum
else
simvoleon = 
endif


SUBDIRS = APRIL animal animalvc chess md2_art stuberena vampire $(ocardir) $(simvoleon) $(vtkdir) $(construct3ddir)
