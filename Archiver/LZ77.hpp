#ifndef LZ77_hpp
#define LZ77_hpp

#include "Archiver.h"

class LZ77: public Archiver {
private:
    int inputBufferSize, outputBufferSize, windowSize;
    
    std::pair<int, int> findPrefix(std::string s, std::string t);

public:
    LZ77(int window, int outputBuffer) : windowSize(window), inputBufferSize(window - outputBuffer), outputBufferSize(outputBuffer) {}
    
    void encode() override;
    void decode() override;
    std::string getPath(File file) override;
};

#endif /* LZ77_hpp */
