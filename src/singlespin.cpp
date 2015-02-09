#include "singlespin.h"
#include <boost/random.hpp>
#include "random.h"
#include "la.h"
#include <armadillo>
#include <vector>

SingleSpin::SingleSpin()
{
// Initial conditions

	// t0=0
	this->times.push_back(0);

	// s_z=1
	arma::vec init;
	init << 0 << 0 << 1 ;
	this->spins.push_back(init);

	// random initial k vec (measured in k_F)
	randgen::pseudogen gen=randgen::gen::Instance()->getGen();
	boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
	this->kvecs.push_back(RandUnitVec(gen));
	this->omega=0.1;
}

void SingleSpin::Step()
{

}


