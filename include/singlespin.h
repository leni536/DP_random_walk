#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>

class SingleSpin {
	private:
	public:
		std::vector<double> times;
		std::vector<arma::vec> spins;
};

#endif
