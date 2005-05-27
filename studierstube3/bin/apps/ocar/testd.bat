cls
make -f Makefile.ocar
%STBROOT%\bin\workspace\workspaced -a ocar.iv -uk config/monoUserHudPip.iv -tr config/testHUD.xml %1 %2 %3
