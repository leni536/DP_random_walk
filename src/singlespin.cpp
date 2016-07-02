#include "singlespin.h"
#include <boost/random.hpp>
#include "random.h"
#include "la.h"
#include <armadillo>
#include <vector>
#include <iostream>
#include <exception>

SingleSpin::SingleSpin(const double& o, const double& deltao, const model_t& m, const meas_t& meas,
		       double B_m, double tmin)
    : model(m), meas(meas), tmin(tmin), omega(o), delta_omega(deltao){
// Initial conditions
	randgen::pseudogen& gen=randgen::gen::Instance()->getGen();

	// t0=0
	times.push_back(tmin);

	// External magnetic field
	switch (model) {
		case naiv:
		case burkov_2d:
		case rashba_3d:
		case mixed_3d:
			B_meas << 0 << 0 << B_m;
			break;
		case burkov_2d_Sx:
			B_meas << B_m << 0 << 0;
			break;
	}

	// s_z=1
	arma::vec init;

	// Initial spin direction
	switch (meas) {
		case prep:
			switch (model) {
				case naiv:
				case burkov_2d:
				case rashba_3d:
				case mixed_3d:
					init << 0 << 0 << 1;
					break;
				case burkov_2d_Sx:
					init << 1 << 0 << 0;
					break;
			}
			break;
		case B_shot:
			boost::random::uniform_on_sphere<double, arma::vec>
			    RandUnitVec(3);
			init = RandUnitVec(gen);
			break;
	}
	spins.push_back(init);

	// initial k vector
	switch (model) {
		case naiv: {
			// 3D model, k vectors are from the unit sphere
			boost::random::uniform_on_sphere<double, arma::vec>
			    RandUnitVec(3);
			kvecs.push_back(RandUnitVec(gen));
			break;
		}
		case burkov_2d:
		case burkov_2d_Sx: {
			// 2D model, k vectors are from the unit circle, however
			// we treat them as 3D vectors.
			boost::random::uniform_on_sphere<double, arma::vec>
			    RandUnitVec(2);
			arma::vec temp = RandUnitVec(gen);
			temp.reshape(3, 1);
			kvecs.push_back(temp);
			break;
		}
		case rashba_3d: {
			boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
			arma::vec temp = RandUnitVec(gen);
			temp[2] = 0;
			kvecs.push_back(temp);
			break;
		}
		case mixed_3d: {
			boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
			arma::vec o1 = RandUnitVec(gen);
			arma::vec o2 = la::Rotate( o1, {0,0,atan(1)*2} );
			o2[2]=0.;
			kvecs.push_back(o1+(delta_omega/omega)*o2);
			break;
		}
	}
}

void SingleSpin::Step()
{
	randgen::pseudogen& gen=randgen::gen::Instance()->getGen();
	boost::random::exponential_distribution<> ExpDist(1.);

	//Scattering follows Poisson distribution
	double interval = ExpDist(gen);

	//Last state values
	arma::vec k = kvecs.back();
	arma::vec s = spins.back();
	double t    = times.back();

	//New values
	if (t<0)
	{
		if (t+interval<0)
			s= la::Rotate(s,k*omega*interval);
		else
		{
			s= la::Rotate(s,k*omega*(-t));
			s= la::Rotate(s,(k*omega+B_meas)*(interval+t));
		}
	}
	else
	{
		s  = la::Rotate(s,(k*omega+B_meas)*interval);
	}

	switch (model) {
		case naiv: {
			boost::random::uniform_on_sphere<double, arma::vec>
			    RandUnitVec(3);
			k = RandUnitVec(gen);
			break;
		}
		case burkov_2d:
		case burkov_2d_Sx: {
			boost::random::uniform_on_sphere<double, arma::vec>
			    RandUnitVec(2);
			k = RandUnitVec(gen);
			k.reshape(3, 1);
			break;
		}
		case rashba_3d: {
			boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
			k = RandUnitVec(gen);
			k[2] = 0;
			break;
		}
		case mixed_3d: {
			boost::random::uniform_on_sphere<double, arma::vec> RandUnitVec(3);
			arma::vec o1 = RandUnitVec(gen);
			arma::vec o2 = la::Rotate( o1, {0,0,atan(1)*2} );
			o2[2]=0.;
			k = o1 + (delta_omega/omega)*o2;
			break;
		}
	}

	t += interval;

	kvecs.push_back(k);
	spins.push_back(s);
	times.push_back(t);
}

void SingleSpin::Print(std::ostream& out)
{
	for(size_t i=0; i<spins.size(); i++)
	{
		out << "t= " << times[i] << " s" << std::endl;
		spins[i].print(out, "s:");
		kvecs[i].print(out, "k:");
		out << std::endl;
	}
}

void SingleSpin::RawPrint(std::ostream& out)
{
	for(size_t i=0; i<spins.size(); i++)
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

	//throw if t is not in range
	if (t < t1 || t > t2) {
		throw std::runtime_error("Time is not in range");
	}

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
	while (GetLastTime() < tmin+(size-1)*dt)
		Step();
	int ind=0;
	double next_t=times[ind+1];
	for(int i=0; i<size; i++)
	{
		while (tmin+i*dt>next_t)
		{
			ind++;
			next_t=times[ind+1];
		}
		double interval = tmin+i*dt - times[ind];
		arma::vec s=spins[ind];
		arma::vec k=kvecs[ind];
		if (times[ind]<0)
		{
			if (times[ind]+interval<0)
				s= la::Rotate(s,k*omega*interval);
			else
			{
				s= la::Rotate(s,k*omega*(-times[ind]));
				s= la::Rotate(s,(k*omega+B_meas)*(interval+times[ind]));
			}
		}
		else
		{
			s  = la::Rotate(s,(k*omega+B_meas)*interval);
		}
		switch (model) {
			case naiv:
			case burkov_2d:
			case rashba_3d:
			case mixed_3d:
				Sz[i] += s[2];
				break;
			case burkov_2d_Sx:
				Sz[i] += s[0];
				break;
		}
	}
}

//***************************
//* SingleSpinAutocorr

SingleSpinAutocorr::SingleSpinAutocorr(const double& o,
	   const double& deltao,
	   const model_t& m,
	   const meas_t& meas,
	   double B_meas,
	   double tmin,
	   double dt,
	   unsigned int N
	   ): SingleSpin(o,deltao,m,meas,B_meas,tmin), buf(N), dt(dt)
{
}

void SingleSpinAutocorr::Step()
{
	SingleSpin::Step();
	while(tmin+buf.get_eff_size()*dt < times.back() )
	{
		double t=times.end()[-2];
		double interval=tmin+buf.get_eff_size()*dt-t;
		arma::vec s=spins.end()[-2];
		arma::vec k=kvecs.end()[-2];
		if (t<0)
		{
			if (t+interval<0)
				s= la::Rotate(s,k*omega*interval);
			else
			{
				s= la::Rotate(s,k*omega*(-t));
				s= la::Rotate(s,(k*omega+B_meas)*(interval+t));
			}
		}
		else
		{
			s  = la::Rotate(s,(k*omega+B_meas)*interval);
		}
		switch (model) {
			case naiv:
			case burkov_2d:
			case rashba_3d:
			case mixed_3d:
			buf.push(s[2]);
			break;
			case burkov_2d_Sx:
			buf.push(s[0]);
			break;
		}
	}
	// !!! dirty hack !!!
	if (spins.size()>1000)
	{
		spins=std::vector<arma::vec>(spins.end()-2,spins.end());
		kvecs=std::vector<arma::vec>(kvecs.end()-2,kvecs.end());
		times=std::vector<double>(times.end()-2,times.end());
	}
}

std::unique_ptr<std::vector<double>> SingleSpinAutocorr::GetAutocorr()
{
	return std::move(buf.get_autocorr());
}
