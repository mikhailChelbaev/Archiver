#ifndef ShannonFano_hpp
#define ShannonFano_hpp

#include <string>
#include "Archiver.h"

class ShannonFano: public Archiver {

private:
    
    struct Node {
        char byte;
        int chance;
        std::string code;
        
        Node(char byte, int chance) {
            this->byte = byte;
            this->chance = chance;
        }
        
        void changeCode(char c);
    };
    
    const char SEPARATOR = '$';
    const char SPECIAL_SYMBOL = ':'; // should be equal to separator
    
    void buildCode(int left, int right, std::vector<Node*> &nodes);
    int findCenter(int left, int right, std::vector<Node*> &nodes);

public:
    void encode() override;
    void decode() override;
    std::string getPath(File file) override;
};

#endif /* ShannonFano_hpp */
