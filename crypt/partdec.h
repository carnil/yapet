// -*- c++ -*-
//
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

#ifndef _PARTDEC_H
#define _PARTDEC_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include "record.h"
#include "key.h"
#include "yapetexception.h"
#include "bdbuffer.h"
#include "structs.h"

namespace YAPET {
    /**
     * @brief Holds a partially decrypted record
     * 
     * Partially decrypted records have their name stored in plain
     * text. The other fields of the password record remain
     * encrypted. This class is used for convenience. It relieves the
     * user of writing code for decrypting the record in order to get
     * only the record name.
     *
     * The \c File class uses this class when reading and returning
     * the records stored in a file. It also expects a list of \c
     * PartDec object when writing password records to the file.
     *
     * The encrypted data is also attached to this class as a \c BDBuffer.
     */
    class PartDec {
	private:
	    /**
	     * @brief The record name in plain text
	     *
	     * The password record name in plain text.
	     */
	    uint8_t name[NAME_SIZE];
	    /**
	     * @brief The encrypted password record.
	     *
	     * This is the encrypted password record.
	     */
	    BDBuffer enc_data;

	public:
	    PartDec();

	    PartDec(BDBuffer& bd,
		    const Key& key) throw(YAPETException);

	    PartDec(Record<PasswordRecord>& pr,
		    const Key& key) throw(YAPETException);

	    PartDec(const PartDec& pd);

	    ~PartDec();

	    void setRecord(Record<PasswordRecord>& pr,
			   const Key& key) throw(YAPETException);
	    /**
	     * @brief Get the encrypted password record.
	     *
	     * Gets the encrypted password record associated with this
	     * object.
	     *
	     * @return reference to the \c BDBuffer holding the
	     * encrypted data.
	     */
	    inline const BDBuffer& getEncRecord() const { return enc_data; }
	    /**
	     * @brief Get the plain text name of the password record.
	     *
	     * Returns the pointer to the plain text name of the
	     * password record.
	     *
	     * @return pointer to the buffer holding the plain text
	     * name of the password record.
	     */
	    inline const uint8_t* getName() const { return name; }
	    /**
	     * This method has been added because \c
	     * YAPETUI::ListWidget expects the object assigned to the
	     * list being displayed to have a public method called \c
	     * c_str().
	     *
	     * @copydoc getName()
	     */
	    inline const char* c_str() const { return (char*)name; }
	    const PartDec& operator=(const PartDec& pd);
    };
}

#endif // _PARTDEC_H
