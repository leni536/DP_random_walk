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
            ("autocorr"                                                 , "set autocorr measurement")
	    ("spins"	, po::value<int>()	-> default_value(10)	, "set number of spins")
	    ("duration"	, po::value<double>()	-> default_value(300)	, "set simulation duration")
	    ("timestep"	, po::value<double>()	-> default_value(1)	, "set timestep")
	    ("omega"	, po::value<double>()	-> default_value(0.2)	, "set the absolute value of Larmor precession")
	    ("delta_omega", po::value<double>()	-> default_value(0.)	, "set the width of omega distribution")
	    ("seed"	, po::value<string>()	-> default_value("rand"), "set the seed for the random generator")
	    ("output,o"	, po::value<string>()	-> default_value("-")	, "output file path")
	    ("model,m"	, po::value<string>()	-> default_value("naiv"), "name of the model")
	    ("meas"	, po::value<string>()	-> default_value("prep"), "name of measurement method")
	    ("B_meas,b" , po::value<double>()	-> default_value(0.)	, "measurement field")
	    ("tmin"	, po::value<double>()	-> default_value(0.)	, "starting time, B_meas turns in at t=0")
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
        bool autocorr   = vm.count("autocorr");
	int n_spins	= vm["spins"].as<int>();
	double duration	= vm["duration"].as<double>();
	double timestep	= vm["timestep"].as<double>();
	double omega	= vm["omega"].as<double>();
	double delta_omega = vm["delta_omega"].as<double>();
	string seed	= vm["seed"].as<string>();
	string fname	= vm["output"].as<string>();
	string model_str= vm["model"].as<string>();
	string meas_str = vm["meas"].as<string>();
	double B_meas	= vm["B_meas"].as<double>();
	double tmin	= vm["tmin"].as<double>();

	//setting the model param
	SingleSpin::model_t model;
	if (model_str=="naiv") model=SingleSpin::naiv;
	else if (model_str=="burkov_2d") model=SingleSpin::burkov_2d;
	else if (model_str=="burkov_2d_Sx") model=SingleSpin::burkov_2d_Sx;
	else if (model_str=="rashba_3d") model=SingleSpin::rashba_3d;
	else if (model_str=="mixed_3d") model=SingleSpin::mixed_3d;
	else if (model_str=="mn_1d") model=SingleSpin::mn_1d;
	else if (model_str=="dresselhaus") model=SingleSpin::dresselhaus;
	else if (model_str=="dresselhaus_xy") model=SingleSpin::dresselhaus_xy;
	else if (model_str=="rashba_dressel_2d_z") model=SingleSpin::rashba_dressel_2d_z;
	else if (model_str=="rashba_dressel_2d_x") model=SingleSpin::rashba_dressel_2d_x;
	else if (model_str=="rashba_dressel_2d_xy") model=SingleSpin::rashba_dressel_2d_xy;
	else 
	{
		cerr << "Unknown model name: " << model_str << endl;
		return 2;
	}

	SingleSpin::meas_t meas;
	if (meas_str=="prep") meas=SingleSpin::prep;
	else if (meas_str=="B_shot") meas=SingleSpin::B_shot;
	else
	{
		cerr << "Unknown measurement name: " << meas_str << endl;
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
	unsigned int seed_int;
	if (seed == "rand")
	{
		boost::random::random_device rd;
		seed_int = rd();
	}
	else
	{
		seed_int = stoi(seed);
	}
	randgen::gen::Instance(seed_int);

	//Run simulation
        if (!autocorr)
        {
	        SingleSpin * s;
	        vector<double> sz(size,0.);

	        for(int i=0;i<n_spins;i++)
	        {
	        	s=new SingleSpin(omega,delta_omega,model,meas,B_meas,tmin);
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
	        *out << "#  seed: " << seed_int << endl;
	        *out << "#  model: " << model_str << endl;
	        *out << "#  meas: " << meas_str << endl;
	        *out << "#  B_meas: " << B_meas << endl;
	        *out << "#  tmin: " << tmin << endl;
		*out << "#  autocorr: false" << endl;
	        *out << "# t, Sz" << endl;
	        for(int i=0;i<size;i++)
	        {
	        	*out 	<< tmin+i*timestep
	        		<< ", " 
	        		//<< ((i==0) ? 1. :  sz[i]/(double)n_spins )
	        		<< sz[i]/(double)n_spins
	        		<< endl;
	        }
        }
	else
	{
		SingleSpinAutocorr s(omega,delta_omega,model,meas,B_meas,tmin,timestep,size);
		while(s.GetLastTime()<tmin+duration*n_spins)
			s.Step();
		auto vautocorr=s.GetAutocorr();
	        *out << "# Djakonov-Perel simulation" << endl;
	        *out << "# t=0 Sz=1, no magnetic field" << endl;
	        *out << "#  version: " << VERSION << endl;
	        *out << "#  spins: " << n_spins << endl;
	        *out << "#  duration: " << duration << endl;
	        *out << "#  timestep: " << timestep << endl;
	        *out << "#  omega: " << omega << endl;
	        *out << "#  seed: " << seed_int << endl;
	        *out << "#  model: " << model_str << endl;
	        *out << "#  meas: " << meas_str << endl;
	        *out << "#  B_meas: " << B_meas << endl;
	        *out << "#  tmin: " << tmin << endl;
		*out << "#  autocorr: true" << endl;
	        *out << "# t, Sz" << endl;

		for(int i=0;i<size;i++)
		{
			*out << i*timestep
			<< ", "
			<< (*vautocorr)[i]
			<< endl;
		}
	}
	if (file!=nullptr) 
            delete file;
	return 0;
}
