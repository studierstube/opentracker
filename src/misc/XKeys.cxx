//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Bernhard Reitinger
// created 	Thu Jul 14 09:32:34 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------


#include "XKeys.h"

#ifndef WIN32

namespace ot {

   XKeys::XKeys(const char* rcfile) {
      
      init();
      loadRC(rcfile);
   }
   
   void XKeys::init() {
      numlock_mask = 0;
      scrolllock_mask = 0;
      capslock_mask = 0;

      verbose = true;
   }

   bool XKeys::loadRC(const char* rcfile) {
   
      char line[1024];
      char line2[1024];
      char command[1024];
      KeyType_t type;
      EventType_t event_type;
      KeySym keysym;
      KeyCode keycode;
      unsigned int button;
      unsigned int modifier;
      FILE *stream = NULL;
      char *pos1;
      char *pos2;
      char *p;
      int i;
   
   
      initKeys();

      /* Open RC File */
      if ((stream = fopen (rcfile, "r")) == NULL) {
	 fprintf (stderr, "Error : %s not found or reading not allowed.\n",
		  rcfile);
	 fprintf (stderr, "please, create one with 'xbindkeys --defaults > %s'.\n",
		  rcfile);
	 return false;
      }


      /* Read RC file */
      while (fgets (line, sizeof (line), stream)) {
	 /* search for keyevent options */
	 if (strstr (line, "keyevent_numlock") != NULL) {
	    /* search for comment or command line */
	    pos1 = strchr (line, '"');
	    pos2 = strchr (line, '#');
	    if (!pos1 && !pos2) {
	       if (strstr (line, "enable") != NULL) {
		  numlock_mask = 0;
	       }
	       if (verbose)
		  printf ("keyevent_numlock: %s\n",
			  numlock_mask == 0 ? "Enabled" : "Disabled");
	    
	       continue;
	    }
	 }
      
	 if (strstr (line, "keyevent_capslock") != NULL) {
	    /* search for comment or command line */
	    pos1 = strchr (line, '"');
	    pos2 = strchr (line, '#');
	    if (!pos1 && !pos2) {
	       if (strstr (line, "enable") != NULL) {
		  capslock_mask = 0;
	       }
	       if (verbose)
		  printf ("keyevent_capslock: %s\n",
			  capslock_mask == 0 ? "Enabled" : "Disabled");
	    
	       continue;
	    }
	 }
      
	 if (strstr (line, "keyevent_scrolllock") != NULL) {
	    /* search for comment or command line */
	    pos1 = strchr (line, '"');
	    pos2 = strchr (line, '#');
	    if (!pos1 && !pos2) {
	       if (strstr (line, "enable") != NULL) {
		  scrolllock_mask = 0;
	       }
	       if (verbose)
		  printf ("keyevent_scrolllock: %s\n",
			  scrolllock_mask == 0 ? "Enabled" : "Disabled");
	    
	       continue;
	    }
	 }
      
      
	 pos1 = strchr (line, '"');
	 if (pos1) {
	    pos2 = strchr (line, '#');
	 
	    if (!pos2 || pos2 > pos1) {
	       /* search for command line */
	       pos2 = strrchr (line, '"');
	       if (pos2 && pos1 < pos2) {
		  command[0] = '\0';
		  type = SYM;
		  event_type = PRESS;
		  keysym = 0;
		  keycode = 0;
		  button = 0;
		  modifier = 0;
	       
		  for (p = pos1 + 1, i = 0;
		       p < pos2 && static_cast<unsigned int>(i) < sizeof (command); p++, i++) {
		     command[i] = *p;
		  }
		  command[i] = '\0';
	       
		  /* get associated keys */
		  if (fgets (line, sizeof (line), stream)) {
		     pos1 = line;
		  
		     while (*pos1 != '\n') {
			/* jump space */
			for (; *pos1 == '+' || *pos1 == ' '
				|| *pos1 == '\t'; pos1++);
		     
			/* find corresponding + or \n */
			pos2 = strchr (pos1, '+');
			if (!pos2) {
			   for (pos2 = pos1; *pos2 != '\n'; pos2++);
			}
		     
			/* copy string in line2 */
			for (p = pos1, i = 0;
			     p < pos2 && static_cast<unsigned int>(i) < sizeof (line2)
				&& *p != '+' && *p != ' ' && *p != '\t';
			     p++, i++) {
			   line2[i] = *p;
			}
			line2[i] = '\0';
		     
			/* is a numeric keycode (c:nnn) ? */
			if (line2[0] == 'c' && line2[1] == ':') {
			   if (isdigit (line2[2])) {
			      type = CODE;
			      keycode = strtol (line2+2, (char **) NULL, 0);
			   } else {
			      keysym = 0;
			      keycode = 0;
			      button = 0;
			      break;
			   }
			   /* is a numeric modifier (m:nnn) ? */
			} else if (line2[0] == 'm' && line2[1] == ':') {
			   if (isdigit (line2[2])) {
			      modifier |= strtol (line2+2, (char **) NULL, 0);
			   } else {
			      keysym = 0;
			      keycode = 0;
			      button = 0;
			      break;
			   }
			   /* is a mouse button (b:nnn) ? */
			} else if (line2[0] == 'b' && line2[1] == ':') {
			   if (isdigit (line2[2])) {
			      type = BUTTON;
			      button = strtol (line2+2, (char **) NULL, 0);
			   } else {
			      keysym = 0;
			      keycode = 0;
			      button = 0;
			      break;
			   }
			   /* apply to modifier, release/press or key */
			} else {
			   if (strcasecmp (line2, "control") == 0)
			      modifier |= ControlMask;
			   else if (strcasecmp (line2, "shift") == 0)
			      modifier |= ShiftMask;
			   else if (strcasecmp (line2, "mod1") == 0
				    || strcasecmp (line2, "alt") == 0)
			      modifier |= Mod1Mask;
			   else if (strcasecmp (line2, "mod2") == 0)
			      modifier |= Mod2Mask;
			   else if (strcasecmp (line2, "mod3") == 0)
			      modifier |= Mod3Mask;
			   else if (strcasecmp (line2, "mod4") == 0)
			      modifier |= Mod4Mask;
			   else if (strcasecmp (line2, "mod5") == 0)
			      modifier |= Mod5Mask;
			   else if (strcasecmp (line2, "release") == 0)
			      event_type = RELEASE;
			   else {
			      type = SYM;
			      keysym = XStringToKeysym (line2);
			      if (keysym == 0)
				 break;
			   }
			}
		  
			pos1 = pos2;
		     }
		  }
	       
		  if (!add_key(type, event_type, keysym, keycode, 
			       button, modifier, command, rcfile)) {
		     break;
		  }
	       }
	    }
	 }
      }
   

      /* Close RC File */
      if (stream != NULL)
	 fclose (stream);
   
      if (keys == NULL) {
	 fprintf (stderr, "Error in alocation of keys\n");
	 return false;
      }
   
      /* Verify if all is good (like my english) */
      for (i = 0; i < nb_keys; i++) {
	 if (keys[i].key.sym == 0 || keys[i].command == NULL) {
	    fprintf (stderr, "Error in RC file : %s\n", rcfile);
	    return false;
	 }
      }
   
      printf ("%d keys in %s\n", nb_keys, rcfile);
   
      return true;
   }


   void XKeys::initKeys() {
      nb_keys = 0;
      keys = NULL;
   }

   bool XKeys::add_key(KeyType_t type, EventType_t event_type, KeySym keysym, KeyCode keycode,
		       unsigned int button, unsigned int modifier, char *command, const char* rc_file) {
      Keys_t *keys_bis = NULL;
      int i;
      
      if (keysym == 0 && keycode == 0 && button == 0) {
	 fprintf (stderr, "Warning: unkown key in RC file : %s\n", rc_file);
	 return false;
      }
      
      /* make new array keys_bis */
      keys_bis = (Keys_t *) malloc ((nb_keys + 1) * sizeof (Keys_t));
      if (keys_bis == NULL)
	 return false;

      if (keys != NULL) {
	 /* copy keys in keys_bis */
	 for (i = 0; i < nb_keys; i++) {
	    keys_bis[i] = keys[i];
	 }

	 /* delete old keys array */
	 free (keys);
      }

      /* make keys_bis as keys */
      keys = keys_bis;
      
      modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);
      
      /* set key */
      if (type == SYM) {
	 set_keysym (&keys[nb_keys], event_type, keysym, modifier, command);
      } else if (type == BUTTON) {
	 set_button (&keys[nb_keys], event_type, button, modifier, command);
      } else {
	 set_keycode (&keys[nb_keys], event_type, keycode, modifier, command);
      }
      
      /* new key */
      nb_keys += 1;
      
      return true;
   }
   

   void XKeys::set_keysym (Keys_t * key, EventType_t event_type, KeySym keysym,
			   unsigned int modifier, char *command) {
      
      key->type = SYM;
      key->event_type = event_type;
      key->key.sym = keysym;
      key->modifier = modifier;
      
      key->command = (char *) malloc ((strlen (command) + 1) * sizeof (char));
      
      if (key->command != NULL)
	 strcpy (key->command, command);
   }

   void XKeys::set_keycode (Keys_t * key, EventType_t event_type, KeySym keycode, 
			    unsigned int modifier, char *command) {
      
      key->type = CODE;
      key->event_type = event_type;
      key->key.code = keycode;
      key->modifier = modifier;
      
      key->command = (char *) malloc ((strlen (command) + 1) * sizeof (char));
      
      if (key->command != NULL)
	 strcpy (key->command, command);
   }
   

   void XKeys::set_button (Keys_t * key, EventType_t event_type, unsigned int button, 
			   unsigned int modifier, char *command) {

      key->type = BUTTON;
      key->event_type = event_type;
      key->key.button = button;
      key->modifier = modifier;
      
      key->command = (char *) malloc ((strlen (command) + 1) * sizeof (char));
      
      if (key->command != NULL)
	 strcpy (key->command, command);
   }

   void XKeys::free_key (Keys_t * key) {

      key->type = SYM;
      key->event_type = PRESS;
      key->key.sym = 0;
      key->modifier = 0;
      
      if (key->command != NULL)
	 free (key->command);
   }
   
   void XKeys::showKeys(Display * d)
   {
      int i;
      int last_verbose = verbose;
      
      verbose = 1;
      
      for (i = 0; i < nb_keys; i++)
	 {
	    print_key(d, &keys[i]);
	 }
      
      verbose = last_verbose;
   }

   void XKeys::print_key (Display * d, Keys_t * key)
   {
      char str[1000];
      
      if (verbose)
	 {
	    if (key->type == SYM)
	       {
		  modifier_to_string (key->modifier, str);
		  
		  printf ("\"%s\"\n    %s%s%s%s\n",
			  key->command != NULL ? key->command : "NoCommand",
			  key->event_type == PRESS ? "" : "Release + ",
			  str, str[0] ? " + " : "", XKeysymToString (key->key.sym));
	       }
	    else if (key->type == BUTTON)
	       {
		  printf ("\"%s\"\n    %sm:0x%x + b:%d   (mouse)\n",
			  key->command != NULL ? key->command : "NoCommand",
			  key->event_type == PRESS ? "" : "Release + ",
			  key->modifier, key->key.button);
	       }
	    else
	       {
		  printf ("\"%s\"\n    %sm:0x%x + c:%d\n",
			  key->command != NULL ? key->command : "NoCommand",
			  key->event_type == PRESS ? "" : "Release + ",
			  key->modifier, key->key.code);
		  if (d != NULL)
		     {
			modifier_to_string (key->modifier, str);
			printf ("    %s%s%s%s\n",
				str,
				str[0] ? " + " : "",
				key->event_type == PRESS ? "" : "Release + ",
				(XKeysymToString
				 (XKeycodeToKeysym (d, key->key.code, 0)) !=
				 NULL) ? XKeysymToString (XKeycodeToKeysym (d,
									    key->key.
									    code,
									    0)) :
				"NoSymbol");
		     }
	       }
	 }
   }

void XKeys::modifier_to_string (unsigned int modifier, char *str)
{

   char *modifier_string[] = { "Control", "Shift", "Alt", "Mod2",
			       "Mod3", "Mod4", "Mod5"
   };
   

  str[0] = '\0';

  if (modifier & ControlMask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[0]);
    }

  if (modifier & ShiftMask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[1]);
    }


  if (modifier & Mod1Mask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[2]);
    }

  if (modifier & Mod2Mask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[3]);
    }

  if (modifier & Mod3Mask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[4]);
    }

  if (modifier & Mod4Mask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[5]);
    }

  if (modifier & Mod5Mask)
    {
      if (str[0])
	strcat (str, "+");
      strcat (str, modifier_string[6]);
    }
}

void XKeys::grab_keys (Display * dpy)
{
  int i;
  int min, max;
  int screen;

  XDisplayKeycodes (dpy, &min, &max);

  if (verbose)
    {
      printf ("\n");
      printf ("min_keycode=%d     max_keycode=%d (ie: know keycodes)\n", min,
	      max);
    }


  for (i = 0; i < nb_keys; i++)
    {
      print_key (dpy, &keys[i]);

      if (keys[i].type == SYM)
	{
	  for (screen = 0; screen < ScreenCount (dpy); screen++)
	    {
	      my_grab_key (dpy, XKeysymToKeycode (dpy, keys[i].key.sym),
			   keys[i].modifier, RootWindow (dpy, screen));
	    }
	}
      else if (keys[i].type == BUTTON)
	{
	  for (screen = 0; screen < ScreenCount (dpy); screen++)
	    {
	      my_grab_button (dpy, keys[i].key.button, keys[i].modifier, 
			      RootWindow (dpy, screen));
	    }
	}
      else
	{
	  if (keys[i].key.code >= min && keys[i].key.code <= max)
	    {
	      for (screen = 0; screen < ScreenCount (dpy); screen++)
		{
		  my_grab_key (dpy, keys[i].key.code, keys[i].modifier, 
			       RootWindow (dpy, screen));
		}
	    }
	  else
	    {
	      fprintf (stderr, "--- xbindkeys error ---\n");

	      if (!verbose)
		{
		  verbose = 1;
		  print_key (dpy, &keys[i]);
		}

	      fprintf (stderr,
		       "  The keycode %d cannot be used, as it's not between the\n"
		       "  min(%d) and max(%d) keycode of your keyboard.\n"
		       "  Please increase the 'maximum' value in\n"
		       "    /usr/X11R6/lib/X11/xkb/keycodes/xfree86,\n"
		       "  then restart X.\n", keys[i].key.code, min, max);
// 	      end_it_all (dpy);
	      exit (-1);
	    }
	}
    }
}


void XKeys::my_grab_key (Display * dpy, KeyCode keycode, unsigned int modifier,
	     Window win)
{
  modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);


  XGrabKey (dpy, keycode, modifier, (win ? win : DefaultRootWindow (dpy)),
	    False, GrabModeAsync, GrabModeAsync);

  if (modifier == AnyModifier)
    return;

  if (numlock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (capslock_mask)
    XGrabKey (dpy, keycode, modifier | capslock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && capslock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask | capslock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (capslock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | capslock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && capslock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode,
	      modifier | numlock_mask | capslock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)), False, GrabModeAsync,
	      GrabModeAsync);

}


void XKeys::my_grab_button (Display * dpy, unsigned int button, unsigned int modifier,
		Window win)
{
  modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);

  XGrabButton (dpy, button, modifier, (win ? win : DefaultRootWindow (dpy)),
	       False, ButtonPressMask | ButtonReleaseMask,
	       GrabModeAsync, GrabModeAsync, None, None);

  if (modifier == AnyModifier)
    return;

  if (numlock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);


  if (capslock_mask)
    XGrabButton (dpy, button, modifier | capslock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (scrolllock_mask)
    XGrabButton (dpy, button, modifier | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && capslock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask | capslock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && scrolllock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (capslock_mask && scrolllock_mask)
    XGrabButton (dpy, button, modifier | capslock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && capslock_mask && scrolllock_mask)
    XGrabButton (dpy, button,
		 modifier | numlock_mask | capslock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)), False,
		 ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
		 GrabModeAsync, None, None);
}


void XKeys::freeKeys() {
   int i;

   for (i = 0; i < nb_keys; i++)
      free_key (&keys[i]);
   
   if (keys != NULL)
      free (keys);
}



} // namespace

#endif



/* 
 * ------------------------------------------------------------
 *   End of XKeys.h
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
