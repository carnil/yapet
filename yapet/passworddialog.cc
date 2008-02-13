// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "passworddialog.h"
#include "messagebox.h"

void
PasswordDialog::createWindow() throw(YAPETUI::UIException) {
    if (window != NULL)
	throw YAPETUI::UIException("May you consider deleting the window before reallocating");

    window = newwin(getHeight(), getWidth(), getStartY(), getStartX());
    if (window == NULL)
	throw YAPETUI::UIException("Error creating password dialog");

    pwidget1 = new YAPETUI::PasswordWidget(getStartX() + 1,
					 getStartY() + 3,
					 getWidth() - 2);
    if (pwtype == NEW_PW)
	pwidget2 = new YAPETUI::PasswordWidget(getStartX() + 1,
					     getStartY() + 5,
					     getWidth()-2);

    okbutton = new YAPETUI::Button("Ok",
				 getStartX() + 1,
				 getStartY() + getHeight() - 2);

    cancelbutton = new YAPETUI::Button("Cancel",
				     getStartX() + okbutton->getLength() + 2,
				     getStartY() + getHeight() - 2);

}

PasswordDialog::PasswordDialog(PWTYPE pt, std::string fn)
    throw(YAPETUI::UIException) : window(NULL),
			 pwidget1(NULL),
			 pwidget2(NULL),
			 okbutton(NULL),
			 cancelbutton(NULL),
			 pwtype(pt),
			 key(NULL),
			 filename(fn){
    createWindow();
			 }

PasswordDialog::~PasswordDialog() {
    delete pwidget1;
    if (pwtype == NEW_PW)
	delete pwidget2;
    delete okbutton;
    delete cancelbutton;
    wclear(window);
    delwin(window);
}

void
PasswordDialog::run() throw(YAPETUI::UIException) {
    refresh();
    while (true) {
	int ch = 0;
#ifdef HAVE_WRESIZE
	while ( (ch = pwidget1->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	pwidget1->focus();
#endif // HAVE_WRESIZE

	// Password input widget 2 only if we prompt for a new password
	if (pwtype == NEW_PW) {
#ifdef HAVE_WRESIZE
	    while ( (ch = pwidget2->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	    pwidget2->focus();
#endif // HAVE_WRESIZE
	}

#ifdef HAVE_WRESIZE
	while ( (ch = okbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = okbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n') {
	    if (pwtype == NEW_PW) {
		if (pwidget1->getText() == pwidget2->getText()) {
		    key = new YAPET::Key(pwidget1->getText().c_str());
		    return;
		} else {
		    YAPETUI::MessageBox* errmsg = NULL;
		    try {
			errmsg = new YAPETUI::MessageBox("Error", "Passwords do not match");
			errmsg->run();
			delete errmsg;
		    } catch(YAPETUI::UIException&) {
			if (errmsg == NULL)
			    delete errmsg;
		    }
		    pwidget1->setText("");
		    pwidget2->setText("");
		    refresh();
		    continue;
		}
	    } else {
		key = new YAPET::Key(pwidget1->getText().c_str());
		pwidget1->clearText();
		return;
	    }
	}
#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n')
	    return;
    }

}

void
PasswordDialog::resize() throw(YAPETUI::UIException) {
    int retval = delwin(window);
    if (retval == ERR)
	throw YAPETUI::UIException("Error deleting password dialog window");

    pwidget1->clearText();
    delete pwidget1;
    if (pwtype == NEW_PW) {
	pwidget2->clearText();
	delete pwidget2;
    }
    delete okbutton;
    delete cancelbutton;

    window = NULL;
    pwidget1 = NULL;
    pwidget2 = NULL;
    okbutton = NULL;
    cancelbutton = NULL;

    createWindow();
}

void
PasswordDialog::refresh() throw(YAPETUI::UIException) {
    YAPETUI::Colors::setcolor(window, YAPETUI::MESSAGEBOX);
    int retval = werase(window);
    if (retval == ERR)
	throw YAPETUI::UIException("Error clearing password dialog");

    retval = box(window, 0, 0);
    if (retval == ERR)
    throw YAPETUI::UIException("Error adding box");

    retval = mymvwaddstr(window, 0, 2, "P A S S W O R D");
    if (retval == ERR)
	throw YAPETUI::UIException("Error setting title");

    // The label holding the file name
    retval = mymvwaddstr(window, 2, 1, filename.c_str());
    if (retval == ERR)
	throw YAPETUI::UIException("Error setting label");

    if (pwtype == NEW_PW) {
	retval = mymvwaddstr(window, 1, 1, "Enter new password for");
	if (retval == ERR)
	    throw YAPETUI::UIException("Error setting label");

	retval = mymvwaddstr(window, 4, 1, "Confirm password");
	if (retval == ERR)
	    throw YAPETUI::UIException("Error setting label");
    } else {
	retval = mymvwaddstr(window, 1, 1, "Enter password for");
	if (retval == ERR)
	    throw YAPETUI::UIException("Error setting label");
    }

    retval = wrefresh(window);
    if (retval == ERR)
	throw YAPETUI::UIException("Error refreshing password dialog");

    pwidget1->refresh();
    if (pwtype == NEW_PW)
	pwidget2->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}
