#ifndef SINGLESPIN_H
#define SINGLESPIN_H

#include <armadillo>
#include <vector>
#include <iostream>
#include <memory>
#include "autocorr.h"

/**
 * \brief Spin relaxation experiment class mainly for ensamble measurements.
 *
 * It represents an experiment on a single spin. 
 * For an ensamble one has to create multiple classes an average over the results.
 */
class SingleSpin {
	public:
        /**
         * \brief An enum type for the underlying Hamiltonian and direction of interest.
         *
         * Each Hamiltonian has different built in SOC field, so they have different time evolutions.
         * The direction of interest is relevant for the FillSzVec method.
         */
		enum model_t {
		        naiv, /**< Fully isotropic 3D model. */
		        burkov_2d, /**< 2DEG model with Rashba SOC, z axis relaxation. */
		        burkov_2d_Sx, /**< 2DEG model with Rashba SOC, x axis relaxationa. */
			burkov_2d_angle, /**< 2DEG model with Rashba SOC, spins started polarized at an angle to the z axis, z component gathered. */
			burkov_2d_angle_sx, /**< 2DEG model with Rashba SOC, spins started polarized at an angle to the z axis, x component gathered. */
		        rashba_3d, /**< 3D model with Rashba SOC. */
		        mixed_3d, /**< 3D model with both isotropic and Rashba SOC. */
		        mn_1d,
		        dresselhaus, /**< 3D model with dresselhaus SOC, z axis relaxation */
			dresselhaus_xy, /**< 3D model with dresselhaus SOC, spins started with x direction polarization, y component gathered. */
			rashba_dressel_2d_z, /**< 2DEG model with both rashba and dresselhaus SOC, z axis relaxation. */
			rashba_dressel_2d_x, /**< 2DEG model with both rashba and dresselhaus SOC, x axis relaxation. */
			rashba_dressel_2d_xy, /**< 2DEG model with both rashba and dresselhaus SOC, spins started with x direction polarization, y component gathered. */
			rashba_dressel_3d_x,
			rashba_dressel_3d_z,
			rashba_dressel_3d_xz,
			rashba_dressel_3d_xy,
			rashba_dressel_3d_111_xx,
			rashba_dressel_3d_111_zz
		};
        /**
         * \brief Measurement type
         */
		enum meas_t {
            prep, /**< Measurement starting from fully polarized spin state */
            B_shot /**< Measurement starting from random spin state, external magnetic field is turned on at t=0 */
        };
	protected:
		model_t model;
		meas_t meas;
		double tmin;

		arma::vec B_meas;
		double omega;
		double delta_omega;
		std::vector<double> times;
		std::vector<arma::vec> kvecs;
		std::vector<arma::vec> spins;
		int binary_search_t(const double &t);
	public:

        /**
         * \brief Constructor.
         *
         * \param o The primary parameter for the SOC.
         *          It's typically the amplitude of the Larmor angular frequency due to the SOC field.
         *          The exact meaning depends on the model in question.
         * \param deltao The secondary paramter for the SOC.
         *               It typically describes the deviation of amplitude of the Larmor angular frequency.
         *               The exact meaning is model dependent.
         * \param m The model that selects the Hamiltonian, initial condition and the direction of interest.
         * \param meas The measurement type.
         * \param B_meas The amplitude of the external magnetic field for B_shot measurements.
         * \param tmin The starting time of the measurement.
         *        The magnetic field is turned on at t=0 for B_shot measurements.
         */
		SingleSpin(const double& o=0.2,
			   const double& deltao=0.,
			   const model_t& m=naiv,
			   const meas_t& meas=prep,
			   double B_meas=0.,
			   double tmin=0.); 

        /**
         * \brief Advances the simulation.
         *
         * The simulation is event driven.
         * Each step advances the simulation to the next scattering event.
         */
		virtual void Step();

        /**
         * \brief Prints the electron states at the scattering events.
         *
         */
		void Print(std::ostream& out=std::cout);

        /** \brief Prints the electron states at the scattering events without pretty formatting.
         */
		void RawPrint(std::ostream& out=std::cout);

        /** \brief Gets the electron spin state at an arbitrary time within the simulation range.
         *
         * \returns The electron spin state.
         * \throws std::runtime_error if t is not in range.
         */
		arma::vec GetSpin(const double &t);

        /**
         * \brief Fills a vector with a given spin component at uniform time samples.
         * 
         * \param Sz The vector to be filled.
         * \param size The number of points gathered.
         * \param dt The timestep between samples.
         */
		void FillSzVec(std::vector<double>& Sz, const int& size, const double& dt);

        /**
         * \brief Gets the starting time of the simulation.
         * \returns The starting time of the simulation.
         */
		double GetFirstTime() {return times.front();}

        /**
         * \brief Gets the time of the last scattering event.
         * \returns The time of the last scattering event.
         */
		double GetLastTime()  {return times.back(); }
};

/** 
 * \brief Spin relaxation experiment class for autocorrelation measurements.
 *
 * It measures the spin-spin autocorrelation for a single spin.
 * For a useful measurement it should be ran for a long time.
 */
class SingleSpinAutocorr : public SingleSpin {
	private:
		autocorr buf;
		const double dt;
	public:
        /** 
         * \copydoc SingleSpin::SingleSpin
         *
         * \param dt The timestep for the autocorrelation data.
         * \param N The number of samples for the autocorrelation data.
         */
		SingleSpinAutocorr(const double& o,
			   const double& deltao,
			   const model_t& m,
			   const meas_t& meas,
			   double B_meas,
			   double tmin,
			   double dt,
			   unsigned int N
			   ); 
		void Step();

        /**
         * \brief Gets the autocorrelation vector gathered during the simulation.
         *
         * \returns The autocorrelation vector.
         */
		std::unique_ptr<std::vector<double>> GetAutocorr();
};

#endif
