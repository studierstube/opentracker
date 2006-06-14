/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for TargusModule.
 *
 * @author Bernhard Reitinger
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this is a linux implementation
#ifndef WIN32
#include "TargusModule.h"

#include "TargusSource.h"

#ifdef USE_TARGUSMODULE

#include <cstdio>
#include <iostream>
#include <cassert>

#include <ace/Log_Msg.h>

#define OT_NAMESPACE_BEGIN namespace ot {
#define OT_NAMESPACE_END }

OT_NAMESPACE_BEGIN

void end_it_all (Display * d);
static int *null_X_error (Display *, XErrorEvent *);

// Destructor method
TargusModule::~TargusModule() {
    nodes.clear();
}

// This method is called to construct a new Node.
Node * TargusModule::createNode( const std::string& name, StringTable& attributes) {

    if( name.compare("TargusSource") == 0 ) {       
        TargusSource * source = new TargusSource;
        source->event.getButton() = 0;
        source->event.getConfidence() = 1.0f;
        nodes.push_back( source );
        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Build TargusSource node\n")));
        initialized = 1;
        return source;
    }
    return NULL;
}

// opens X keyboard library (xbindkeys)
void TargusModule::start() {
    if( isInitialized() == 1 ) {
      
        char *display_name = XDisplayName(NULL);
        int screen;

        display = XOpenDisplay(display_name);
        if (!display) {
            fprintf (stderr,
                     "Could not open display, check shell DISPLAY variable, and export or setenv it!\n");
            exit (1);
        }
      
        XAllowEvents (display, AsyncBoth, CurrentTime);
      
        for (screen = 0; screen < ScreenCount(display); screen++) {
            XSelectInput (display, RootWindow (display, screen),
                          KeyPressMask | KeyReleaseMask | PointerMotionMask);
        }
      
        // get the XKeys object for handling registring keyevents
        //       xkeys = new XKeys("/mount/homes/breiting/.xbindkeysrc");
        xkeys = new XKeys("/root/.xbindkeysrc");
        assert(xkeys);
        xkeys->showKeys(display);
      
        xkeys->grab_keys(display);

        //       get_offending_modifiers (d);

        //   grab_keys (d);

        //   event_loop (d);


    }
}

// closes CyberMouse library
void TargusModule::close() {
    if( isInitialized() == 1) {

        end_it_all (display);

        delete xkeys;
    }
}

// pushes events into the tracker tree.
void TargusModule::pushEvent() {
    using namespace std;

    unsigned short buttons=0;

    XEvent e;
    int i;
    bool verbose = true;
    XSetErrorHandler ((XErrorHandler) null_X_error);
   
    if ( isInitialized() == 1 ) {
        for ( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ ) {
            TargusSource * source = (TargusSource *)(*it);
	 
            XNextEvent (display, &e);

            // 	 cerr << "event = " << e.type << endl;

            // 	 if (e.xkey.keycode == 130) {
            // 	       buttons = buttons | 0x01;
            // 	 } else if (e.xkey.keycode == 176) {
            // 	       buttons = buttons | 0x02;
            // 	 }

            // 	 source->event.getButton() = buttons;

            switch (e.type) {
                case KeyPress:
                    if (verbose) {
                        printf ("Key press !\n");
                        printf ("e.xkey.keycode=%d\n", e.xkey.keycode);
                        printf ("e.xkey.state=%d\n", e.xkey.state);
                    }

                    // 	    event.getButton() = states[B1] | (states[B2] << 1) | (states[B3] << 2) | (states[B4] << 3);

                    if (e.xkey.keycode == 130) {
                        source->event.getButton() = 1;  // 00000001
                    } else if(e.xkey.keycode == 162) {
                        source->event.getButton() = 2;  // 00000010
                    } else if(e.xkey.keycode == 174) {
                        source->event.getButton() = 4;  // 00000100
                    } else  {
                        source->event.getButton() = 1;
                    }

                    source->event.timeStamp();
                    source->updateObservers(source->event);

                    e.xkey.state &= ~(xkeys->numlock_mask | xkeys->capslock_mask | xkeys->scrolllock_mask);

                    for (i = 0; i < xkeys->nb_keys; i++) {
                        if (xkeys->keys[i].type == XKeys::SYM && xkeys->keys[i].event_type == XKeys::PRESS) {
                            if (e.xkey.keycode == XKeysymToKeycode (display, xkeys->keys[i].key.sym)
                                && e.xkey.state == xkeys->keys[i].modifier) {

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		     adjust_display(&e.xany);
                                // 		     start_command_key (&xkeys->keys[i]);
                            }
                        } else if (xkeys->keys[i].type == XKeys::CODE && xkeys->keys[i].event_type == XKeys::PRESS) {
                            if (e.xkey.keycode == xkeys->keys[i].key.code
                                && e.xkey.state == xkeys->keys[i].modifier) {

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		      adjust_display(&e.xany);
                                // 		      start_command_key (&xkeys->keys[i]);
                            }
                        }
                    }
                    break;

                case KeyRelease:
                    if (verbose) {
                        printf ("Key release !\n");
                        printf ("e.xkey.keycode=%d\n", e.xkey.keycode);
                        printf ("e.xkey.state=%d\n", e.xkey.state);
                    }

                    source->event.getButton() = 0;
                    source->event.timeStamp();
                    source->updateObservers(source->event);

                    e.xkey.state &= ~(xkeys->numlock_mask | xkeys->capslock_mask | xkeys->scrolllock_mask);

                    for (i = 0; i < xkeys->nb_keys; i++) {
                        if (xkeys->keys[i].type == XKeys::SYM && xkeys->keys[i].event_type == XKeys::RELEASE) {
                            if (e.xkey.keycode == XKeysymToKeycode (display, xkeys->keys[i].key.sym)
                                && e.xkey.state == xkeys->keys[i].modifier) {

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		     adjust_display(&e.xany);
                                // 		     start_command_key (&xkeys->keys[i]);
                            }
                        } else if (xkeys->keys[i].type == XKeys::CODE && xkeys->keys[i].event_type == XKeys::RELEASE) {
                            if (e.xkey.keycode == xkeys->keys[i].key.code
                                && e.xkey.state == xkeys->keys[i].modifier) {

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		     adjust_display(&e.xany);
                                // 		     start_command_key (&xkeys->keys[i]);
                            }
                        }
                    }
                    break;

                case ButtonPress:
                    if (verbose) {
                        printf ("Button press !\n");
                        printf ("e.xbutton.button=%d\n", e.xbutton.button);
                        printf ("e.xbutton.state=%d\n", e.xbutton.state);
                    }

                    e.xbutton.state &= ~(xkeys->numlock_mask | xkeys->capslock_mask | xkeys->scrolllock_mask
                                         | Button1Mask | Button2Mask | Button3Mask
                                         | Button4Mask | Button5Mask);
	    
                    for (i = 0; i < xkeys->nb_keys; i++) {
                        if (xkeys->keys[i].type == XKeys::BUTTON && xkeys->keys[i].event_type == XKeys::PRESS) {
                            if (e.xbutton.button == xkeys->keys[i].key.button
                                && e.xbutton.state == xkeys->keys[i].modifier) {

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		      print_key (d, &xkeys->keys[i]);
                                // 		      adjust_display(&e.xany);
                                // 		      start_command_key (&xkeys->keys[i]);
                            }
                        }
                    }
                    break;
	    
                case ButtonRelease:
                    if (verbose) {
                        printf ("Button release !\n");
                        printf ("e.xbutton.button=%d\n", e.xbutton.button);
                        printf ("e.xbutton.state=%d\n", e.xbutton.state);
                    }
	    
                    e.xbutton.state &= ~(xkeys->numlock_mask | xkeys->capslock_mask | xkeys->scrolllock_mask
                                         | Button1Mask | Button2Mask | Button3Mask
                                         | Button4Mask | Button5Mask);
	    
                    for (i = 0; i < xkeys->nb_keys; i++) {
                        if (xkeys->keys[i].type == XKeys::BUTTON && xkeys->keys[i].event_type == XKeys::RELEASE) {
                            if (e.xbutton.button == xkeys->keys[i].key.button
                                && e.xbutton.state == xkeys->keys[i].modifier){

                                xkeys->print_key(display, &xkeys->keys[i]);
                                // 		      print_key (d, &xkeys->keys[i]);
                                // 		      adjust_display(&e.xany);
                                // 		      start_command_key (&xkeys->keys[i]);
                            }
                        }
                    }
                    break;
	    
                default:
                    // 	   cerr << "unknown state!" << endl;
                    break;
            };

            // 	 source->event.getButton() = source->event.getButton()++;
            // 	 source->event.timeStamp();
            // 	 source->updateObservers(source->event);


        }
      





        // 	 source->event.getPosition()[0] = FREED_Info.AbsX;
        // 	 source->event.getPosition()[1] = FREED_Info.AbsY;
        // 	 source->event.getPosition()[2] = FREED_Info.AbsZ;
        // 	 //middle button = 1, second button = 2, both = 3
        // 	 source->event.getButton() = (FREED_Info.Status & 0x3);
        // 	 source->event.timeStamp();
        // 	 source->updateObservers( source->event );
    }
}


// ============================================================
// XDisplay related stuff


void
end_it_all (Display * d) {
    int screen;

    for (screen = 0; screen < ScreenCount (d); screen++) {
        XUngrabKey (d, AnyKey, AnyModifier, RootWindow (d, screen));
        XUngrabButton (d, AnyButton, AnyModifier, RootWindow (d, screen));
    }
    //   close_keys ();
    XCloseDisplay (d);
}


static int *
null_X_error (Display * d, XErrorEvent * e)
{
    static int already = 0;

    /* The warning is displayed only once */
    if (already != 0)
        return (NULL);
    already = 1;

    printf ("\n*** Warning ***\n");
    printf ("Please, verifie that there is not another program which\n");
    printf ("captures already one of the keys captured by xbindkeys.\n");
    printf ("It seem that there is a conflict in keys and xbindkeys\n");
    printf ("can't grab all the keys defined in its configuration file.\n");

    /*   end_it_all (d); */

    /*   exit (-1); */

    return (NULL);
}


char rc_file[512];

static int
file_exist (char * filename)
{
    FILE * stream;

    if ((stream = fopen (filename, "r")) == NULL)
        return 0;

    fclose (stream);
    return 1;
}


int
rc_file_exist (void)
{
#ifdef GUILE_FLAG
    if (file_exist (rc_guile_file))
        return 1;
#endif

    if (!file_exist (rc_file))
    {
        fprintf (stderr, "Error : %s not found or reading not allowed.\n",
                 rc_file);
        fprintf (stderr,
                 "please, create one with 'xbindkeys --defaults > %s'.\n",
                 rc_file);
#ifdef GUILE_FLAG
        fprintf (stderr,
                 "or, if you want scheme configuration style,\n");
        fprintf (stderr,
                 "with 'xbindkeys --defaults-guile > %s'.\n",
                 rc_guile_file);
#endif
        return 0;
    }

    return 1;
}


OT_NAMESPACE_END

#endif

// WIN32
#endif

/* 
 * ------------------------------------------------------------
 *   End of TargusModule.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
