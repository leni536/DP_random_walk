#include"autocorr.h"
#include"buffer.h"
#include<vector>
#include<memory>
#include<cmath>
#include<algorithm>

autocorr::autocorr(size_t size):
	buffer<double>(size), 
	autocorrsum(size,0.), 
	autocorrcount(size,0)
{};

void autocorr::push(const double& item)
{
	buffer<double>::push(item);
	for(int i=0; i<std::min(this->get_size(),this->get_eff_size()); i++)
	{
		autocorrsum[i]+=(*this)[this->get_eff_size()-1]*(*this)[this->get_eff_size()-1-i];
		autocorrcount[i]++;
	}
}

std::unique_ptr<std::vector<double> > autocorr::get_autocorr()
{
	std::unique_ptr<std::vector<double> > vec_ptr(new std::vector<double>(this->get_size()));
	for(int i=0; i<this->get_size(); i++)
	{
		if (autocorrcount[i]>0)
			(*vec_ptr)[i]=autocorrsum[i]/autocorrcount[i];
		else
			(*vec_ptr)[i]=NAN;
	}
	return std::move(vec_ptr);
}

