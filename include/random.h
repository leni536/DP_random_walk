#include <boost/random.hpp>

namespace randgen{
	typedef boost::random::mt19937 pseudogen;
	class gen
	{
		public:
			static gen* Instance();
			static gen* Instance(uint32_t seed);
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
