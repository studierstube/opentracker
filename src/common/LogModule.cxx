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
/** source file for LogModule.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <stdlib.h>
#include "LogModule.h"

#ifndef _WIN32_WCE
#include <fstream>
#endif

#include <ace/Log_Msg.h>

namespace ot {

LogModule::LogModule(void)
{
}

void LogModule::init( StringTable & attributes,  ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    
    if( attributes.get("active").compare("off") == 0 )
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT);
    else
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SILENT);

    if( attributes.get("stderr").compare("on") == 0 )
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR);
    else
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR);

    if( attributes.get("verbose").compare("full") == 0 )
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::VERBOSE);
    else if( attributes.get("verbose").compare("lite") == 0 )
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::VERBOSE_LITE);

    if( attributes.get("level").compare("info") == 0 )
    {
        ACE_LOG_MSG->priority_mask( LM_INFO | LM_NOTICE | LM_WARNING | LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY, 
                                    ACE_Log_Msg::PROCESS );
    }
    else if( attributes.get("level").compare("error") == 0 )
    {
        ACE_LOG_MSG->priority_mask( LM_WARNING | LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY,
                                    ACE_Log_Msg::PROCESS );
    }

#ifndef _WIN32_WCE
    if( attributes.get("file").compare("on") == 0 && attributes.containsKey("filename"))
    {
        ACE_OSTREAM_TYPE * file = new std::ofstream( attributes.get("filename").c_str());
        ACE_LOG_MSG->msg_ostream( file, 1 );
        ACE_LOG_MSG->set_flags( ACE_Log_Msg::OSTREAM );
    }
#endif
}

} // namespace ot
