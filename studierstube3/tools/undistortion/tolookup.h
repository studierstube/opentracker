/* ===========================================================================
    no copyright
   ===========================================================================
    NAME:       tolookup
    TYPE:       c++ header
    PROJECT:    Studierstube
    CONTENT:    Converts scattered data to lookup-table, data declarations and
    VERSION:    2.1
   ===========================================================================
    AUTHORS:    Falk    Dobermann (fakl@iname.com)
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

=========================================================================== */

#ifndef TOLOOKUP_H
#define TOLOOKUP_H

#include <stdlib.h>
#include <math.h>

#ifndef WIN32
#include <stream.h>
#else
#include <iostream.h>
#endif

#include <stbapi/util/option.h>

#define LOOKUP(x,y,z) (table->dims[1] * table->dims[2] * (x) + table->dims[2] * (y) + (z))
#define LOOK_X(v) (div(v, table->dims[2]*table->dims[1]).quot % table->dims[0])
#define LOOK_Y(v) (div(v, table->dims[2]).quot % table->dims[1])
#define LOOK_Z(v) ((v) % table->dims[2])

typedef class in_data * p_in_data;

#ifndef WIN32
#  ifndef _BOOL
typedef int         bool;
#    define true    1
#    define false   0
#  endif //_BOOL
#endif // ifndef WIN32

class in_data
{
public:
  double measured[3];
  double data[3];
};

class out_data
{
public:
  double real_point[3];
  double measured_point[3];
  bool set;
};

class out_data_desc
{
public:
  double llb[3];			//lower left bottom point or look up cube
  double urt[3];			//upper right top of look up cube
  int dims[3];				//dimensions of cube int index
  double resolution;			//resolution of cube

  out_data *outData;
};

#endif
