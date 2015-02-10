#include "singlespin.h"
#include <boost/random.hpp>
#include "random.h"
#include "la.h"
#include <armadillo>
#include <vector>
#include <iostream>

SingleSpin::SingleSpin(const double& o) : omega(o)
{
// Initial conditions

	// t0=0
	times.push_back(0);

	// s_z=1
	arma::vec init;
	init << 0 << 0 << 1 ;
	spins.push_back(init);

	// random initial k vec (measured in k_F)
	randgen::pseudogen& gen=randgen::gen::Instance()->getGen();
	boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
	kvecs.push_back(RandUnitVec(gen));
}

void SingleSpin::Step()
{
	randgen::pseudogen& gen=randgen::gen::Instance()->getGen();
	boost::random::exponential_distribution<> ExpDist(1.);
	boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);

	//Scattering follows Poisson distribution
	double interval = ExpDist(gen);

	//Last state values
	arma::vec k = kvecs.back();
	arma::vec s = spins.back();
	double t    = times.back();

	//New values
	s  = la::Rotate(s,k*omega*interval);
	k  = RandUnitVec(gen);
	t += interval;

	kvecs.push_back(k);
	spins.push_back(s);
	times.push_back(t);
}

void SingleSpin::Print(std::ostream& out)
{
	for(int i=0; i<spins.size(); i++)
	{
		out << "t= " << times[i] << " s" << std::endl;
		spins[i].print(out, "s:");
		kvecs[i].print(out, "k:");
		out << std::endl;
	}
}

void SingleSpin::RawPrint(std::ostream& out)
{
	for(int i=0; i<spins.size(); i++)
	{
		out << times[i] << ", ";
		for(int j=0;j<3;j++)
			out << spins[i][j] << ", ";
		for(int j=0;j<3;j++)
			out << kvecs[i][j] << ((j<2) ? ", " : "");
		out << std::endl;
	}
}

arma::vec SingleSpin::GetSpin(const double &t)
{
	double i=binary_search_t(t);
	double interval=t-times[i];
	arma::vec s=spins[i];
	arma::vec k=kvecs[i];
	s=la::Rotate(s,k*omega*interval);
	return s;
}

int SingleSpin::binary_search_t(const double &t)
{
	//time interval of simulation:
	double t1=times.front();
	double t2=times.back();

	//halt if t is not in range
	assert(t>t1 && t<t2);

	double size=times.size();
	double rmin=0;
	double rmax=size-1;

	while (rmin != rmax)
	{
		double i=rmin+(rmax-rmin)/2;
		if (times[i] <= t)
			if (rmin==i)
			{
				if (times[i+1]<=t)
					rmin=i+1;
				else
					rmax=rmin;
			}
			else
				rmin=i;
		else
			rmax=i-1;
	}
	return rmin;
}


void SingleSpin::FillSzVec(std::vector<double>& Sz, const int& size, const double& dt)
{
	while (GetLastTime() < (size-1)*dt)
		Step();
	int ind=0;
	double next_t=times[ind+1];
	for(int i=0; i<size; i++)
	{
		while (i*dt>next_t)
		{
			ind++;
			next_t=times[ind+1];
		}
		double interval = i*dt - times[ind];
		arma::vec s=spins[ind];
		arma::vec k=kvecs[ind];
		s = la::Rotate(s,k*omega*interval);
		Sz[i] += s[2];
	}
}
