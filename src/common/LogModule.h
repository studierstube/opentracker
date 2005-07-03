 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for LogModule.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section logmodule LogModule
 * The LogModule controls the output of log information that OpenTracker creates.
 * Possible targets are stderr and a given file. Logging information can be of
 * different verbosity levels including timestamp and host information and can 
 * be of different severity levels to only output important messages. 
 * The configuration element is called @c LogConfig and has the following attributes :
 * @li @c active (on|off) on controls if any log information should be output
 * @li @c stderr (on|off) on controls if logs should be send to stderr
 * @li @c file   (on|off) off controls if logs should be send to a file - does not work on WinCE
 * @li @c filename the name of the log file 
 * @li @c verbose (full|lite|std) std verbosity levels output information with timestamps and host info. See 
 *     ACE documentation for details.
 * @li @c level (debug|info|error) info severity levels, map somehow to the ACE levels
 * 
 * An example configuration element looks like this :
 * @verbatim
<LogConfig active="on" stderr="on" file="on" filename="log.txt" level="info" verbose="std"/>@endverbatim
 */

#ifndef _LOGMODULE_H
#define _LOGMODULE_H

#include "../OpenTracker.h"

namespace ot {

/** A simple module that modifies the ACE_LOG framework to provide some different logging options. 
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API LogModule : public Module
{
public:
    LogModule(void);

    void init( StringTable & attributes,  ConfigNode * localTree);
};

} // namespace

#endif // _LOGMODULE_H
