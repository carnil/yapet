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

#ifndef _MAINWINDOWHOTKEYS_H
#define _MAINWINDOWHOTKEYS_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>

#include "changepassword.h"
#include "createfile.h"
#include "loadfile.h"
#include "mainwindow.h"

// QUIT
class HotKeyQ : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyQ(MainWindow& r) : HotKey('Q'), ref(r) {}
    HotKeyQ(const HotKeyQ& hkq) : HotKey(hkq), ref(hkq.ref) {}
    void action() { ref.quit(); }

    HotKey* clone() const { return new HotKeyQ(*this); }
};

class HotKeyq : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyq(MainWindow& r) : HotKey('q'), ref(r) {}
    HotKeyq(const HotKeyq& hkq) : HotKey(hkq), ref(hkq.ref) {}
    void action() { ref.quit(); }

    HotKey* clone() const { return new HotKeyq(*this); }
};

// HELP
class HotKeyH : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyH(MainWindow& r) : HotKey('H'), ref(r) {}
    HotKeyH(const HotKeyH& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_help(); }

    HotKey* clone() const { return new HotKeyH(*this); }
};

class HotKeyh : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyh(MainWindow& r) : HotKey('h'), ref(r) {}
    HotKeyh(const HotKeyh& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_help(); }

    HotKey* clone() const { return new HotKeyh(*this); }
};

// INFO
class HotKeyI : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyI(MainWindow& r) : HotKey('I'), ref(r) {}
    HotKeyI(const HotKeyI& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_info(); }

    HotKey* clone() const { return new HotKeyI(*this); }
};

class HotKeyi : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyi(MainWindow& r) : HotKey('i'), ref(r) {}
    HotKeyi(const HotKeyi& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_info(); }

    HotKey* clone() const { return new HotKeyi(*this); }
};

// READ FILE
class HotKeyR : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyR(MainWindow& r) : HotKey('R'), ref(r) {}
    HotKeyR(const HotKeyR& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() {
        // LoadFile does apoptosis
        (new LoadFile(ref))->run();
    }

    HotKey* clone() const { return new HotKeyR(*this); }
};

class HotKeyr : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyr(MainWindow& r) : HotKey('r'), ref(r) {}
    HotKeyr(const HotKeyr& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() {
        // LoadFile does apoptosis
        (new LoadFile(ref))->run();
    }

    HotKey* clone() const { return new HotKeyr(*this); }
};

// CREATE FILE
class HotKeyE : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyE(MainWindow& r) : HotKey('E'), ref(r) {}
    HotKeyE(const HotKeyE& hkc) : HotKey(hkc), ref(hkc.ref) {}

    void action() {
        // CreateFile does apoptosis
        (new CreateFile(ref))->run();
    }

    HotKey* clone() const { return new HotKeyE(*this); }
};

class HotKeye : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeye(MainWindow& r) : HotKey('e'), ref(r) {}
    HotKeye(const HotKeye& hkc) : HotKey(hkc), ref(hkc.ref) {}

    void action() {
        // CreateFile does apoptosis
        (new CreateFile(ref))->run();
    }

    HotKey* clone() const { return new HotKeye(*this); }
};

// SAVE FILE
class HotKeyS : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyS(MainWindow& r) : HotKey('S'), ref(r) {}
    HotKeyS(const HotKeyS& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.save_records(); }

    HotKey* clone() const { return new HotKeyS(*this); }
};

class HotKeys : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeys(MainWindow& r) : HotKey('s'), ref(r) {}
    HotKeys(const HotKeys& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.save_records(); }

    HotKey* clone() const { return new HotKeys(*this); }
};

// ADD RECORD
class HotKeyA : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyA(MainWindow& r) : HotKey('A'), ref(r) {}
    HotKeyA(const HotKeyA& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_password_record(false); }

    HotKey* clone() const { return new HotKeyA(*this); }
};

class HotKeya : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeya(MainWindow& r) : HotKey('a'), ref(r) {}
    HotKeya(const HotKeya& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_password_record(false); }

    HotKey* clone() const { return new HotKeya(*this); }
};

// SORT ORDER
class HotKeyO : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyO(MainWindow& r) : HotKey('O'), ref(r) {}
    HotKeyO(const HotKeyO& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.sort_asc(!ref.sort_asc()); }

    HotKey* clone() const { return new HotKeyO(*this); }
};

class HotKeyo : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyo(MainWindow& r) : HotKey('o'), ref(r) {}
    HotKeyo(const HotKeyo& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.sort_asc(!ref.sort_asc()); }

    HotKey* clone() const { return new HotKeyo(*this); }
};

// SEARCH
class HotKeySearch : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeySearch(MainWindow& r) : HotKey('/'), ref(r) {}
    HotKeySearch(const HotKeySearch& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.search_first(); }

    HotKey* clone() const { return new HotKeySearch(*this); }
};

// SEARCH NEXT
class HotKeyN : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyN(MainWindow& r) : HotKey('N'), ref(r) {}
    HotKeyN(const HotKeyN& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.search_next(); }

    HotKey* clone() const { return new HotKeyN(*this); }
};

class HotKeyn : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyn(MainWindow& r) : HotKey('n'), ref(r) {}
    HotKeyn(const HotKeyn& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.search_next(); }

    HotKey* clone() const { return new HotKeyn(*this); }
};

// DELETE
class HotKeyD : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyD(MainWindow& r) : HotKey('D'), ref(r) {}
    HotKeyD(const HotKeyD& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.delete_selected(); }

    HotKey* clone() const { return new HotKeyD(*this); }
};

class HotKeyd : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyd(MainWindow& r) : HotKey('d'), ref(r) {}
    HotKeyd(const HotKeyd& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.delete_selected(); }

    HotKey* clone() const { return new HotKeyd(*this); }
};

// CHANGE PASSWORD
class HotKeyC : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyC(MainWindow& r) : HotKey('C'), ref(r) {}
    HotKeyC(const HotKeyC& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() {
        // ChangePassword does apoptosis
        (new ChangePassword(ref))->run();
    }

    HotKey* clone() const { return new HotKeyC(*this); }
};

class HotKeyc : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyc(MainWindow& r) : HotKey('c'), ref(r) {}
    HotKeyc(const HotKeyc& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() {
        // ChangePassword does apoptosis
        (new ChangePassword(ref))->run();
    }

    HotKey* clone() const { return new HotKeyc(*this); }
};

// LOCK SCREEN
class HotKeyL : public YACURS::HotKey {
   public:
    HotKeyL() : HotKey('L') {}
    HotKeyL(const HotKeyL& hkh) : HotKey(hkh) {}

    void action() {
        // cheat by simulating an alarm event
        YACURS::EventQueue::submit(YACURS::EVT_SIGALRM);
    }

    HotKey* clone() const { return new HotKeyL(*this); }
};

class HotKeyl : public YACURS::HotKey {
   public:
    HotKeyl() : HotKey('l') {}
    HotKeyl(const HotKeyl& hkh) : HotKey(hkh) {}

    void action() {
        // cheat by simulating an alarm event
        YACURS::EventQueue::submit(YACURS::EVT_SIGALRM);
    }

    HotKey* clone() const { return new HotKeyl(*this); }
};

// PASSWORD GENERATOR
class HotKeyG : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyG(MainWindow& r) : HotKey('G'), ref(r) {}
    HotKeyG(const HotKeyG& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_pwgen(); }

    HotKey* clone() const { return new HotKeyG(*this); }
};

class HotKeyg : public YACURS::HotKey {
   private:
    MainWindow& ref;

   public:
    HotKeyg(MainWindow& r) : HotKey('g'), ref(r) {}
    HotKeyg(const HotKeyg& hkh) : HotKey(hkh), ref(hkh.ref) {}

    void action() { ref.show_pwgen(); }

    HotKey* clone() const { return new HotKeyg(*this); }
};

#endif  // _MAINWINDOWHOTKEYS_H
