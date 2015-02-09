#include <iostream>
#include <armadillo>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include "random.h"

using namespace std;

int main()
{
	boost::random::random_device urandom;
	randgen::pseudogen gen=randgen::gen::Instance(urandom())->getGen();
	boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
	cout << RandUnitVec(gen) << endl;
	return 0;
}
