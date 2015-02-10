#include <iostream>
#include "singlespin.h"
#include <vector>

using namespace std;

int main()
{
	SingleSpin s1;
	vector<double> sz(200,0);
	double dt=0.01;
	s1.FillSzVec(sz ,200, dt);
	cout << sz[150] << endl;
	return 0;
}
