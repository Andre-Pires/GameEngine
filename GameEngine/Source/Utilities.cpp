#include "Utilities.h"
#include <sstream>
#include <random>

namespace Utilities
{
	GLchar* loadFile(char* fileName) {
		std::string fileLine, result;
		std::ifstream shaderFile(fileName);
		char* shaderContents;
		int size;

		if (shaderFile.is_open()) {
			while (getline(shaderFile, fileLine)) {
				result += fileLine + "\n";
			}
			shaderFile.close();
		}
		size = result.size();
		shaderContents = new char[size];
		memcpy(shaderContents, result.c_str(), size);
		shaderContents[size - 1] = '\0';

		return shaderContents;
	}

	std::default_random_engine& global_urng()
	{
		static std::default_random_engine u{};
		return u;
	}

	void randomize()
	{
		static std::random_device rd{};
		global_urng().seed(rd());
	}

	int random_uniform(int from, int thru)
	{
		static std::uniform_int_distribution<> d{};
		using param_t = decltype(d)::param_type;
		return d(global_urng(), param_t{ from, thru });
	}

	float random_uniform(float from, float upto)
	{
		static std::uniform_real_distribution<> d{};
		using param_t = decltype(d)::param_type;
		return d(global_urng(), param_t{ from, upto });
	}

	float random_normal(float mean, float stddev)
	{
		static std::normal_distribution<float> d(mean, stddev);
		using param_t = decltype(d)::param_type;
		return d(global_urng());
	}
}