#include "Som.h"

Som::Som()
{
}


Som::~Som()
{
}

void Som::Backingtrack(Audio audio) {
	// Criem uma fun��o void InitAudio() com o seguinte c�digo
	audio.buffer = alutCreateBufferFromFile("Survivor-Burning Heart.wav");
	alGenSources(1, &audio.source);
	alSourcei(audio.source, AL_BUFFER, audio.buffer);
	alSourcef(audio.source, AL_GAIN, 0.2);
	alSourcePlay(audio.source);
}
