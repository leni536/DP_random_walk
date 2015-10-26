#include <armadillo>
#include <math.h>
#include "la.h"

namespace la
{
using namespace arma;

vec Rotate(const vec& v0,const vec& phi)
{
	double phi_scal=norm(phi,2);
	vec dir=phi/phi_scal;
	return cos(phi_scal)*v0 +
		sin(phi_scal)*cross(dir,v0) +
		(1-cos(phi_scal))*dot(dir,v0)*dir;

}


}
