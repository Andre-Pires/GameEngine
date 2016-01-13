#include "Noise.h"

PerlinNoise::PerlinNoise(float persistence, int octaves) {
	this->persistence = persistence;
	this->octaves = octaves;
}

void PerlinNoise::calculateNoise(int size) {
	float n;

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			n = 0;
			for (int z = 0; z < size; z++) {
				n = PerlinNoise3D(x, y, z);
				n = n - floor(n);

				float color = floor(255 * n);
				noise[x + (y * size) + (z * size * size)] = color / 256;
			}
		}
	}
}

float PerlinNoise::Linear_Interpolate(float a, float b, float x) {
	return  a*(1 - x) + b*x;
}

float PerlinNoise::Noise(int x, int y, int z) {
	int n = x + y * 57 + z * 131;
	n = (n << 13) ^ n;
	float r3 = 0.000000000731322574615478515625 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.000000000931322574615478515625 - 0.000000000831322574615478515625)));
	int r4 = 15000 + rand() % (16500 - 15000);

	return (1.0f - ((n * (n * n * r4 + 789221) + 1376312589) & 0x7fffffff)* r3);
}

float PerlinNoise::SmoothedNoise(float x, float y, float z) {
	float fractX = x - int(x);
	float fractY = y - int(y);
	float fractZ = z - int(z);

	int x1 = (int(x) + 256) % 256;
	int y1 = (int(y) + 256) % 256;
	int z1 = (int(z) + 3) % 4;

	int x2 = (x1 + 255) % 256;
	int y2 = (y1 + 255) % 256;
	int z2 = (z1 + 3) % 4;

	float value = 0.0f;
	value += fractX       * fractY       * fractZ       * Noise(x1, y1, z1);
	value += fractX       * (1 - fractY) * fractZ       * Noise(x1, y2, z1);
	value += (1 - fractX) * fractY       * fractZ       * Noise(x2, y1, z1);
	value += (1 - fractX) * (1 - fractY) * fractZ       * Noise(x2, y2, z1);

	value += fractX       * fractY       * (1 - fractZ) * Noise(x1, y1, z2);
	value += fractX       * (1 - fractY) * (1 - fractZ) * Noise(x1, y2, z2);
	value += (1 - fractX) * fractY       * (1 - fractZ) * Noise(x2, y1, z2);
	value += (1 - fractX) * (1 - fractY) * (1 - fractZ) * Noise(x2, y2, z2);

	return value;
}

float PerlinNoise::InterpolatedNoise(float x, float y, float z) {
	float v1 = SmoothedNoise(x, y, z);
	float v2 = SmoothedNoise(x + 1, y, z);
	float v3 = SmoothedNoise(x, y + 1, z);
	float v4 = SmoothedNoise(x, y, z + 1);
	float v5 = SmoothedNoise(x + 1, y + 1, z);
	float v6 = SmoothedNoise(x + 1, y, z + 1);
	float v7 = SmoothedNoise(x, y + 1, z + 1);
	float v8 = SmoothedNoise(x + 1, y + 1, z + 1);

	float fractX = x - int(x);
	float fractY = y - int(y);
	float fractZ = z - int(z);

	float i1 = Linear_Interpolate(v1, v2, fractX);
	float i2 = Linear_Interpolate(v3, v4, fractX);
	float i3 = Linear_Interpolate(v5, v6, fractX);
	float i4 = Linear_Interpolate(v7, v8, fractX);

	float i5 = Linear_Interpolate(i1, i2, fractY);
	float i6 = Linear_Interpolate(i3, i4, fractY);

	return Linear_Interpolate(i5, i6, fractZ);
}

float PerlinNoise::PerlinNoise3D(float x, float y, float z) {
	float total = 0;

	float p = persistence;

	for (int i = 0; i < octaves; i++) {
		float frequency = pow(2, i);
		float amplitude = pow(p, i);

		total = total + InterpolatedNoise(x * frequency, y * frequency, z * frequency) * amplitude;
	}
	return total;
}