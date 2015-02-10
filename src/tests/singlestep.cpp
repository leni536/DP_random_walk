#include <iostream>
#include <armadillo>
#include "singlespin.h"

using namespace std;
using namespace arma;

int main()
{
	SingleSpin s1;
	s1.Step();
	s1.Print();
	s1.RawPrint();
	vec s=s1.GetSpin(1.);
	s.print("s(t=1s):");
	return 0;
}
