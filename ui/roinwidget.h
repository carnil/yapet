// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _ROINWIDGET_H
#define _ROINWIDGET_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#include "inputwidget.h"

namespace YAPET {
    namespace UI {

        /**
         * @brief A read-only input widget.
         *
         * Basically the same as \c InputWidget, but does not allow editing. Good
         * for showing stuff that doesn't fit on the visible width of the widget.
         */
        class ROInWidget : public InputWidget {
            private:
                inline const ROInWidget& operator= (const ROInWidget&) {
                    return *this;
                }

            protected:
                virtual inline void processBackspace() throw (UIException) { /* Simply do nothing */ }
                virtual inline void processDelete() throw (UIException) { /* Simply do nothing */ }
                virtual inline void processInput (int ) throw (UIException) { /* Simply do nothing */ }

            public:
                ROInWidget (int sx, int sy, int w, int ml = 512) throw (UIException);
                virtual ~ROInWidget();
        };

    }
}
#endif // _ROINWIDGET_H