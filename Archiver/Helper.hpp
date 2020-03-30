#ifndef Helper_hpp
#define Helper_hpp

#include <string>
#include <map>
#include <vector>
#include <fstream>

enum FileType {
    TEXT, DOCX, PPTX, PDF, EXE, JPG, WBJPG, BMP, WBBMP, KEY
};

struct Helper {
    const std::string PATH = "DATA/";
    const int NUMBER_OF_TYPES = 10;
    
    std::string getFolderPath(FileType type);
    std::string getExtension(FileType type);
    FileType* getAllTypes();
    void calculateFrequency();
};

#endif /* Helper_hpp */
