#include <cmath>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include "singlespin.h"
#include "random.h"

//Stupid ass macros
#include "mathlink.h"

using namespace std::string_literals;

struct Progress {
	std::string Mathematica_variable;
	double progress;
	void MLSend(MLINK lp);
};

void Progress::MLSend(MLINK lp) {
	if (Mathematica_variable != "") {
		std::ostringstream sstream;
		sstream << progress;
		std::string expr_str =
		    (Mathematica_variable + "=" + sstream.str());
		char* expr = new char[expr_str.size() + 1];
		strcpy(expr, expr_str.c_str());
		MLEvaluateString(lp, expr);
		delete[] expr;
	}
}

Progress prog={"",0.};

void test(int j) { 
	std::vector<double> v={1.,2.,3.,4.,(double)j};
	MLPutReal64List(stdlink,v.data(),v.size());
	return;
}

void setProgress() {
	const char * s;
	if ( !MLGetSymbol(stdlink,&s) ) {
		MLPutSymbol(stdlink,"$Failed");
		return;
	}
	prog.Mathematica_variable=s;
	MLReleaseSymbol(stdlink,s);
	MLPutSymbol(stdlink,"Null");
	return;
}

void simulation(
		double Omega,
		double DeltaOmega,
		char const* ModelString,
		char const* MeasurementString,
		char const* AutocorrString,
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
	else if (std::string(ModelString)=="mn_1d") model=SingleSpin::mn_1d;
	else if (std::string(ModelString)=="dresselhaus") model=SingleSpin::dresselhaus;
	else 
	{
		MLPutSymbol(stdlink,"$Failed");
		return ;
	}

	SingleSpin::meas_t meas;
	if (std::string(MeasurementString)=="prep") meas=SingleSpin::prep;
	else if (std::string(MeasurementString)=="B_shot") meas=SingleSpin::B_shot;
	else
	{
		MLPutSymbol(stdlink,"$Failed");
		return ;
	}

	bool autocorr=false;
	if (std::string(AutocorrString)=="autocorr") autocorr=true;

	//Set dataset size
	int size=Duration/Timestep;

	SingleSpin * s;
	std::vector<double> sz(size,0.);

        if (!autocorr)
        {
		for(int i=0;i<SpinNumber;i++)
		{
			s=new SingleSpin(Omega,DeltaOmega,model,meas,MagneticField,Tmin);
			s->FillSzVec(sz,size,Timestep);
			delete s;
			prog.progress=(double)(i+1)/SpinNumber;
			prog.MLSend(stdlink);
			if (MLAbort) {
				MLPutSymbol(stdlink,"$Aborted");
				return;
			}
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
	} else {
		SingleSpinAutocorr s(Omega, DeltaOmega, model, meas, meas, Tmin,
				     Timestep, size);
		int counter=0;
		while (s.GetLastTime() < Tmin + Duration * SpinNumber) {
			s.Step();
			if (counter % size == 0) {
				prog.progress = (s.GetLastTime() - Tmin) /
						(Duration * SpinNumber);
				prog.MLSend(stdlink);
			}
			if (MLAbort) {
				MLPutSymbol(stdlink,"$Aborted");
				return;
			}
			counter++;
		}
		auto vautocorr = s.GetAutocorr();

		std::vector<double> ret(2 * size);
		for (int i = 0; i < size; i++) {
			ret[i] = i * Timestep;
			ret[i + size] = (*vautocorr)[i];
		}
		const int dims[] = {2, size};
		MLPutReal64Array(stdlink, ret.data(), dims,
				 (const char**)nullptr, 2);

		return;
	}
}

int main(int argc, char* argv[]) {
	//Seed random generator
	unsigned int seed_int;
	boost::random::random_device rd;
	seed_int = rd();
	randgen::gen::Instance(seed_int);

	return MLMain(argc, argv); 
}
