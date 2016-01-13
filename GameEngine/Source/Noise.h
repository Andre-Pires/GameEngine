#pragma once

#include "Utilities.h"

class PerlinNoise {
private:
	float persistence;
	int octaves;

	float Linear_Interpolate(float a, float b, float x);
	float Noise(int x, int y, int z);
	float SmoothedNoise(float x, float y, float z);
	float InterpolatedNoise(float x, float y, float z);
	float PerlinNoise3D(float x, float y, float z);

public:
	float noise[32 * 32 * 32];

	PerlinNoise(float persistence, int octaves);
	void calculateNoise(int size);
};
