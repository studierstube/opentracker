#! /bin/sh

OCARDIR=../apps/ocar
export PATH=`pwd`:$PATH
make -f Makefile.ocar
../../workspace/workspace -a $OCARDIR/ocar.iv -uk $OCARDIR/config/monoUserHudPip.iv -tr $OCARDIR/config/absolute.xml $*
