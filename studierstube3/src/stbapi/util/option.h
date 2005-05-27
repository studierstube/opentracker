/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * header file for commandline option parser
 *
 * @author Robert F. Tobler, Dieter Schmalstieg, Gerd Hesina
 *
 * $Id: option.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

// ===========================================================================
//  MANUAL:
//
//  DESCRIPTION
//      This is a command line parser that allows simple specification of all
//      options that may be put on the commandline.
//
//  EXAMPLE
//      Suppose we want to have a commandline utility that has the following
//      syntax:
//
//              tool [-h] [-n <int>] [-r <double>] [-s <string>] parameters
//
//      The options can also be used with their long names if they are
//      preceeded with '--' (e.g. --help, --number, --ratio, and --string),
//      the parameters can be intersperesed with the options, and the number
//      and ratio should get default values of 1 and 1.0.
//
//      To program that tool we would write the following code:
//
//              int main(int argc, char ** argv)
//              {
//
//                  OptionFlag          helpOpt  ("help",   "h");
//                  OptionLong          numberOpt("number", "n", 1);
//                  OptionDouble        ratioOpt ("ratio",  "r", 1.0);
//                  OptionString        stringOpt("string", "s");
//
//                  Option_errors.setState(ErrorHandler::useErrorNumber);
//                  if(!Option::parse(argc,argv))
//                  {
//                      // print usage
//                  }
//
//                  if (helpOpt.isSpecified()) { ... }
//
//                  int number = numberOpt.getValue();
//                  double ratio = ratioOpt.getValue();
//
//                  if (stringOpt.isSpecified())
//                  {
//                      const char * string = stringOpt.getValue();
//                      ...
//                  }
//
//                  //
//                  // After the call to Option::parse all options and
//                  // parameters of options are stripped from argv.
//                  // argc is adjusted accordingly.
//                  //
//
//                  for (int i = 1; i < argc; i++)
//                  {
//                      // use argv[i]
//                      ...
//                  }
//              }
//
//      Note, that for the number and ratio options it is not necessary to
//      check if they were specified, since they both have default values.
//
//  BUGS
//      No type checking on the specified parameter is done. Incompletely
//      parsed numbers are not reported.
//
//      If multiple Options with the same name are initialized, only the last
//      one will be used. (No checks to guard against that are performed.)
//
//  SEE ALSO
//      VEGA, BaseLibrary
// ===========================================================================

#ifndef _BASE_OPTION_H_
#define _BASE_OPTION_H_
#include <stbapi/stbapi.h>

extern "C++"
{

// ============================================================== class Option

class STBAPI_API Option
{
public:

    int isSpecified(
	) const;

    static int parse(
	int & argc,
	char **  argv
	);

    static void printUsage();

protected:
    Option(
        const char * newLongName,
        const char * newShortName,
        int newSpecified
        );

    virtual void setSpecified();
    virtual int isMissingParameter();
    virtual int takesParameter(const char * par);
    virtual const char * parameterInfo();
    
    static Option * findByLongName(const char * name);
    static Option * findByShortName(const char * name);


    int specified;
    const char * shortName;
    const char * longName;
    Option * next;

    static Option * optionList;
};

// ========================================================== class OptionFlag

class STBAPI_API OptionFlag :
    public Option
{
public:

    OptionFlag(
        const char * newLongName,
        const char * newShortName
        );

protected:
    virtual const char * parameterInfo();
};

// ===================================================== class OptionParameter

class STBAPI_API OptionParameter :
    public Option
{
protected:
    OptionParameter(
        const char * newLongName,
        const char * newShortName,
        int hasDefault
        );

    virtual void setSpecified();
    virtual int isMissingParameter();
    virtual void setParameter(const char * par);
    virtual int takesParameter(const char * par);

    int parameter;
};

// ======================================================== class OptionString

class STBAPI_API OptionString :
    public OptionParameter
{
public:

    OptionString(
        const char * newLongName,
        const char * newShortName
        );
    OptionString(
        const char * newLongName,
        const char * newShortName,
        const char * defaultValue
        );

    const char * getValue() const;

protected:
    virtual void setParameter(const char * par);
    virtual const char * parameterInfo();

    const char * value;
};

// ========================================================== class OptionLong

class STBAPI_API OptionLong :
    public OptionParameter
{
public:

    OptionLong(
        const char * newLongName,
        const char * newShortName
        );
    OptionLong(
        const char * newLongName,
        const char * newShortName,
        int defaultValue
        );

    long getValue() const;

protected:
    virtual void setParameter(const char * par);
    virtual const char * parameterInfo();

    long value;
};

// ======================================================== class OptionDouble

class STBAPI_API OptionDouble :
    public OptionParameter
{
public:

    OptionDouble(
        const char * newLongName,
        const char * newShortName
        );
    OptionDouble(
        const char * newLongName,
        const char * newShortName,
        double defaultValue
        );

    double getValue() const;

protected:
    virtual void setParameter(const char * par);
    virtual const char * parameterInfo();

    double value;
};

} // extern "C++"

// ===========================================================================
#endif//_BASE_OPTION_H_

