#include <map>
#include "ShannonFano.hpp"

typedef std::pair<char ,int> pair;

std::string ShannonFano::getPath(File file) {
    std::string extension;
    switch (file) {
        case ORIGIN:
            extension = helper.getExtension(type);
            break;
        case ZIP:
            extension = ".shan";
            break;
        default:
            extension = ".unshan";
            break;
    }
    return helper.PATH + helper.getFolderPath(type) + "/file" + extension;
}

void ShannonFano::encode() {
    std::map<char, int> counter;
    char byte;
    std::vector<char> bytes;
    std::ifstream input(getPath(ORIGIN), std::ios::in);
    // read bits
    while (true) {
        input >> std::noskipws >> byte;
        if (input.eof())
            break;
        ++counter[byte];
        bytes.push_back(byte);
    }
    input.close();
    
    std::vector<Node*> nodes;
    for (std::map<char, int>::iterator it = counter.begin(); it != counter.end(); ++it) {
        Node* node = new Node(it->first, it->second);
        nodes.push_back(node);
    }
    counter.clear();
    
    // sort in descending order
    std::sort(nodes.begin(), nodes.end(), [](const Node* l, const Node* r) {
        return l->chance > r->chance;
    });
    
    // compress
    buildCode(0, nodes.size() - 1, nodes);
    
    std::ofstream output(getPath(ZIP), std::ios::binary);
    std::map<char, std::string> alphabet;
    output << SPECIAL_SYMBOL;
    for (Node* node : nodes) {
        alphabet[node->byte] = node->code;
        // write alphabet in file
        output << std::noskipws << node->byte;
        for (char item : node->code)
            output << (item == '0' ? 0 : 1);
        output << SPECIAL_SYMBOL;
    }
    output << SEPARATOR << SEPARATOR; // separates alphabet and code
    
    // write results in file
    unsigned long pos = 0;
    char b = 0;
    for (char byte : bytes) {
        for(char item : alphabet[byte]) {
            if (item == '1')
                b |= (1 << (7 - (pos % 8)));
            ++pos;
            if (pos % 8 == 0) {
                output << b;
                b = 0;
            }
        }
    }
    
    // fill byte with 0 and write number of missing bits in file
    char missingBits = pos % 8;
    if (missingBits > 0) {
        while (pos % 8 != 0) {
            b = b << 1;
            ++pos;
        }
    }
    output << b;
    output << missingBits;
    output.close();
}

void ShannonFano::buildCode(int left, int right, std::vector<Node*> &nodes) {
    if (left >= right)
        return;

    auto center = findCenter(left, right, nodes);
    buildCode(left, center, nodes);
    buildCode(center + 1, right, nodes);
}

int ShannonFano::findCenter(int left, int right, std::vector<Node*> &nodes) {
    int i = left, j = right, leftSum = 0, rightSum = 0;
    while (i <= j) {
        if (leftSum < rightSum) {
            leftSum += nodes[i]->chance;
            nodes[i]->changeCode('0');
            ++i;
        } else {
            rightSum += nodes[j]->chance;
            nodes[j]->changeCode('1');
            --j;
        }
    }

    return j;
}

void ShannonFano::Node::changeCode(char c) {
    code += c;
}

void ShannonFano::decode() {
    std::map<std::string, char> alphabet;
    std::string line = "";
    char byte = 'a', previous, symbol;
    bool isNextSymbol;
    std::ifstream input(getPath(ZIP), std::ios::binary);
    std::ofstream ouput(getPath(UNZIP), std::ios::out);
    
    // read alphabet
    while (true) {
        previous = byte;
        input >> std::noskipws >> byte;
        
        if (byte == SPECIAL_SYMBOL && previous != SPECIAL_SYMBOL) {
            isNextSymbol = true;
            continue;
        }
        
        if (byte == SEPARATOR && previous == SEPARATOR)
            break;
        
        if (isNextSymbol) {
            if (line.length() > 0)
                alphabet[line] = symbol;
            symbol = byte;
            line = "";
            isNextSymbol = false;
        } else {
            line += byte;
        }
    }
    
    auto writeSymbol = [&] (int pos, char sym) {
        for (int i = 7; i >= pos; --i) {
            line += ((sym & (1 << i)) == 0 ? '0' : '1');

            if (alphabet.count(line) > 0) {
                // write symbol in file, if alphabet contains it
                ouput << alphabet[line];
                line = "";
            }
        }
    };
    
    line = "";
    input >> std::noskipws >> byte;
    // read bits
    while (true) {
        previous = byte;
        input >> std::noskipws >> byte;
        
        if (input.eof()) {
            // handle the missing bits
            writeSymbol((int) byte, previous);
            break;
        }
        
        writeSymbol(0, previous);
    }
    input.close();
    ouput.close();
}



