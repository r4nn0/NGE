#ifndef WAVE_HEADER
#define WAVE_HEADER
//#define _CRT_SECURE_NO_DEPRECATE

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fstream>
#pragma once

class Wave {
public:
	// Constructors
	Wave(std::string str);
	Wave() {};

	// Read / Write files
	void load_wav(std::string str);

	// Get functions
	// Subchunk2Size = NumSamples * NumChannels * BitsPerSample / 8 <==> NumSamples = Subchunk2Size / (NumChannels*(BitsPerSample / 8))
	unsigned get_size() { return size; }
	unsigned get_sample_rate() { return SampleRate; }
	unsigned get_n_channels() { return NumChannels; }
	int get_al_format(){
		if(BitsPerSample==16){
			if(NumChannels>1) return 0x1103;
			else return 0x1101;
		}
		else if(BitsPerSample==8){
			if(NumChannels>1) return 0x1102;
			else return 0x1100;
		}
		return -1;
	}
	short* getData(){ return data;}
	// Set Functions
	void set_sample_rate(int n) { SampleRate = n; }
	void set_n_channels(int n) { if (n != 1 && n != 2)throw std::invalid_argument("n can only be 1 (MONO) or 2 (STEREO)!"); NumChannels = n; }

	// Overloaded Operators
protected:
	char type[5];
	char format[5];
	char Subchunk1ID[5];
	char Subchunk2ID[5];

	int ChunkSize;
	int Subchunk1Size;
	int SampleRate;
	int ByteRate;
	int Subchunk2Size;

	short AudioFormat;
	short NumChannels;
	short BlockAlign;
	short BitsPerSample;
	unsigned size;
	// utility
	unsigned NumSamples;

	short* data;
};
#endif