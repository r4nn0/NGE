#include "Mp3.h"

Mp3::Mp3(std::string path){
    if (path.substr(path.size() - 4) != ".mp3")
		throw std::invalid_argument("Can only read Mp3 files!");
	load_mp3(path);
}

void Mp3::load_mp3(std::string path){
    char byteToRead;
    FILE *fp =fopen(path.c_str(), "rb");

    fread(nullptr, 1, 1, fp); // Ignore first byte
    fread(&byteToRead, 1,1,fp); // Read second byte
    audioVersion = byteToRead>>3;
    layerIndex = (byteToRead>>1)&3;
    protectionBit = byteToRead&1;

    fread(&byteToRead, 1,1,fp);
    bitrateIndex = byteToRead>>4;
    samplingRate = (byteToRead>>2)&3;
    paddingBit = (byteToRead>>1)&1;
    privateBit = byteToRead&1;

    fread(&byteToRead, 1,1,fp);
    channelMode=byteToRead>>6;
    modeExtension=(byteToRead>>4)&3;
    copyrightBit=(byteToRead>>3)&1;
    originalBit=(byteToRead>>2)&1;
    emphasisBit=byteToRead&3;

}