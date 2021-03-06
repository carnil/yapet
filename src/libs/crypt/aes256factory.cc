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

#include "aes256factory.hh"

using namespace yapet;

Aes256Factory::Aes256Factory(const SecureArray& password,
                             const MetaData& keyingParameters)
    : _key256{new Key256{}} {
    _key256->keyingParameters(keyingParameters);
    _key256->password(password);
}

std::shared_ptr<AbstractCryptoFactory> Aes256Factory::newFactory(
    const SecureArray& password, const MetaData& keyingParameters) const {
    return std::shared_ptr<AbstractCryptoFactory>{
        new Aes256Factory(password, keyingParameters)};
}

std::unique_ptr<Crypto> Aes256Factory::crypto() const {
    return std::unique_ptr<Crypto>{new Aes256{_key256}};
}

std::unique_ptr<YapetFile> Aes256Factory::file(const std::string& filename,
                                               bool create, bool secure) const {
    return std::unique_ptr<YapetFile>{
        new Yapet20File{filename, create, secure}};
}