#include"autocorr.h"
#include<vector>
#include<memory>
#include<iostream>

using namespace std;

int main()
{
	autocorr test(10);
	for(int i=0;i<100;i++)
	{
		test.push(i);
	}
	
	unique_ptr<vector<double> > vec_ptr=test.get_autocorr();
	for(int i=0;i<test.get_size();i++)
	{
		cout << (*vec_ptr)[i] << endl;
	}

	return 0;
}
