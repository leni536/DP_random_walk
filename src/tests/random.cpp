#include <iostream>
#include <boost/random.hpp>
#include "random.h"

using namespace std;

int main()
{
	boost::random::mt19937 randgen = randgen::gen::Instance()->getGen();
	boost::random::uniform_real_distribution<> U(0,1);
	for (int i=0; i< 10; i++)
	{
		cout << U(randgen) << endl;
	}
	return 0;
}
