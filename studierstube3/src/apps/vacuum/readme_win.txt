What to add for H323 support in Studierstube when using Microsoft Visual C++ 6.0?
---------------------------------------------------------------------------------

C/C++:

Preprocessor definitions: USE_H323

Additional include directories: $(H323ROOT)\pwlib\include\pwlib\mswin,$(H323ROOT)\pwlib\include\ptlib\msos,$(H323ROOT)\pwlib\include,$(H323ROOT)\openh323\include

======================

Link:

Object/library modules: 
    - Debug: ptlibd.lib pwlibd.lib openh323d.lib
    - Release: ptlib.lib pwlib.lib openh323.lib

Additional library path: $(H323ROOT)\pwlib\lib,$(H323ROOT)\openh323\lib
