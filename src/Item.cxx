//
// "$Id$"
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <fltk/Item.h>
#include <fltk/Symbol.h>
#include <fltk/CheckButton.h>
#include <fltk/draw.h>
#include <string.h>

using namespace fltk;

/*! \class fltk::Item

  This widget is designed to be put into fltk::Menu and fltk::Browser widgets
  and draw plain-text items. All events are ignored, thus causing
  the menu and browser to set/clear the SELECTED flag on these
  widgets. If they are selected they draw in the selection_color(),
  otherwise in the color().

*/

static void revert(Style* s) {s->box_ = FLAT_BOX;}
static NamedStyle style("Item", revert, &Item::default_style);
/*! The default style sets FLAT_BOX. Changing this will mess up the
  appearance of both menus and browsers. All the rest of the style
  is blank, and normally it inherits from the current browser or
  menu, which should call set_style() before drawing any items.
*/
NamedStyle* Item::default_style = &::style;

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly.
*/
Item::Item(const char* l) : Widget(0,0,0,0,l) {
  // we need to defer setting the glyph to here because C++ has no way
  // to make sure the check button style is constructed before this style:
  if (!default_style->glyph_)
    default_style->glyph_ = CheckButton::default_style->glyph_;
  style(default_style);
  set_flag(ALIGN_LEFT|ALIGN_INSIDE);
}

/** Modify the parent of the Item::default_style to this style.
    If no style settings have been done to an Item, it will use the
    textfont, textsize, textcolor, and possibly other settings
    inherited from this style to draw itself. This is used by menus
    and browsers to cause all the elements inside them to draw
    using their settings.

    Use Item::clear_style() to put this back so that \a style can
    be deleted. This is the same as setting it to Widget::default_style.
*/
void Item::set_style(const Style* style) {
  // make sure we don't make a loop:
  for (const Style* p = style; p; p = p->parent_)
    if (p == &::style) return;
  ::style.parent_ = style;
}

/** The SELECTED flag will cause it to draw using the selected colors.
    Focusbox is also drawn if FOCUSED is on.

    The current version can also draw check or radio buttons
    but this functionality may be removed.
*/
void Item::draw() {
  drawstyle(style(), flags() & ~OUTPUT);
  if (flags() & SELECTED) {
    setbgcolor(selection_color());
    setcolor(contrast(selection_textcolor(), getbgcolor()));
  }
  box()->draw(Rectangle(w(),h()));
  Rectangle r(w(),h());
  box()->inset(r);
  if (type()) {
    int gw = int(textsize())+2;
    Rectangle lr(r);
    lr.move_x(gw+3);
    draw_label(lr, flags());
    draw_glyph(0, Rectangle(r.x()+3, r.y()+((r.h()-gw)>>1), gw, gw));
  } else {
    draw_label(r, flags());
  }
  focusbox()->draw(r);
}

/** Measure the space the draw() will take and set w() and h().
    Maybe this should be the default layout() function for Widget? */
void Item::layout() {
  if (w() && h()) return; // already at the correct size
  //int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  setfont(textfont(), textsize());
  int w = 250, h = 250;
  measure(label(), w, h);
  if (w) {w += 6+int(textsize())/2;}
  if (type()) w += 15;
  if (image()) {
    int W,H;
    image()->measure(W, H);
    if (H > h) h = H;
    w += W;
  }
  this->w(w);
  this->h(h+int(leading()));
  Widget::layout();
}

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int Item::handle(int) {return 0;}

////////////////////////////////////////////////////////////////

/*! \class fltk::ItemGroup

  This widget describes a set of items that are to be put inside a
  fltk::Menu or fltk::Browser widget. It indicates the title of a
  submenu, or a level of hierarchy in the browser. Any child widgets
  are the items in that submenu, or the items under this parent in the
  browser.

  If this widget is told to draw, it draws just like fltk::Item
  draws. See that for more details. The triangle indicating a submenu
  is not drawn by this, it is drawn by the parent menu.

  Because this is an fltk::Menu subclass, you can also call popup()
  and add() and other methods to manipulate the items inside it.

  In a Browser, the value() indicates if the widget is open or not. In
  a MultiBrowser, the selected() indicates if the widget is currently
  selected.

*/

#include <fltk/ItemGroup.h>
#include <fltk/damage.h>

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly. */
ItemGroup::ItemGroup(const char* l) : Menu(0,0,0,0,l) {
  style(Item::default_style);
  align(ALIGN_LEFT|ALIGN_INSIDE);
  // Undo the Menu class changing default_callback:
  callback(Widget::default_callback);
}

// implementation of draw & layout should be identical to Item type()==0

void ItemGroup::draw() {
  drawstyle(style(), flags() & ~OUTPUT);
  if (flags() & SELECTED) {
    setbgcolor(selection_color());
    setcolor(contrast(selection_textcolor(), getbgcolor()));
  }
  box()->draw(Rectangle(w(),h()));
  Rectangle r(w(),h());
  box()->inset(r);
  draw_label(r, flags());
  focusbox()->draw(r);
}

void ItemGroup::layout() {
  if (w() && h()) return; // already at the correct size
  //int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  setfont(textfont(), textsize());
  int w = 250, h = 250;
  measure(label(), w, h);
  if (w) {w += 6+int(textsize())/2;}
  if (image()) {
    int W,H;
    image()->measure(W, H);
    if (H > h) h = H;
    w += W;
  }
  this->w(w);
  this->h(h+int(leading()));
  Widget::layout();
}

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int ItemGroup::handle(int) {return 0;}

////////////////////////////////////////////////////////////////

/*! \class fltk::Divider
  This widget is designed to go into Menu and Browser widgets and draw
  an inset horizontal line across them. It has the OUTPUT flag set
  so the user cannot choose it.
*/

#include <fltk/Divider.h>

/*! Unlike other widgets the constructor does not take any dimensions,
  since it is assummed the container widget will size this
  correctly. */
Divider::Divider() : Widget(0,0,0,0) {
  style(Item::default_style);
  set_flag(ALIGN_LEFT|ALIGN_INSIDE|OUTPUT);
}

void Divider::draw() {
  setcolor(GRAY33);
  if (w() > h()) {
    int y = (h()-1)/2;
    drawline(0, y, w()-1, y);
    setcolor(WHITE);
    drawline(0, y+1, w()-1, y+1);
  } else if (h()) {
    int x = (w()-1)/2;
    drawline(x, 0, x, h()-1);
    setcolor(WHITE);
    drawline(x+1, 0, x+1, h()-1);
  }
}

/*! Resizes to a 2x2 square */
void Divider::layout() {
  if (!w()) w(2);
  if (!h()) h(2);
}

/*! Returns 0 always. Items do not accept \e any events. Any results
  of clicking on them is handled by the parent Menu or Browser. */
int Divider::handle(int) {return 0;}