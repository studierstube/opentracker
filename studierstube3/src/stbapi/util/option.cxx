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
 * commandline option parser
 *
 * @author Robert F. Tobler, Dieter Schmalstieg, Gerd Hesina
 *
 * This file has been created in 1994 and therefore put under Studierstube
 * "Denkmalschutz"! Don't you ever dare to remove it!!! --tamer, gerhard, flo
 *
 * $Id: option.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include "option.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

using namespace std;

// ===========================================================================

Option * Option::optionList = 0;

Option::Option(
        const char * newLongName,
        const char * newShortName,
        int newSpecified
        )
:
    specified(newSpecified),
    shortName(newShortName),
    longName(newLongName),
    next(optionList)
{
   if (((const void *)0) != (findByLongName(longName)))
    {
	cerr << "Option with long name: '" << longName <<  "' already exists\n";
    exit(-1);
    }
    if (((const void *)0) != (findByShortName(shortName)))
    {
	cerr << "Option with short name: '" << shortName <<  "' already exists\n";
    exit(-1);
    }
    optionList = this;
}

int Option::isSpecified() const
{
    return specified;
}

Option * Option::findByLongName(const char * name)
{
    for (Option * optionPtr = optionList;
         optionPtr;
         optionPtr = optionPtr->next)
    {
        if (strcmp(name,optionPtr->longName) == 0)
            return optionPtr;
    }
    return (0);
}

void Option::printUsage()
{
    for (Option * optionPtr = optionList;
         optionPtr;
         optionPtr = optionPtr->next)
    {
        cout << optionPtr->parameterInfo() <<" --" << optionPtr->longName
             << " -" << optionPtr->shortName << endl;
    }
}

Option * Option::findByShortName(const char * name)
{
    for (Option * optionPtr = optionList;
         optionPtr;
         optionPtr = optionPtr->next)
    {
        if (strcmp(name,optionPtr->shortName) == 0)
            return optionPtr;
    }
    return (0);
}

void Option::setSpecified()
{
    specified = TRUE;
}

int Option::isMissingParameter()
{
    return 0;
}

int Option::takesParameter(const char *)
{
    return FALSE;
}

const char * Option::parameterInfo()
{
    return "";
}

int Option::parse(
        int & argc,
        char **  argv
        )
{
    int i;
    Option * optionPtr = 0;
    int numberOfOptionsAndParameters = 0;
    /*
    TR_CODE
    (
        TR_MESSAGE(Option_tracer,"BEFORE PARSING:");
        TR_MESSAGE(Option_tracer,"  argc = " << argc);
        for (i = 1; i < argc; i++)
            TR_MESSAGE(Option_tracer,"  argv[" << i << "] = " << argv[i]);
    );
    */
    for (i = 1; i < argc; i++)
    {
        if (numberOfOptionsAndParameters > 0)
            argv[i - numberOfOptionsAndParameters] = argv[i];

        if (argv[i][0] != '-' ||
	    (optionPtr && (optionPtr->isMissingParameter())))
        {
            if (optionPtr &&
                (optionPtr->takesParameter(argv[i])))
                numberOfOptionsAndParameters++;
        }
        else
        {
            if (argv[i][1] == '-')
            {
                optionPtr = findByLongName(argv[i] + 2);
            }
            else
            {
                optionPtr = findByShortName(argv[i] + 1);
            }
            if (! optionPtr)
            {
                cerr <<      "unknown option '" << argv[i] << "'\n";
                return FALSE;
            }
            if ((optionPtr->specified))
            {
                cerr <<   "option '" << argv[i] << "' specified twice\n";
                return FALSE;
            }
            optionPtr->setSpecified();
            numberOfOptionsAndParameters++;
        }
    }
    if (optionPtr && optionPtr->isMissingParameter())
    {
        cerr <<  optionPtr->longName << " missing option parameter\n";
        return FALSE;
    }

    argc -= numberOfOptionsAndParameters;
    return TRUE;
}

OptionFlag::OptionFlag(
        const char * newLongName,
        const char * newShortName
        )
:
    Option(newLongName,newShortName,FALSE)
{
  //    TR_FUN2(Option_tracer,OptionFlag,newLongName,newShortName);
}

const char * OptionFlag::parameterInfo()
{
    return "FLAG";
}

OptionParameter::OptionParameter(
        const char * newLongName,
        const char * newShortName,
        int hasDefault
        )
:
    Option(newLongName,newShortName,FALSE),
    parameter(hasDefault)
{}

void OptionParameter::setSpecified()
{
    Option::setSpecified();
    parameter = FALSE;
}

int OptionParameter::takesParameter(const char * par)
{
    if (!(parameter))
    {
        setParameter(par);      
        parameter = TRUE;
        return TRUE;
    }
    return FALSE;
}

void OptionParameter::setParameter(const char *)
{
    cerr <<"should be pure virtual\n";
}

int OptionParameter::isMissingParameter()
{
    if (!(parameter))
        return 1;
    else
	return 0;
}

OptionString::OptionString(
        const char * newLongName,
        const char * newShortName
        )
:
    OptionParameter(newLongName,newShortName, FALSE), value(0)
{
  //    TR_FUN2(Option_tracer,OptionString,newLongName,newShortName);
}

OptionString::OptionString(
        const char * newLongName,
        const char * newShortName,
        const char * defaultValue
        )
:
    OptionParameter(newLongName,newShortName, TRUE), value(defaultValue)
{
  //  TR_FUN3(Option_tracer,OptionString,newLongName,newShortName,
  //           defaultValue);
}

const char * OptionString::getValue() const { return value; }

void OptionString::setParameter(const char * par) { value = par; }

const char * OptionString::parameterInfo()
{
    return "STRING";
}

OptionLong::OptionLong(
        const char * newLongName,
        const char * newShortName
        )
:
    OptionParameter(newLongName,newShortName, FALSE), value(0)
{
  //    TR_FUN2(Option_tracer,OptionLong,newLongName,newShortName);
}

OptionLong::OptionLong(
        const char * newLongName,
        const char * newShortName,
        int defaultValue
        )
:
    OptionParameter(newLongName,newShortName, TRUE), value(defaultValue)
{
  //    TR_FUN3(Option_tracer,OptionLong,newLongName,newShortName,
  //            defaultValue);
}

long OptionLong::getValue() const { return value; }

void OptionLong::setParameter(const char * par) { value = atol(par); }

const char * OptionLong::parameterInfo()
{
    return "LONG";
}

OptionDouble::OptionDouble(
        const char * newLongName,
        const char * newShortName
        )
:
    OptionParameter(newLongName,newShortName, FALSE), value(0)
{
  //    TR_FUN2(Option_tracer,OptionDouble,newLongName,newShortName);
}

OptionDouble::OptionDouble(
        const char * newLongName,
        const char * newShortName,
        double defaultValue
        )
:
    OptionParameter(newLongName,newShortName, TRUE), value(defaultValue)
{
  
  //  TR_FUN3(Option_tracer,OptionDouble,newLongName,newShortName, defaultValue);

}

double OptionDouble::getValue() const { return value; }

void OptionDouble::setParameter(const char * par) { value = atof(par); }

const char * OptionDouble::parameterInfo()
{
    return "DOUBLE";
}

// ===========================================================================

