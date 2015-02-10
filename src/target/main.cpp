#include <iostream>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/program_options.hpp>
#include "singlespin.h"
#include "random.h"

using namespace std;
namespace po=boost::program_options;



int main(int argc, char* argv[])
{
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help"							, "produce help message")
	    ("spins"	, po::value<int>()	-> default_value(10)	, "set number of spins")
	    ("duration"	, po::value<double>()	-> default_value(300)	, "set simulation duration")
	    ("timestep"	, po::value<double>()	-> default_value(1)	, "set timestep")
	    ("omega"	, po::value<double>()	-> default_value(0.2)	, "set the absolute value of Larmour precession")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	//Parsed option values
	int n_spins	= vm["spins"].as<int>();
	double duration	= vm["duration"].as<double>();
	double timestep	= vm["timestep"].as<double>();
	double omega	= vm["omega"].as<double>();

	int size=duration/timestep;

	boost::random::random_device rd;
	unsigned int seed = rd();
	randgen::gen::Instance(seed);

	SingleSpin * s;
	vector<double> sz(size,0.);

	s=new SingleSpin;
	for(int i=0;i<n_spins;i++)
	{
		s=new SingleSpin(omega);
		s->FillSzVec(sz,size,timestep);
		delete s;
	}

	for(int i=0;i<size;i++)
	{
		cout << i*1 << ", " << sz[i]/(double)n_spins << endl;
	}
}
