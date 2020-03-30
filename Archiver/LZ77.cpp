#include "LZ77.hpp"

std::string LZ77::getPath(File file) {
    std::string extension;
    switch (file) {
        case ORIGIN:
            extension = helper.getExtension(type);
            break;
        case ZIP:
            extension = ".lz77" + std::to_string(windowSize / 1024);
            break;
        default:
            extension = ".unlz77" + std::to_string(windowSize / 1024);
            break;
    }
    return helper.PATH + helper.getFolderPath(type) + "/file" + extension;
}

void LZ77::encode() { 
    std::string s;
    char byte;
    std::ifstream input(getPath(ORIGIN), std::ios::in);
    std::ofstream output(getPath(ZIP), std::ios::out);
    // read data
    while (true) {
        input >> std::noskipws >> byte;        
        if (input.eof())
            break;
        s += byte;
    }
    input.close();
    
    // write resutls in file
    auto writeRes = [&] (int pos, int len, char symb) {
        output << std::to_string(pos) << " ";
        output << std::to_string(len) << " ";
        output << symb << " ";
    };
    
    int pos = 1;
    writeRes(0, 0, s[0]);
    while (pos < s.length()) {
        int input_len = std::min(int(s.length() - pos), inputBufferSize);
        int output_len = std::min(pos, outputBufferSize);
        std::string input = s.substr(pos, input_len);
        std::string output = s.substr(std::max(0, pos - outputBufferSize), output_len);
        auto prefix = findPrefix(input, output);
        
//        if (pos + prefix.first >= s.length()) {
//            // remove extra symbols here
//            prefix.first -= s.length() - pos;
//            if (prefix.first == 0)
//                prefix.second = 0;
//        }
        
        pos += prefix.first;
        writeRes(prefix.second, prefix.first, s[pos]);
        ++pos;
    }    
    output.close();
}

std::pair<int, int> LZ77::findPrefix(std::string s, std::string t) {
    std::pair<int, int> maxLength(0, 0); // first - length; second - position
    int lastEntry = 0;
    std::string stUnion = s + "$" + t;
    std::vector<int> br(stUnion.length());
    br[0] = 0;

    for (int i = 1; i < stUnion.length(); ++i) {
        auto temp = br[i - 1];
        while (true) {
            if (stUnion[temp] == stUnion[i]) {
                br[i] = temp + 1;
                if (i > s.length()) {
                    if (maxLength.first < br[i]) {
                        maxLength.first = br[i];
                        maxLength.second = t.length() - (i - s.length() - br[i]);
                    } else if (i == stUnion.length() - 1) {
                        lastEntry = t.length() - (i - s.length() - br[i]);
                    }
                }
                break;
            } else if (temp == 0) {
                br[i] = 0;
                break;
            } else {
                temp = br[temp - 1];
            }
        }
    }
    
//    auto insertFromInputBuffer = [&] (int pos, int &len) {
//        auto str = t.substr(t.length() - pos, pos);
//        int ind = 0;
//        while ((ind + pos < s.length() - 1) && (str[ind % pos] == s[ind + pos])) {
//            ++ind;
//            ++len;
//        }
//    };
//
//    if (maxLength.first != 0 && maxLength.second == maxLength.first) {
//        insertFromInputBuffer(maxLength.second, maxLength.first);
//    }
//    if (lastEntry != 0) {
//        int len = lastEntry;
//        insertFromInputBuffer(lastEntry, len);
//        if (len > maxLength.first) {
//            maxLength.first = len;
//            maxLength.second = lastEntry;
//        }
//    }
    
    return maxLength;
}


void LZ77::decode() {
    std::ifstream input(getPath(ZIP), std::ios::in);
    std::ofstream output(getPath(UNZIP), std::ios::out);
    std::string s, result;
    char byte;
    
    // read data
    while (!input.eof()) {
        input >> std::noskipws >> byte;
        s += byte;
    }
    input.close();
    
    auto move = [&] (int &i, int &offset) mutable {
        while (s[i + offset] != ' ')
            ++offset;
    };
    
    auto getValue = [&] (int &i, int &offset) -> int {
        move(i, offset);
        int pos = atoi(s.substr(i, offset).c_str());
        i += offset + 1;
        offset = 0;
        return pos;
    };
        
    auto insert = [&] (int pos, int dist, std::string symb) {
        if (pos == 0) {
            result.append(symb);
        } else {
            auto start = result.length() - pos;
            auto offset = std::min(dist, int(result.length() - start));
            result.append(result.substr(start, offset));
            std::string last = result.substr(result.length() - pos, pos);
            for (int i = 0; i < std::max(dist - int(offset), 0); ++i)
                result.append(last.substr(i % last.length(), 1));
            result.append(symb);
        }
    };
        
    for (int i = 0, offset = 0; i < s.length() - 1; i += offset + 1) {
        int pos = getValue(i, offset);
        int dist = getValue(i, offset);
        ++offset;
        std::string symb = s.substr(i, offset);
        insert(pos, dist, symb);
    }
    
    for (int i = 0; i < result.length(); ++i) {
        output << result[i];
    }
    output.close();
}
