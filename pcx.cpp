//
// Created by podda on 05/07/2021.
//
#include "pcx.h"
#include <vector>
#include <fstream>
#include <cmath>

class PCX {
    std::ifstream& inputFile;
    mat<uint8_t>& matrix;

    template<class T>
    void readType(T &element) {

        //inputFile >> element;                                                 SBAGLIATO!!!!!!!!!!!!!!!!!!
        inputFile.read(reinterpret_cast<char *>(&element), sizeof(element));    // GIUSTO =)
    }


    uint8_t manufacturer, version, encoding, bitsPerPlane, reserved, colorPlanes;
    uint16_t windowXMin, windowYMin, windowXMax, windowYMax, vertDPI, horzDPI, bytesPerPlaneLine, paletteInfo, horScrSize, verScrSize;
    std::vector<uint8_t> palette;
    std::vector<unsigned char> padding;

    bool isRun(unsigned char c){
        if( (c & 0b11000000) == 0b11000000){
            return true;
        }
        else
            return false;
    }

    void writeByteToMatrix(unsigned int& pos, unsigned char tmp){
        for(int i = 7; i >= 0; --i){
            unsigned char mask = static_cast<unsigned char>(pow(2,i));
            if((tmp & mask) == mask)
                matrix[pos]= 255;
            else
                matrix[pos] = 0;
            ++pos;
        }
    }
public:
    int xSize, ySize, totalBytes;
    PCX(std::ifstream& input, mat<uint8_t>& matr) : inputFile(input), matrix(matr){
        readType(manufacturer);
        readType(version);
        readType(encoding);
        readType(bitsPerPlane);
        readType(windowXMin);
        readType(windowYMin);
        readType(windowXMax);
        readType(windowYMax);
        readType(vertDPI);
        readType(horzDPI);
        for(unsigned int i = 0; i < 48; ++i){
            uint8_t tmp;
            readType(tmp);
            palette.push_back(tmp);
        }
        readType(reserved);
        readType(colorPlanes);
        readType(bytesPerPlaneLine);
        readType(paletteInfo);
        readType(horScrSize);
        readType(verScrSize);
        for(unsigned int i = 0; i < 54; ++i){
            unsigned char tmp;
            readType(tmp);
            padding.push_back(tmp);
        }

        xSize = windowXMax - windowXMin + 1;
        ySize = windowYMax - windowYMin + 1;
        totalBytes = colorPlanes * bytesPerPlaneLine;
        std::cout << "X size: " << xSize << "\nY size: " << ySize << "\nTotal bytes: " << totalBytes << std::endl;
        matrix.empty();
        matrix.resize(ySize, xSize);
    }

    void decode(){

        unsigned int bufferPosition = 0;
        unsigned int position = 0;

        for(unsigned int k = 0; k < ySize; ++k){

            if(bufferPosition >= totalBytes)
                continue;

            unsigned char tmp;

            readType(tmp);

            if(isRun(tmp)){
                tmp = tmp & 0b00111111;
                unsigned char tmp2;
                readType(tmp2);

                for(unsigned int i = 0; i < tmp; ++i){
                    if(bufferPosition < totalBytes){
                        writeByteToMatrix(position, tmp2);
                        ++bufferPosition;
                    }
                    else {
                        break;
                    }
                }

            }
            else{
                writeByteToMatrix(position, tmp);
                bufferPosition++;
            }
        }

    }

};



bool load_pcx(const std::string& filename, mat<uint8_t>& img){
    std::ifstream input;
    input.open(filename, std::ios::binary);

    if(!input){
        std::cout<< "Error reading .pcx file" << std::endl;
        return false;
    }

    PCX pcx(input, img);

    pcx.decode();

    return true;
}

