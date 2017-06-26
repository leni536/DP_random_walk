#ifndef LA_H
#define LA_H
/// @file

#include <armadillo>

namespace la
{
	/**
	 * \brief Rotates a vector around an other vecor.
	 * \param v0 The vector to be rotated.
	 * \param phi The angle vector. 
	 *        The length of this vector is the angle of the rotation in radians.
     *        The direction of the vector denotes the axis of the rotation.
     *        The rotation follows the right hand rule.
     * \returns The rotated vector.
	 */
	arma::vec Rotate(const arma::vec& v0,const arma::vec& phi);
}

#endif
