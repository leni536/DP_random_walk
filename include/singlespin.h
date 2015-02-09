#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>

class SingleSpin {
	private:
		double omega;
		std::vector<double> times;
		std::vector<arma::vec> kvecs;
		std::vector<arma::vec> spins;
	public:
		SingleSpin();
		void Step();
};

#endif
