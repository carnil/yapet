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

#ifdef CFGDEBUG
#include <iostream>
#endif

#include <cctype>
#include <cstdio>
#include <cstdlib>

#include <pwd.h>
#include <unistd.h>

#include "cfg.h"
#include "intl.h"

using namespace YAPET;
using namespace YAPET::CONFIG;

namespace YAPET {
namespace CONFIG {
std::string trim(const std::string& s) {
    if (s.empty()) return s;

    // find leading spaces
    std::string::size_type pos = 0;
    while (std::isspace(s[pos++]) && s.length() > pos)
        ;
    pos--;
    assert(pos < s.length());

    std::string working_copy(s.substr(pos));

    // find trailing spaces
    pos = working_copy.length() - 1;
    while (std::isspace(working_copy[pos]) && pos != 0) {
        pos--;
    }
    assert(pos < working_copy.length());

    return working_copy.substr(0, pos + 1);
}

std::string getHomeDir() {
    std::string homedir("");

    char* hd = getenv("HOME");

    if (hd != 0) {
        homedir = hd;

        if (homedir[homedir.length()] != '/') homedir.push_back('/');

        return homedir;
    }

    struct passwd* pwd;
    pwd = getpwuid(getuid());

    if (pwd != 0) {
        homedir = pwd->pw_dir;

        if (homedir[homedir.length()] != '/') homedir.push_back('/');

        return homedir;
    }

    assert(!homedir.empty());
    return homedir;
}
}  // namespace CONFIG
}  // namespace YAPET

//
// Class CfgValPetFile
//
std::string CfgValPetFile::cleanup_path(const std::string& p) {
    if (p.empty()) return p;

    std::string working_copy(p);

    // If there is a ~ at the very first position, replace that with
    // the home directory of the user.
    if (working_copy[0] == '~') working_copy.replace(0, 1, getHomeDir());

    if (working_copy.find("//") == std::string::npos) return working_copy;

    std::string::size_type pos = 0;

    while ((pos = working_copy.find("//")) != std::string::npos)
        working_copy = working_copy.replace(pos, 2, "/");

    return working_copy;
}

std::string CfgValPetFile::add_suffix(const std::string& p) {
    if (p.empty()) {
        return p;
    }

    if (p.length() < 4) {
        // Since this holds, there can no ".pet"
        return p + Consts::DEFAULT_FILE_SUFFIX;
    }

    // We don't have to check for the string length, because that
    // already happened above.
    if (p.substr(p.length() - 4, 4) != Consts::DEFAULT_FILE_SUFFIX) {
        return p + Consts::DEFAULT_FILE_SUFFIX;
    }

    return p;
}

void CfgValPetFile::set(const std::string& s) {
    CfgValStr::set(add_suffix(cleanup_path(s)));
}

void CfgValPetFile::set_str(const std::string& s) { set(s); }
//
// Class CfgValBool
//
void CfgValBool::set_str(const std::string& s) {
    std::string sanitized(tolower(trim(s)));

    if (sanitized == "0" || sanitized == "false" || sanitized == "no" ||
        sanitized == "disable" || sanitized == "disabled") {
        set(false);
        return;
    }

    if (sanitized == "1" || sanitized == "true" || sanitized == "yes" ||
        sanitized == "enable" || sanitized == "enabled") {
        set(true);
        return;
    }

    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
    std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                  _("'%s' is not a valid bool"), sanitized.c_str());
    throw std::invalid_argument(msg);
}

//
// Class CfgValInt
//
void CfgValInt::set_str(const std::string& s) { set(std::atoi(s.c_str())); }

void Config::setup_map() {
    _options.clear();

    _options["load"] = &petfile;
    _options["locktimeout"] = &timeout;
    _options["checkfsecurity"] = &filesecurity;
    _options["allowlockquit"] = &allow_lock_quit;
    _options["pwinputtimeout"] = &pw_input_timeout;
    _options["pwgen_pwlen"] = &pwgenpwlen;
    _options["pwgen_letters"] = &pwgen_letters;
    _options["pwgen_digits"] = &pwgen_digits;
    _options["pwgen_punct"] = &pwgen_punct;
    _options["pwgen_special"] = &pwgen_special;
    _options["pwgen_other"] = &pwgen_other;
    _options["argon2_memory"] = &argon2_memory;
    _options["argon2_parallelism"] = &argon2_parallelism;
    _options["argon2_iterations"] = &argon2_iterations;
    _options["colors"] = &colors;
    // ignorerc can't be set in the configuration file
}

Config::Config()
    : _options{},
      petfile{std::string{}},
      timeout{Consts::DEFAULT_LOCK_TIMEOUT, Consts::MIN_LOCK_TIMEOUT,
              Consts::MIN_LOCK_TIMEOUT},
      filesecurity{Consts::DEFAULT_FILE_SECURITY},
      pwgenpwlen{Consts::DEFAULT_PASSWORD_LENGTH,
                 Consts::DEFAULT_PASSWORD_LENGTH, Consts::MIN_PASSWORD_LENGTH,
                 Consts::MAX_PASSWORD_LENGTH},
      pwgen_letters{yapet::pwgen::isLetters(Consts::DEFAULT_CHARACTER_POOLS)},
      pwgen_digits{yapet::pwgen::isDigits(Consts::DEFAULT_CHARACTER_POOLS)},
      pwgen_punct{yapet::pwgen::isPunct(Consts::DEFAULT_CHARACTER_POOLS)},
      pwgen_special{yapet::pwgen::isSpecial(Consts::DEFAULT_CHARACTER_POOLS)},
      pwgen_other{yapet::pwgen::isOther(Consts::DEFAULT_CHARACTER_POOLS)},
      allow_lock_quit{Consts::DEFAULT_ALLOW_LOCK_QUIT},
      pw_input_timeout{Consts::DEFAULT_PASSWORD_INPUT_TIMEOUT,
                       Consts::MIN_LOCK_TIMEOUT, Consts::MIN_LOCK_TIMEOUT},
      argon2_memory{Consts::DEFAULT_ARGON2_MEMORY,
                    Consts::DEFAULT_ARGON2_MEMORY, Consts::MIN_ARGON2_MEMORY},
      argon2_parallelism{Consts::DEFAULT_ARGON2_PARALLELISM,
                         Consts::DEFAULT_ARGON2_PARALLELISM,
                         Consts::MIN_ARGON2_PARALLELISM},
      argon2_iterations{Consts::DEFAULT_ARGON2_TIME_COST,
                        Consts::DEFAULT_ARGON2_TIME_COST,
                        Consts::MIN_ARGON2_TIME_COSTS},
      ignorerc{false},
      colors{} {
    setup_map();
}

Config::Config(const Config& c)
    : _options{},
      petfile{c.petfile},
      timeout{c.timeout},
      filesecurity{c.filesecurity},
      pwgenpwlen{c.pwgenpwlen},
      pwgen_letters{c.pwgen_letters},
      pwgen_digits{c.pwgen_digits},
      pwgen_punct{c.pwgen_punct},
      pwgen_special{c.pwgen_special},
      pwgen_other{c.pwgen_other},
      allow_lock_quit{c.allow_lock_quit},
      pw_input_timeout{c.pw_input_timeout},
      argon2_memory{c.argon2_memory},
      argon2_parallelism{c.argon2_parallelism},
      argon2_iterations{c.argon2_iterations},
      ignorerc{c.ignorerc},
      colors{c.colors} {
    setup_map();
}

Config::~Config() {}

Config& Config::operator=(const Config& c) {
    if (&c == this) return *this;

    petfile = c.petfile;
    timeout = c.timeout;
    filesecurity = c.filesecurity;
    pwgenpwlen = c.pwgenpwlen;
    pwgen_letters = c.pwgen_letters;
    pwgen_digits = c.pwgen_digits;
    pwgen_punct = c.pwgen_punct;
    pwgen_special = c.pwgen_special;
    pwgen_other = c.pwgen_other;
    allow_lock_quit = c.allow_lock_quit;
    pw_input_timeout = c.pw_input_timeout;
    argon2_memory = c.argon2_memory;
    argon2_parallelism = c.argon2_parallelism;
    argon2_iterations = c.argon2_iterations;
    ignorerc = c.ignorerc;
    colors = c.colors;

    setup_map();

    return *this;
}

int Config::character_pools() const {
    int pools = 0;

    if (pwgen_letters) pools |= yapet::pwgen::LETTERS;

    if (pwgen_digits) pools |= yapet::pwgen::DIGITS;

    if (pwgen_punct) pools |= yapet::pwgen::PUNCT;

    if (pwgen_special) pools |= yapet::pwgen::SPECIAL;

    if (pwgen_other) pools |= yapet::pwgen::OTHER;

    return pools;
}

void Config::lock() {
    petfile.lock();
    timeout.lock();
    filesecurity.lock();
    pwgenpwlen.lock();
    pwgen_letters.lock();
    pwgen_digits.lock();
    pwgen_punct.lock();
    pwgen_special.lock();
    pwgen_other.lock();
    allow_lock_quit.lock();
    pw_input_timeout.lock();
    argon2_iterations.lock();
    argon2_memory.lock();
    argon2_parallelism.lock();
    ignorerc.lock();
    colors.lock();
}

void Config::unlock() {
    petfile.unlock();
    timeout.unlock();
    filesecurity.unlock();
    pwgenpwlen.unlock();
    pwgen_letters.unlock();
    pwgen_digits.unlock();
    pwgen_punct.unlock();
    pwgen_special.unlock();
    pwgen_other.unlock();
    allow_lock_quit.unlock();
    pw_input_timeout.unlock();
    argon2_iterations.unlock();
    argon2_memory.unlock();
    argon2_parallelism.unlock();
    ignorerc.unlock();
    colors.unlock();
}

CfgValBase& Config::operator[](const std::string& key) {
    if (key.empty())
        throw std::invalid_argument(_("Configuration key must not be empty"));

    std::map<std::string, CfgValBase*>::iterator it = _options.find(key);

    if (it == _options.end()) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Configuration key '%s' not found"), key.c_str());
        throw std::invalid_argument(msg);
    }

    return *((*it).second);
}
