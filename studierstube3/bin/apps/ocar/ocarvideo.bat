cls
make -f Makefile.ocar
%STBROOT%/bin/workspace/workspace -a ocar.iv -uk config/monoUserHudVideo.iv -tr config/absoluteVideo.xml %1 %2 %3 %4
pause