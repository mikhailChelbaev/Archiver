// КДЗ по дисциплине Алгоритмы и структуры данных, 2019-2020 уч.год
// Челбаев Михаил Александрович, группа БПИ182, дата (28.03.2020)
// Среда разработки: Xcode
// Состав проекта: main.cpp, Archiver.h, Archiver.cpp, ShannonFano.hpp, ShannonFano.cpp, LZ77.hpp, LZ77.cpp, Helper.hpp, Helper.cpp
// Что сделано (сжатие и распаковка методом Хаффмана / сжатие и распаковка методом Шеннона - Фано, сжатие и распаковка методом LZ77, сжатие и распаковка методом LZW, проведен вычислительный эксперимент, построены таблицы и графики, для измерения времени выполнения использовалось ХХХ, оформлен отчет)
// Что не сделано (см. список выше)

#include <clocale>
#include <ctime>
#include <time.h>
#include "ShannonFano.hpp"
#include "LZ77.hpp"

const int NUMBER_OF_TESTS = 2;

void calculateTime(std::string path, Archiver* arc);

int main(int argc, const char * argv[]) {
    std::string path;

//    path = "/timing_shan.txt";
//    calculateTime(path, new ShannonFano());
    LZ77 lz(5 * 1024, 4 * 1024);
    lz.setType(JPG);
    lz.encode();
    lz.decode();
    
//    path = "/timing_lz775.txt";
//    calculateTime(path, new LZ77(5 * 1024, 4 * 1024));
    
//    path = "/timing_lz7710.txt";
//    calculateTime(path, new LZ77(10 * 1024, 8 * 1024));
//
//    path = "/timing_lz7720.txt";
//    calculateTime(path, new LZ77(20 * 1024, 16 * 1024));
    
//    helper.calculateFrequency();

    return 0;
}

void calculateTime(std::string pathEnding, Archiver* arc) {
    Helper helper;
    FileType* types = helper.getAllTypes();
    clock_t startTime, encodeTime, decodeTime;
    time_t sTime, eTime, dTime;
    clock_t encode[NUMBER_OF_TESTS], decode[NUMBER_OF_TESTS];
    time_t e[NUMBER_OF_TESTS], d[NUMBER_OF_TESTS];
    std::cout << "########################\n";
    for (int i = 0; i < helper.NUMBER_OF_TYPES; ++i) {
        std::cout << "Working with file of type " << helper.getFolderPath(types[i]) << "\n";
        arc->setType(types[i]);
        for (int j = 0; j < NUMBER_OF_TESTS; ++j) {
            std::cout << j << "\n";
            std::cout << "Encoding file\n";
            sTime = time(NULL);
            startTime = clock();
            arc->encode();
            encodeTime = clock() - startTime;
            eTime = time(NULL) - sTime;
            std::cout << "Encoding: " << encodeTime << "\n";
            sTime = time(NULL);
            startTime = clock();
            std::cout << "Decoding file\n";
            arc->decode();
            decodeTime = clock() - startTime;
            dTime = time(NULL) - sTime;
            std::cout << "Decoding: " << decodeTime << "\n";

            encode[j] = encodeTime;
            decode[j] = decodeTime;
            e[j] = eTime;
            d[j] = dTime;
        }

        encodeTime = 0; decodeTime = 0;
        eTime = 0; dTime = 0;
        for (int j = 0; j < NUMBER_OF_TESTS; ++j) {
            encodeTime += encode[j];
            decodeTime += decode[j];
            eTime += e[j];
            dTime += d[j];
        }

        std::string path = helper.PATH + helper.getFolderPath(types[i]) + pathEnding;
        std::ofstream input(path, std::ios::out);
        input << "Encoding time: " << encodeTime / NUMBER_OF_TESTS << "ms or " << eTime / NUMBER_OF_TESTS << "s\n";
        input << "Decoding time: " << decodeTime / NUMBER_OF_TESTS << "ms or " << dTime / NUMBER_OF_TESTS << "s\n";
        input.close();
    }
    
    delete [] types;
    delete arc;
}



