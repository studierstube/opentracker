# a simple script to flip artoolkit pattern files vertically.
# it accepts a list of files and will convert each one in place
# overwriting the old version.
# Usage : python mirrorpatterns.py file1 file2 ...
# @author Gerhard Reitmayr
# $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/tools/mirrorpatterns.py,v 1.1 2003/05/21 14:17:34 reitmayr Exp $

import sys

def flipFile( input ) :
    fin = file( input );
    lines = fin.readlines()
    size = lines[0].strip().count(' ')+1
    fin.close();
    
    fout = file( input, 'w' );

    for images in [0,3,2,1] :
	for components in range(3) :
		for count in range(size) :
			fout.write( lines[images*(size*3+1)+components*size+(size-count-1)] )
	fout.write('\n');

    fout.close()

def main( args ) :
    for arg in args[1:] :
        print 'doing file ',arg
        flipFile( arg )

if __name__ == "__main__":
    main(sys.argv)
