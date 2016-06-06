#include <cmath>
#include <string>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include "singlespin.h"
#include "random.h"

//Stupid ass macros
#include "mathlink.h"

using namespace std::string_literals;

void test(int j) { 
	std::vector<double> v={1.,2.,3.,4.,(double)j};
	MLPutReal64List(stdlink,v.data(),v.size());
	return;
}

void simulation(
		double Omega,
		double DeltaOmega,
		char const* ModelString,
		char const* MeasurementString,
		double MagneticField,
		double Tmin,
		double Duration,
		double Timestep,
		int SpinNumber
	       )
{
	SingleSpin::model_t model;
	if (std::string(ModelString)=="naiv") model=SingleSpin::naiv;
	else if (std::string(ModelString)=="burkov_2d") model=SingleSpin::burkov_2d;
	else if (std::string(ModelString)=="burkov_2d_Sx") model=SingleSpin::burkov_2d_Sx;
	else if (std::string(ModelString)=="rashba_3d") model=SingleSpin::rashba_3d;
	else if (std::string(ModelString)=="mixed_3d") model=SingleSpin::mixed_3d;
	else 
	{
		MLPutString(stdlink,"Faszom");
		return ;
	}

	SingleSpin::meas_t meas;
	if (std::string(MeasurementString)=="prep") meas=SingleSpin::prep;
	else if (std::string(MeasurementString)=="B_shot") meas=SingleSpin::B_shot;
	else
	{
		MLPutString(stdlink,"Anyad");
		return ;
	}

	//Set dataset size
	int size=Duration/Timestep;

	SingleSpin * s;
	std::vector<double> sz(size,0.);

	for(int i=0;i<SpinNumber;i++)
	{
		s=new SingleSpin(Omega,DeltaOmega,model,meas,MagneticField,Tmin);
		s->FillSzVec(sz,size,Timestep);
		delete s;
	}

	std::vector<double> ret(2*size);

	for(int i=0;i<size;i++)
	{
		ret[i] = i*Timestep;
		ret[i+size] = sz[i]/(double)SpinNumber;
	}
	const int dims[]={2,size};
	MLPutReal64Array(stdlink,ret.data(),dims,(const char **)nullptr,2);
	return;
}

int main(int argc, char* argv[]) {
	//Seed random generator
	unsigned int seed_int;
	boost::random::random_device rd;
	seed_int = rd();
	randgen::gen::Instance(seed_int);

	return MLMain(argc, argv); 
}
