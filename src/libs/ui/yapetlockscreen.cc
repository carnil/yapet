/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

#include <cstdlib>
#include <cstring>

#include "globals.h"
#include "intl.h"
#include "yapetlockscreen.h"

//
// Public
//

YapetLockScreen::YapetLockScreen(YACURS::Window& mainWindow,
                                 YACURS::UnlockDialog* _unlock,
                                 unsigned int timeout,
                                 unsigned int ulck_timeout)
    : LockScreen{_unlock, timeout, ulck_timeout},
      _mainWindow{dynamic_cast<const MainWindow&>(mainWindow)} {}

YapetLockScreen::~YapetLockScreen() {}

void YapetLockScreen::show() {
    if (_mainWindow.currentFilename().empty()) {
        // we cannot lock, there is no open file
        return;
    }

    LockScreen::show();

    YACURS::Curses::set_terminal_title(_("YAPET LOCKED"));
}

void YapetLockScreen::close() {
    LockScreen::close();

    if (!_mainWindow.currentFilename().empty()) {
        std::string ttl("YAPET");
#ifdef HAVE_BASENAME
        ttl += " (";
        // basename may modify the string
        char* tmp = strdup(_mainWindow.currentFilename().c_str());
        ttl += basename(tmp);

        ttl += ")";
        std::free(tmp);
#endif
        YACURS::Curses::set_terminal_title(ttl);
    }
}
