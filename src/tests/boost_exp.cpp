#include <iostream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include "random.h"

using namespace std;

int main()
{
	boost::random::random_device rd;
	boost::random::exponential_distribution<> ExpDist(1.);
	double sum;
	for (int i=0;i<10000;i++)
	{
		randgen::pseudogen& gen=randgen::gen::Instance(rd())->getGen();
		double x=ExpDist(gen);
		if(i<20)
			cout << x << endl;
		sum+=x;
	}
	cout << endl;
	cout << sum/10000. << endl;
	return 0;
}
