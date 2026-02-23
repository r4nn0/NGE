#include "Wave.h"

Wave::Wave(std::string str) {
	if (str.substr(str.size() - 4) != ".wav")
		throw std::invalid_argument("Can only read WAV files!");
	load_wav(str);
}

void Wave::load_wav(std::string str) {
	FILE *fp =fopen(str.c_str(), "rb");

	// Chunk
	fread(type, sizeof(char), 4, fp);
	type[4]=0;
	if (strcmp(type, "RIFF"))
		throw std::runtime_error("Not a RIFF file!");

	fread(&ChunkSize, sizeof(char), 4, fp);
	fread(format, sizeof(char), 4, fp);
	format[4]=0;
	if (strcmp(format, "WAVE"))
		throw std::runtime_error("Not a WAVE format!");

	// 1st Subchunk
	fread(Subchunk1ID, sizeof(char), 4, fp);
	Subchunk1ID[4]=0;
	if (strcmp(Subchunk1ID, "fmt "))
		throw std::runtime_error("Missing fmt header!");
	fread(&Subchunk1Size, sizeof(int), 1, fp);
	fread(&AudioFormat, sizeof(short), 1, fp);
	fread(&NumChannels, sizeof(short), 1, fp);
	fread(&SampleRate, sizeof(int), 1, fp);
	fread(&ByteRate, sizeof(int), 1, fp);
	fread(&BlockAlign, sizeof(short), 1, fp);
	fread(&BitsPerSample, sizeof(short), 1, fp);

	// 2nd Subchunk
	fread(Subchunk2ID, sizeof(char), 4, fp);
	Subchunk2ID[4]=0;
	if (strcmp(Subchunk2ID, "data")!=0)
		throw std::runtime_error("Missing data header!");
	fread(&Subchunk2Size, sizeof(int), 1, fp);
	// Data

	//Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8
	int NumSamples = Subchunk2Size / (NumChannels*(BitsPerSample / 8));

	data = (short*)malloc(NumSamples*2*sizeof(short));
	size = NumSamples*2;
	for (int i = 0; i < NumSamples*2; i++) {
		fread(data+i, sizeof(short), 1, fp);
	}
	fclose(fp);
}