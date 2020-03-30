#include <cmath>
#include "Helper.hpp"

std::string Helper::getFolderPath(FileType type) {
    switch (type) {
        case TEXT:
            return "txt";
        case DOCX:
            return "docx";
        case PPTX:
            return "pptx";
        case PDF:
            return "pdf";
        case EXE:
            return "exe";
        case JPG:
            return "jpg";
        case WBJPG:
            return "wbjpg";
        case BMP:
            return "bmp";
        case WBBMP:
            return "wbbmp";
        case KEY:
            return "keynote";
    }
}

std::string Helper::getExtension(FileType type) {
    switch (type) {
        case WBJPG:
            return "." + getFolderPath(JPG);
        case WBBMP:
            return "." + getFolderPath(BMP);
        case KEY:
            return ".key";
        default:
            return "." + getFolderPath(type);
    }
}

FileType* Helper::getAllTypes() {
    FileType* arr = new FileType[NUMBER_OF_TYPES] { TEXT, DOCX, PPTX, PDF, EXE, JPG, WBJPG, BMP, WBBMP, KEY };
    return arr;
}

void Helper::calculateFrequency() {
    FileType* types = getAllTypes();
    std::string outputPath = "results.txt";
    std::ofstream output(outputPath, std::ios::out);
    for (int i = 0; i < NUMBER_OF_TYPES; ++i) {
        std::string inputPath = PATH + getFolderPath(types[i]) + "/file" + getExtension(types[i]);
        std::map<char, int> counter;
        char byte;
        std::ifstream input(inputPath, std::ios::in);
        double entropy = 0, probability;
        long numberOfSymbols = 0;
        // read bits
        while (!input.eof()) {
            input >> std::noskipws >> byte;
            ++counter[byte];
            ++numberOfSymbols;
        }
        input.close();
        
        // write results
        output << "#######" << getExtension(types[i]) << "#######\n";
        for (std::map<char, int>::iterator it = counter.begin(); it != counter.end(); ++it) {
            probability = static_cast<double>(it->second) / numberOfSymbols;
            output << static_cast<int>(it->first) + 128 << " " << probability << "\n";
            entropy += probability * log2(probability);
        }
        output << "Entropy: " << -entropy << "\n\n";
        counter.clear();
    }
    output.close();
}
