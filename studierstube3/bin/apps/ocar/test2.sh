#! /bin/sh

OCARDIR=../apps/ocar
cd ../../workspace
./workspace -a $OCARDIR/ocar2.iv -uk $OCARDIR/config/monoUserHudPip2.iv -tr $OCARDIR/config/testHUD.xml -cl shared -jl shared
cd $OCARDIR
