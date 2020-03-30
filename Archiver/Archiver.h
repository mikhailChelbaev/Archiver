#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Helper.hpp"

#ifndef Archiver_h
#define Archiver_h

class Archiver {
protected:
    FileType type;
    Helper helper;

public:
    
    enum File {
        ORIGIN, ZIP, UNZIP
    };
    
    virtual void encode() {};
    virtual void decode() {};
    virtual std::string getPath(File file) { return ""; };

    void setType(FileType value);
};

#endif /* Archiver_h */
