#ifndef RNG_H
#define RNG_H
#include <random>

using namespace std;

class RNG {
private:

public:

	RNG()
	{

	}

	//NOTE: Potential source of slowdown
	int randomInt(int start, int end)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> distribution(start, end);
		return distribution(gen);
	}

};
#endif