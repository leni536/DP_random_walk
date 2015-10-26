#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>
#include <iostream>

class SingleSpin {
	public:
		enum model_t {naiv,burkov_2d,burkov_2d_Sx};
		enum meas_t  {prep,B_shot};
	private:
		model_t model;
		meas_t meas;
		double tmin;

		arma::vec B_meas;
		double omega;
		std::vector<double> times;
		std::vector<arma::vec> kvecs;
		std::vector<arma::vec> spins;
		int binary_search_t(const double &t);
	public:
		SingleSpin(const double& o=0.2,const model_t& m=naiv,const meas_t& meas=prep,double B_meas=0.,double tmin=0.); 
		void Step();
		void Print(std::ostream& out=std::cout);
		void RawPrint(std::ostream& out=std::cout);
		arma::vec GetSpin(const double &t);
		void FillSzVec(std::vector<double>& Sz, const int& size, const double& dt);
		double GetFirstTime() {return times.front();}
		double GetLastTime()  {return times.back(); }
};

#endif
