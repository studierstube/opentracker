cls
make -f Makefile.ocar
%STBROOT%/bin/workspace/workspaced -a ocar.iv -uk config/monoUserHudPip.iv -tr config/absolute.xml %1 %2 %3 %4
pause