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
 * header file for SessionVisitor
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SessionVisitor.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SESSIONVISITOR_H_
#define _SESSIONVISITOR_H_

class StartApplicationMessage;
class SetApplicationModeMessage;
class StopApplicationMessage;
class AddUserMessage;
class RemoveUserMessage;
class JoinLocaleMessage;
class LeaveLocaleMessage;
class SetDIVParamMessage;
class UserKitMessage;

/** This class brings together the whole protocol for the session manager.
 * It defines the visitor class interface to act upon the various messages and defines
 * the message ids for each message. The following messages are defined :
 *
 * @li @b START_APPLICATION implemented in StartApplicationMessage. Tells
 *     the manager or any host in a certain locale that a new application
 *     was started.
 * @li @b STOP_APPLICATION implemented in StopApplicationMessage. Tells 
 *     the manager or any host in a certain locale that an application
 *     was stopped.
 * @li @b SET_APPLICATION_MODE implemented in SetApplicationModeMessage. 
 *     Tells a host to run a certain application in master or slave mode.
 * @li @b ADD_USER implemented in AddUserMessage. Tells the manager or
 *     any host to add a user to a certain locale.
 * @li @b REMOVE_USER implemented in RemoveUserMessage. Tells the manager
 *     or any host to remove a user from a certain locale.
 * @li @b JOIN_LOCALE implemented in JoinLocaleMessage. Tells the manager
 *     that a host joined a certain locale.
 * @li @b LEAVE_LOCALE implemented in LeaveLocaleMessage. Tells the manager
 *     that a host left a certain locale.
 * @li @b SET_DIV_PARAM implemented in SetDIVParamMessage. Tells a host
 *     whether to start or stop sharing an application and the necessary
 *     DIV parameters to do so.
 * @li @b USERKIT_PARAM a whole subtree of Messages derived from UserKitMessage.
 *     They are documented in UserKitMessageVisitor and are used to send
 *     special user related messages between hosts.
 *
 * A class that handles these messages can implement this interface. Then it
 * only needs to call SessionMessage::execute with the visitor as parameter.
 * The message objects will then call in turn the correct execute method of
 * the visitor interface.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class SessionVisitor  
{
public:

    enum MessageId {
        START_APPLICATION,
        STOP_APPLICATION,
        SET_APPLICATION_MODE,
        ADD_USER,
        REMOVE_USER,
        JOIN_LOCALE,
        LEAVE_LOCALE,
        SET_DIV_PARAM,
        USERKIT_PARAM
    };

    virtual void execute( StartApplicationMessage & message ) = 0;
    virtual void execute( SetApplicationModeMessage & message ) = 0;
    virtual void execute( StopApplicationMessage & message ) = 0;
    virtual void execute( AddUserMessage & message ) = 0;
    virtual void execute( RemoveUserMessage & message ) = 0;
    virtual void execute( JoinLocaleMessage & message ) = 0;
    virtual void execute( LeaveLocaleMessage & message ) = 0;
    virtual void execute( SetDIVParamMessage & message ) = 0;
    virtual void execute( UserKitMessage & message ) = 0;
};

#endif // _SESSIONVISITOR_H_
