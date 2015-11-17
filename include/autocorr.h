#ifndef AUTOCORR_H
#define AUTOCORR_H

#include<vector>
#include<memory>
#include "buffer.h"

class autocorr : public buffer<double>{
	private:
	std::vector<double> autocorrsum;
	std::vector<int> autocorrcount;
	public:
	autocorr(size_t);
	void push(const double&);
	std::unique_ptr<std::vector<double> > get_autocorr();
};
#endif
