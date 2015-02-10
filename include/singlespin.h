#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>
#include <iostream>

class SingleSpin {
	private:
		double omega;
		std::vector<double> times;
		std::vector<arma::vec> kvecs;
		std::vector<arma::vec> spins;
		int binary_search_t(const double &t);
	public:
		SingleSpin(const double& o=0.2); 
		void Step();
		void Print(std::ostream& out=std::cout);
		void RawPrint(std::ostream& out=std::cout);
		arma::vec GetSpin(const double &t);
		void FillSzVec(std::vector<double>& Sz, const int& size, const double& dt);
		double GetFirstTime() {return times.front();}
		double GetLastTime()  {return times.back(); }
};

#endif
