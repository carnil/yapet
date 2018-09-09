//
// Copyright (C) 2018  Rafael Ostertag
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

#ifndef _YAPET20FILE_HH
#define _YAPET20FILE_HH

#include <list>

#include "yapet10file.hh"

namespace yapet {

/**
 * @brief Class for storing and retrieving encrypted data to and from disk
 *
 * This class takes care of storing and retrieving encrypted password
 * records to and from disk.
 *
 * Each file created by this class starts with a unencrypted recognition
 * string which currently consists of the 8 bytes "YAPET1.0" as depicted
 * below.
 *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   2    |   .    |   0    |
| 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
@endverbatim
 *
 * After the recognition string a 4 byte unsigned integer which is stored
 * in big-endian order follows. This indicator is read to determine how
 * many bytes to read in order to get the encrypted header.
 *
@verbatim
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted header exactly as many bytes    |
|        indicated by the prefix             |
+--------+--------+--------+--------+--...---+
@endverbatim
 *
  The unencrypted header size is 29 bytes.
 *
@verbatim
+--------+
|Version |
| 1 byte |
+--------+--------+--------+--...---+
|          Control String           |
|             20 bytes              |
+--------+--------+--------+--...---+--------+--------+--------+--------+
|             Time when the Password was set (8 bytes)                  |
|                                                                       |
+--------+--------+--------+--------+--------+--------+--------+--------+
@endverbatim
 *
 * Each encrypted password record is prefixed by a 4 byte unsigned integer
 * which is stored in big-endian order. The methods take care returning them
 * in the appropriate order of the host system. That integer is used to
 * indicate the length of the following encrypted data chunk.
 *
@verbatim
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
      [ . . . ]
@endverbatim
 *
 * Putting this together, an encrypted file created by this class looks like
 * this
 *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   2    |   .    |   0    |
| 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted header exactly as many bytes    |
|        indicated by the prefix             |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
      [ . . . ]
@endverbatim
 *
 * Instances of this class keeps the file open for the lifetime of the
 * instance.
 *
 */
class Yapet20File : public Yapet10File {
   public:
    Yapet20File(const std::string& filename, bool create = false,
                bool secure = true);

    Yapet20File(const Yapet20File&) = delete;
    Yapet20File& operator=(const Yapet20File&) = delete;

    Yapet20File(Yapet20File&& other);
    Yapet20File& operator=(Yapet20File&& other);

    virtual ~Yapet20File() override;

    virtual int recognitionStringSize() const override;
    virtual const uint8_t* recognitionString() const override;
};
}  // namespace yapet

#endif /* _YAPET20FILE_HH */
