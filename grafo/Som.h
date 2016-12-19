#pragma once
#include <AL\alut.h>

typedef struct {
	ALuint buffer, source;
	ALboolean tecla_s;
} Audio;

class Som
{
public:
	Som();
	~Som();
	void Backingtrack(Audio audioS);
};

