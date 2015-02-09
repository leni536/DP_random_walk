#include "random.h"
#include <boost/random.hpp>

namespace randgen{

gen* gen::pInstance = NULL;

gen* gen::Instance()
{
	if (!pInstance)
	{
		pInstance = new gen;
	}
	return pInstance;
}

gen* gen::Instance(uint32_t seed)
{
	if (!pInstance)
	{
		pInstance = new gen(seed);
	}
	return pInstance;
}

boost::random::mt19937& gen::getGen()
{
	return this->generator;
}


}
