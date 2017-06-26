#ifndef AUTOCORR_H
#define AUTOCORR_H

#include<vector>
#include<memory>
#include "buffer.h"

/** \brief Class for gathering autocorrelation of time series data.
 *
 * For a time series a_0, a_1, a_2 ... it collects the mean values
 * <a_i*a_i>, <a_(i+1)*a_i>, <a_(i+2)*a_i> ...
 */
class autocorr : public buffer<double>{
	private:
	std::vector<double> autocorrsum;
	std::vector<int> autocorrcount;
	public:
	/**
	 * \copydoc buffer::buffer
	 */
	autocorr(size_t size);
	void push(const double& value);
	/**
	 * \brief Gets the autocorrelation vector.
	 * \returns An std::unique_ptr<std::vector<double>> containing the autocorrelation vector.
	 *
	 * The elements of the vector are the index averaged mean values of 
	 * <a_i*a_i>, <a_(i+1)*a_i>, <a_(i+2)*a_i> ...
	 * Where a_i are the pushed inside the buffer.
	 */
	std::unique_ptr<std::vector<double> > get_autocorr();
};
#endif
