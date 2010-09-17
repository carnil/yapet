// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#ifdef HAVE_STRING_H
# include<string.h>
#endif

#include "partdec.h"

#include "record.h"
#include "crypt.h"

using namespace YAPET;

PartDec::PartDec() {
    memset (name, 0, NAME_SIZE);
}

PartDec::PartDec (BDBuffer& bd, const Key& key)
throw (YAPETException) : enc_data (bd) {
    Crypt crypt (key);
    Record<PasswordRecord>* dec_pw_rec = crypt.decrypt<PasswordRecord> (bd);
    PasswordRecord* ptr_dec_pw_rec = *dec_pw_rec;
    memcpy (name, ptr_dec_pw_rec->name, NAME_SIZE);
    delete dec_pw_rec;
}

PartDec::PartDec (Record<PasswordRecord>& pr, const Key& key) throw (YAPETException) {
    setRecord (pr, key);
}

PartDec::PartDec (const PartDec& pd) : enc_data (pd.enc_data) {
    memcpy (name, pd.name, NAME_SIZE);
}

PartDec::~PartDec() {
    memset (name, 0, NAME_SIZE);
}

void
PartDec::setRecord (Record<PasswordRecord>& pr, const Key& key) throw (YAPETException) {
    PasswordRecord* ptr_pr = pr;
    memcpy (name, ptr_pr->name, NAME_SIZE);
    Crypt crypt (key);
    BDBuffer* enc_pr = crypt.encrypt (pr);
    enc_data = *enc_pr;
    delete enc_pr;
}

const PartDec&
PartDec::operator= (const PartDec & pd) {
    if (this == &pd) return *this;

    memset (name, 0, NAME_SIZE);
    memcpy (name, pd.name, NAME_SIZE);
    enc_data = pd.enc_data;
    return *this;
}

/**
 * This is mainly used for sorting the entries...
 */
bool
PartDec::operator< (const PartDec& pd) const {
    if (this == &pd) return false;

    if (strcmp ( (const char*) name, (const char*) pd.name) < 0)
        return true;

    return false;
}
