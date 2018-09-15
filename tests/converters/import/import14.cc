// $Id$
//
// Tests whether or not strange input make the import choke (using double
// quotes)
//
// Relies on test9.csv

#include <iostream>
#include <typeinfo>

#include "csvimport.h"
#include "testpaths.h"

int main(int, char**) {
    try {
        CSVImport imp(SRCDIR "/test9.csv", "test9.pet", ',');
        imp.import("test9");

        if (imp.hadErrors()) {
            imp.printLog();
            return 1;
        }

        if (imp.numErrors() != 0) return 1;
    } catch (std::exception& ex) {
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}