#ifndef MP3_HEADER
#define MP3_HEADER
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fstream>

class Mp3{
public:
    Mp3(std::string);
    void load_mp3(std::string);

protected:
    char audioVersion;      // 2bits
    char layerIndex;        // 2bits
    char protectionBit;     // 1bit

    char bitrateIndex;      // 4bits
    char samplingRate;      // 2bits
    char paddingBit;        // 1bit
    char privateBit;        // 1bit

    char channelMode;       // 2bits
    char modeExtension;     // 2bits
    char copyrightBit;      // 1bit
    char originalBit;       // 1bit
    char emphasisBit;       // 2bits
}

#endif