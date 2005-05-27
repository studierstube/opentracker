# Quick tool to convert csv file of a certain format into a bauml xml format
# Usage : csv2bauml.py input.csv output.xml
# @author Gerhard Reitmayr
# $Id: csv2bauml.py 3427 2004-07-14 12:20:49Z tamer $

from xml.dom import *
from xml.dom.ext import Print, PrettyPrint
import csv
import sys
import time

def main():
    di = DOMImplementation.getDOMImplementation()
    dt = di.createDocumentType('a','b','c')
    dc = di.createDocument('http://www.studierstube.org/bauml','SpatialContainer', dt )
    children = dc.createElement('children')
    dc.documentElement.appendChild(children)

    reader = csv.reader(file("e://studierstube//bin//apps//ocar//data//geobegr.csv"))
    for row in reader:
        if row[0] != 'X' :
            so = dc.createElement('SpatialObject')
            children.appendChild(so)
            so.setAttribute('id',row[2].replace(' ','_'))
            ann = dc.createElement('annotation')
            so.appendChild( ann )
            name = dc.createElement('Name')
            ann.appendChild( name )
            name.appendChild(dc.createTextNode(row[2]))
            kw = dc.createElement('Keywords')
            ann.appendChild(kw)
            kw.appendChild(dc.createTextNode(row[3]))
            pose = dc.createElement('pose')
            transform = dc.createElement('Transformation')
            transform.setAttribute('translation', row[0] + ' 0 ' + str(-float(row[1])));
            pose.appendChild(transform)
            so.appendChild(pose)
            
            
            

    PrettyPrint( dc, file('e:\\studierstube\\bin\\apps\\ocar\\data\\pyout.xml', 'w+'  ))    

if __name__ == "__main__":
    main()
