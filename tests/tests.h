// -*- c++ -*-
//
// $Id$
//
// This file holds defines common to all tests
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <partdec.h>
#include <structs.h>
#include <key.h>
#include <crypt.h>

#define ROUNDS 5000
#define FN "encryptiontest.gps"
#define NAME "Test name %d"
#define HOST "Test host %d"
#define UNAME "Test username %d"
#define PW "Test password %d"
#define COMMENT "Test comment %d"


inline
void progress() {
    static int n = 0;

    switch (n) {
    case 0:
    case 1:
	std::cout << "\\";
	break;
    case 2:
    case 3:
	std::cout << "|";
	break;
    case 4:
    case 5:
	std::cout << "/";
	break;
    case 6:
    case 7:
	std::cout << "-";
	break;
    case 8:
    case 9:
	std::cout << "|";
	n=-1;
	break;
    }
    n++;
}

inline
void print_record(YAPET::PartDec& pd, const YAPET::Key& key) {
    YAPET::Crypt crypt(key);

    std::cout << "PartDec Name:\t" << pd.getName() << std::endl;
    
    const YAPET::BDBuffer& enc_rec = pd.getEncRecord();
    YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = crypt.decrypt<YAPET::PasswordRecord>(enc_rec);
    YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

    std::cout << "\tName:\t" << ptr_pw->name << std::endl;
    std::cout << "\tHost:\t" << ptr_pw->host << std::endl;
    std::cout << "\tUname:\t" << ptr_pw->username << std::endl;
    std::cout << "\tPW:\t" << ptr_pw->password << std::endl;
    std::cout << "\tCMT:\t" << ptr_pw->comment << std::endl;
    std::cout << std::endl;

    delete ptr_dec_rec;
}

inline
void check_record(YAPET::PartDec& pd, const YAPET::Key& key, int rec_no) {
    progress();

    YAPET::Crypt crypt(key);

    char _name[YAPET::NAME_SIZE];
    char _host[YAPET::HOST_SIZE];
    char _uname[YAPET::USERNAME_SIZE];
    char _pw[YAPET::PASSWORD_SIZE];
    char _cmt[YAPET::COMMENT_SIZE];

    snprintf(_name, YAPET::NAME_SIZE, NAME, rec_no);
    snprintf(_host, YAPET::HOST_SIZE, HOST, rec_no);
    snprintf(_uname, YAPET::USERNAME_SIZE, UNAME, rec_no);
    snprintf(_pw, YAPET::PASSWORD_SIZE, PW, rec_no);
    snprintf(_cmt, YAPET::COMMENT_SIZE, COMMENT, rec_no);

    if (strncmp(_name, (char*)pd.getName(), YAPET::NAME_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("Name does not match");
    }
	
    const YAPET::BDBuffer& enc_rec = pd.getEncRecord();
    YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = crypt.decrypt<YAPET::PasswordRecord>(enc_rec);
    YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

    if (strncmp(_name, (char*)ptr_pw->name, YAPET::NAME_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("Name does not match");
    }

    if (strncmp(_host, (char*)ptr_pw->host, YAPET::HOST_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("Host does not match");
    }

    if (strncmp(_uname, (char*)ptr_pw->username, YAPET::USERNAME_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("username does not match");
    }

    if (strncmp(_pw, (char*)ptr_pw->password, YAPET::PASSWORD_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("password does not match");
    }

    if (strncmp(_cmt, (char*)ptr_pw->comment, YAPET::COMMENT_SIZE) != 0) {
	print_record(pd, key);
	throw std::logic_error("comment does not match");
    }

    delete ptr_dec_rec;

    std::cout << "\r";
}