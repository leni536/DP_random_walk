#include <iostream>
#include <fstream>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/program_options.hpp>
#include "singlespin.h"
#include "random.h"
#include "version.h"

using namespace std;
namespace po=boost::program_options;



int main(int argc, char* argv[])
{
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help,h"							, "produce help message")
	    ("version,v"						, "print version number")
	    ("spins"	, po::value<int>()	-> default_value(10)	, "set number of spins")
	    ("duration"	, po::value<double>()	-> default_value(300)	, "set simulation duration")
	    ("timestep"	, po::value<double>()	-> default_value(1)	, "set timestep")
	    ("omega"	, po::value<double>()	-> default_value(0.2)	, "set the absolute value of Larmor precession")
	    ("seed"	, po::value<string>()	-> default_value("rand"), "set the seed for the random generator")
	    ("output,o"	, po::value<string>()	-> default_value("-")	, "output file path")
	    ("model,m"	, po::value<string>()	-> default_value("naiv"), "name of the model")
	;
	po::positional_options_description pos_desc;
	pos_desc.add("output",-1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).
	          options(desc).positional(pos_desc).run(), vm);
	po::notify(vm);
    

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 0;
	}

	if (vm.count("version")) {
	    cout << VERSION << "\n";
	    return 0;
	}

	//Parsed option values
	int n_spins	= vm["spins"].as<int>();
	double duration	= vm["duration"].as<double>();
	double timestep	= vm["timestep"].as<double>();
	double omega	= vm["omega"].as<double>();
	string fname	= vm["output"].as<string>();
	string model_str= vm["model"].as<string>();

	//setting the model param
	SingleSpin::model_t model;
	if (model_str=="naiv") model=SingleSpin::naiv;
	else if (model_str=="burkov_2d") model=SingleSpin::burkov_2d;
	else 
	{
		cerr << "Unknown model name: " << model_str << endl;
		return 2;
	}

	//Setting output stream
	ostream *out=&cout;
	ofstream *file=NULL;
	if (fname != "-")
	{
		file=new ofstream(fname.c_str());
		if (!file->is_open())
		{
			cerr << "Can't open output file.";
			return 1;
		}
		out=file;
	}

	//Set dataset size
	int size=duration/timestep;

	//Seed random generator
	boost::random::random_device rd;
	unsigned int seed = rd();
	randgen::gen::Instance(seed);

	//Run simulation
	SingleSpin * s;
	vector<double> sz(size,0.);

	s=new SingleSpin;
	for(int i=0;i<n_spins;i++)
	{
		s=new SingleSpin(omega,model);
		s->FillSzVec(sz,size,timestep);
		delete s;
	}

	//Output
	*out << "# Djakonov-Perel simulation" << endl;
	*out << "# t=0 Sz=1, no magnetic field" << endl;
	*out << "#  version: " << VERSION << endl;
	*out << "#  spins: " << n_spins << endl;
	*out << "#  duration: " << duration << endl;
	*out << "#  timestep: " << timestep << endl;
	*out << "#  omega: " << omega << endl;
	*out << "#  seed: " << seed << endl;
	*out << "#  model: " << model_str << endl;
	*out << "# t, Sz" << endl;
	for(int i=0;i<size;i++)
	{
		*out 	<< i*timestep
			<< ", " 
			<< ((i==0) ? 1. :  sz[i]/(double)n_spins )
			<< endl;
	}
	delete file;
	return 0;
}
