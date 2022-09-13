#include "Random.h"
#include <random>


void Random::init(){
	std::random_device rd;
	Random::seed(rd());
}

void Random::seed(std::size_t seed){
	generator.seed(seed);
}

float Random::getFloat(){
	return getFloat(0.0, 1.0);
}

float Random::getFloat(float min, float max){
	std::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}

int Random::getInt(int min, int max){
	std::uniform_int_distribution<int> dist(min, max);
	return dist(generator);
}

Math::Vector<float> Random::getVector(const Math::Vector<float>& min, const Math::Vector<float>&  max){
	
	auto r = Math::Vector<float>(getFloat(), getFloat(), getFloat());
	return min + (max - min) * r;
}


std::mt19937 Random::generator;
