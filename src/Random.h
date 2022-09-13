#ifndef RANDOM_H
#define RANDOM_H

#include "../Math.h"
#include <random>

class Random{
	public:
		static void init();

		static void seed(std::size_t seed);

		static float getFloat();

		static float getFloat(float min, float max);

		static int getInt(int min, int max);

		static Math::Vector<float> getVector(const Math::Vector<float>& min, const Math::Vector<float>&  max);

	private:

		static std::mt19937 generator;
};

#endif

