#include <boost/random.hpp>

namespace randgen{
	typedef boost::random::mt19937 pseudogen;

	/**
	 * \brief Random generator singleton
	 *
	 * The random generator singleton used throughout the codebase.
	 * It prevents the creation of multiple independent generators,
	 *   which could potentially hurt the randomness of the simulation.
	 */
	class gen
	{
		public:
			/**
			 * \brief Gets the unique instance of the random generator.
			 * \returns Pointer to the instance.
			 *
			 * It constructs the generator with a random seed (using the OS's entropy pool) if it wasn't already constructed.
			 */
			static gen* Instance();

			/**
			 * \brief Gets the unique instance of the random generator.
			 * \param seed The seed passed to the constructor of the random generator.
			 * \returns Pointer to the instance.
			 *
			 * The seed parameter is omitted if the instance already exists.
			 */
			static gen* Instance(uint32_t seed);

			/**
			 * \brief Gets the underlying random generator.
			 * \returns The underlying random generator.
			 *
			 * The returntype is pseudogen typedefd to boost::random::mt19937.
			 */
			pseudogen& getGen();
		private:
			pseudogen generator;
			gen() {};
			gen(uint32_t seed): generator(seed) {};
			gen(gen const& copy);
			gen& operator=(gen const& copy);

			static gen* pInstance;
	};
}
