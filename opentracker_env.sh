#!/bin/sh

###
# Author:
#     Tamer Fahmy <tamer@ims.tuwien.ac.at>
#
# Description:
#     sets the environment for the libs Opentracker depends on
#
# Usage:
#     . ./opentracker_env.sh
#

STBROOT=/scratch/studierstube
OPENTRACKERROOT=$STBROOT/opentracker
ACEROOT=$STBROOT/ACE_wrappers
XERCESCROOT=$STBROOT/xerces
ARTOOLKITROOT=$STBROOT/artool

LD_LIBRARY_PATH=$ACEROOT/ace:$OPENTRACKERROOT/lib:$XERCESCROOT/lib:$ARTOOLKITROOT/lib

export LD_LIBRARY_PATH
