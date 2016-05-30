#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>
#include <iostream>
#include <memory>
#include "autocorr.h"

class SingleSpin {
	public:
		enum model_t {naiv,burkov_2d,burkov_2d_Sx,rashba_3d,mixed_3d};
		enum meas_t  {prep,B_shot};
	protected:
		model_t model;
		meas_t meas;
		double tmin;

		arma::vec B_meas;
		double omega;
		double delta_omega;
		std::vector<double> times;
		std::vector<arma::vec> kvecs;
		std::vector<arma::vec> spins;
		int binary_search_t(const double &t);
	public:
		SingleSpin(const double& o=0.2,
			   const double& deltao=0.,
			   const model_t& m=naiv,
			   const meas_t& meas=prep,
			   double B_meas=0.,
			   double tmin=0.); 
		virtual void Step();
		void Print(std::ostream& out=std::cout);
		void RawPrint(std::ostream& out=std::cout);
		arma::vec GetSpin(const double &t);
		void FillSzVec(std::vector<double>& Sz, const int& size, const double& dt);
		double GetFirstTime() {return times.front();}
		double GetLastTime()  {return times.back(); }
};

class SingleSpinAutocorr : public SingleSpin {
	private:
		autocorr buf;
		const double dt;
	public:
		SingleSpinAutocorr(const double& o,
			   const double& deltao,
			   const model_t& m,
			   const meas_t& meas,
			   double B_meas,
			   double tmin,
			   double dt,
			   unsigned int N
			   ); 
		void Step();
		std::unique_ptr<std::vector<double>> GetAutocorr();
};

#endif
