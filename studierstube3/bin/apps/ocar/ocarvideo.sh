#! /bin/sh

OCARDIR=../apps/ocar
cd ../../workspace
./workspace -a $OCARDIR/ocar.iv -uk $OCARDIR/config/monoUserHudVideo.iv -tr $OCARDIR/config/absoluteVideo.xml $*
cd ../apps/ocar
