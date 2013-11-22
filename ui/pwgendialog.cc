// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>

#include "intl.h"
#include "globals.h"
#include "pwgendialog.h"

//
// Private
//
std::string
PwGenDialog::get_name_of_rng() const {
    switch (pwgen.getRNGUsed() ) {
    case YAPET::PWGEN::DEVRANDOM:
	return "/dev/random";
    case YAPET::PWGEN::DEVURANDOM:
	return "/dev/urandom";
    case YAPET::PWGEN::LRAND48:
	return "lrand48()";
    case YAPET::PWGEN::RAND:
	    return "rand()";
    case YAPET::PWGEN::NONE:
	return "none!";
    case YAPET::PWGEN::AUTO:
	return "auto";
    }

    assert (0);
    return "oops!";
}

//
// Protected
//
void
PwGenDialog::checkbox_selection_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_CHECKBOX_SELECTION);

    YACURS::EventEx<YACURS::CheckBox*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::CheckBox*>&>(_e);

    if ( evt.data() == charpools) {
	int pools = 0;

	if (charpools->selected(_("Letters")))
	    pools |= YAPET::PWGEN::LETTERS;
	if (charpools->selected(_("Digits")))
	    pools |= YAPET::PWGEN::DIGITS;
	if (charpools->selected(_("Punctuation")))
	    pools |= YAPET::PWGEN::PUNCT;
	if (charpools->selected(_("Special")))
	    pools |= YAPET::PWGEN::SPECIAL;
	if (charpools->selected(_("Other")))
	    pools |= YAPET::PWGEN::OTHER;

	YAPET::Globals::config.character_pools.set(pools);
	pwgen.setNewPool(pools);
	return;
    }

    if (evt.data() == sources) {
	int available_rngs = YAPET::PWGEN::RNG::getAvailableRNGs();
	YAPET::PWGEN::RNGENGINE rng;

	if ( (available_rngs & YAPET::PWGEN::DEVRANDOM) &&
	     sources->selected("/dev/random"))
	    rng = YAPET::PWGEN::DEVRANDOM;

	if ( (available_rngs & YAPET::PWGEN::DEVURANDOM) &&
	     sources->selected("/dev/urandom"))
	    rng = YAPET::PWGEN::DEVURANDOM;

	if ( (available_rngs & YAPET::PWGEN::LRAND48) &&
	     sources->selected("lrand48()"))
	    rng = YAPET::PWGEN::LRAND48;

	if ( (available_rngs & YAPET::PWGEN::RAND) &&
	     sources->selected("rand()"))
	    rng = YAPET::PWGEN::RAND;


	YAPET::Globals::config.pwgen_rng.set(rng);
	pwgen.setNewRNG(rng);
    }
}

void
PwGenDialog::button_press_handler(YACURS::Event& _e) {
    Dialog::button_press_handler(_e);

    YACURS::EventEx<YACURS::Button*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if ( evt.data() == &regenbutton) {
	// Transfer pwlen to configuration (might have changed)
	int num=0;

#warning "Use constants"
	if ( !(std::istringstream(pwlen.input()) >> num) )
	    num = 10; // fallback

#warning "Use constants"
	// check password length
	if (num < 2) {
	    num = 10; // c'mon, nobody wants a password that short
	    pwlen.input("10");
	}

#warning "Use constants"
	if (num > 256) {
	    num = 256;
	    pwlen.input("256");
	}

	YAPET::Globals::config.pwgenpwlen.set(num);

	pwgen.generatePassword (YAPET::Globals::config.pwgenpwlen.get());
	genpw.input(pwgen.getPassword());
    }
}

//
// Public
//
PwGenDialog::PwGenDialog():
    YACURS::Dialog(_("Password Generator"), YACURS::OKCANCEL, YACURS::AUTOMATIC ),
    pwgen(YAPET::Globals::config.character_pools.get()),
    mainpack(),
    boxespack(),
    genpwlabel(_("Generated password")),
    genpw(),
    pwlenlabel(_("Password length")),
    pwlen(),
    regenbutton(_("Regenerate")) {

    // Make sure configuration does meet available rngs
    YAPET::PWGEN::RNGENGINE requested_rng = YAPET::Globals::config.pwgen_rng.get();
    int available_rngs = YAPET::PWGEN::RNG::getAvailableRNGs();
    if (available_rngs & requested_rng) {
	pwgen.setNewRNG(requested_rng);
    }

    // coloring
    genpwlabel.color(YACURS::DIALOG);
    pwlenlabel.color(YACURS::DIALOG);

    genpw.max_input(256);
    pwlen.max_input(3);
    pwlen.filter(YACURS::FilterDigit());

    std::vector<std::string> labels;

    // If labels are updated, please make sure
    // PwGenDialog::checkbox_selection_handler() is adjusted
    // accordingly.
    labels.push_back(_("Letters"));
    labels.push_back(_("Digits"));
    labels.push_back(_("Punctuation"));
    labels.push_back(_("Special"));
    labels.push_back(_("Other"));
    charpools = new YACURS::CheckBox(_("Character Pools"), labels);

    if (YAPET::Globals::config.character_pools.get() & YAPET::PWGEN::LETTERS)
	charpools->set_selection(_("Letters"));
    if (YAPET::Globals::config.character_pools.get() & YAPET::PWGEN::DIGITS)
	charpools->set_selection(_("Digits"));
    if (YAPET::Globals::config.character_pools.get() & YAPET::PWGEN::PUNCT)
	charpools->set_selection(_("Punctuation"));
    if (YAPET::Globals::config.character_pools.get() & YAPET::PWGEN::SPECIAL)
	charpools->set_selection(_("Special"));
    if (YAPET::Globals::config.character_pools.get() & YAPET::PWGEN::OTHER)
	charpools->set_selection(_("Other"));

    // Those labels must match what is returned by get_name_of_rng()
    // Set only those rngs that are available
    labels.clear();
    if (available_rngs & YAPET::PWGEN::DEVRANDOM)
	labels.push_back("/dev/random");
    if (available_rngs & YAPET::PWGEN::DEVURANDOM)
	labels.push_back("/dev/urandom");
    if (available_rngs & YAPET::PWGEN::LRAND48)
	labels.push_back("lrand48()");
    if (available_rngs & YAPET::PWGEN::RAND)
	labels.push_back("rand()");

    sources = new YACURS::RadioBox(_("Sources"), labels);
    sources->set_selection(get_name_of_rng());

    mainpack.add_back(&genpwlabel);
    mainpack.add_back(&genpw);

    mainpack.add_back(&pwlenlabel);
    mainpack.add_back(&pwlen);

    mainpack.add_back(&boxespack);

    boxespack.always_dynamic(true);
    boxespack.add_back(charpools);
    boxespack.add_back(sources);

    widget(&mainpack);

    // Add regenerate button
    add_button(&regenbutton);

    // Initialize values
    std::ostringstream conv;
    conv << YAPET::Globals::config.pwgenpwlen.get();
    pwlen.input(conv.str());

    pwgen.generatePassword (YAPET::Globals::config.pwgenpwlen.get());
    genpw.input(pwgen.getPassword());

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
				      PwGenDialog>(YACURS::
						  EVT_CHECKBOX_SELECTION,
						  this,
						  &PwGenDialog::
						  checkbox_selection_handler) );
}

PwGenDialog::~PwGenDialog() {
    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
					 PwGenDialog>(YACURS::
						      EVT_CHECKBOX_SELECTION,
						      this,
						      &PwGenDialog::
						      checkbox_selection_handler) );

    delete charpools;
    delete sources;
}