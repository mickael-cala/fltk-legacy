//
// "$Id: Fl_Check_Button.h,v 1.4 2002/10/29 00:37:23 easysw Exp $"
//
// Check button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#ifndef Fl_Check_Button_H
#define Fl_Check_Button_H

#include "Fl_Button.h"

/**

   Buttons generate callbacks when they are clicked by the user. You control 
   exactly when and how by changing the values for type() and when(). 

   \image Fl_Check_Button.gif

   The Fl_Check_Button subclass display the "on" state by turning on a light,
   rather than drawing pushed in. The shape of the "light" is initially set 
   to FL_DIAMOND_DOWN_BOX. The color of the light when on is controlled with
   selection_color(), which defaults to FL_RED. 

*/
class FL_API Fl_Check_Button : public Fl_Button {
public:
  /**
   Creates a new Fl_Check_Button widget using the given 
   position, size, and label string.
  */
  Fl_Check_Button(int x,int y,int w,int h,const char *l = 0);
  static Fl_Named_Style* default_style;
protected:
  virtual void draw();
};

#endif

//
// End of "$Id: Fl_Check_Button.h,v 1.4 2002/10/29 00:37:23 easysw Exp $".
//
