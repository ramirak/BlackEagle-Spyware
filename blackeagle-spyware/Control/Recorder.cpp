// Uses the MCI_OPEN, MCI_RECORD, and MCI_SAVE commands to record and
// save a waveform-audio file. Returns 0L if successful; otherwise,
// it returns an MCI error code.

#include "Recorder.h"

DWORD recordAudio(DWORD seconds, const char* path)
{
	HWAVEIN microHandle;
	WAVEHDR waveHeader;

	const int NUMPTS = 22050 * seconds;  
	int sampleRate = 22050;
	short int *waveIn = (short int*) malloc(sizeof(short int) * NUMPTS);   // 'short int' is a 16-bit type;
								// for 8-bit capture, we'd use 'unsigned char' or 'BYTE' 8-bit types

	MMRESULT result = 0;

	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM;      // simple, uncompressed format
	format.wBitsPerSample = 8;                //  16 for high quality, 8 for telephone-grade
	format.nChannels = 1;                     //  1=mono, 2=stereo
	format.nSamplesPerSec = sampleRate;       //  22050
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nChannels * format.wBitsPerSample / 8;
	// = nSamplesPerSec * n.Channels * wBitsPerSample/8
	format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
	// = n.Channels * wBitsPerSample/8
	format.cbSize = 0;

	result = waveInOpen(&microHandle, WAVE_MAPPER, &format, 0L, 0L, WAVE_FORMAT_DIRECT);

	if (result)
	{
		return 0;
	}

	// Set up and prepare header for input
	waveHeader.lpData = (LPSTR)waveIn;
	waveHeader.dwBufferLength = NUMPTS * 2;
	waveHeader.dwBytesRecorded = 0;
	waveHeader.dwUser = 0L;
	waveHeader.dwFlags = 0L;
	waveHeader.dwLoops = 0L;
	waveInPrepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR));

	// Insert a wave input buffer
	result = waveInAddBuffer(microHandle, &waveHeader, sizeof(WAVEHDR));

	if (result)
	{
		return 0;
	}

	result = waveInStart(microHandle);

	if (result)
	{
		return 0;
	}

	// Wait until finished recording
	do {} while (waveInUnprepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);

	waveInClose(microHandle);

	WavHeader header = {};
	header.length_of_fmt = 16;
	header.format_type = 1;
	header.channels = 1;
	header.sample_rate = sampleRate;
	header.bits_per_sample = 8;
	header.byterate = header.sample_rate * header.channels * (header.bits_per_sample / 8);
	header.block_align = (header.bits_per_sample / 8) * header.channels;
	header.data_size = sizeof(short int) * NUMPTS;
	header.overall_size = (header.data_size);

	FILE* out;
	fopen_s(&out, path, "wb");
	if (out == NULL) {
		return 0;
	}
	fwrite(&header, sizeof(header), 1, out);
	fseek(out, 0, SEEK_END);
	fwrite(waveIn, sizeof(short int), NUMPTS, out);
	fclose(out);
	free(waveIn);
	return 0;
}