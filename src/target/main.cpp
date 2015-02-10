#include <iostream>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include "singlespin.h"
#include "random.h"

using namespace std;

int main()
{
	boost::random::random_device rd;
	unsigned int seed = rd();
	randgen::gen::Instance(seed);

	SingleSpin * s;
	vector<double> sz(200,0.);

	s=new SingleSpin;
	for(int i=0;i<1000;i++)
	{
		s=new SingleSpin;
		s->FillSzVec(sz,200,1);
		delete s;
	}

	for(int i=0;i<200;i++)
	{
		cout << i*1 << ", " << sz[i] << endl;
	}
}
