#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <stdlib.h>
namespace zyk
{
	class RandomGenerator
	{
	public:
		RandomGenerator(int seed=1):inv_rand_max(1/(float)RAND_MAX)
		{
			srand(seed);
		}
		int getRand(int num)//get a integer random in [0,num)
		{
			return rand()%num;
		}
		float getRand()//get a float random in [0,1]
		{
			return rand()*inv_rand_max;
		}
	private:
		const float inv_rand_max;
	};
}
#endif