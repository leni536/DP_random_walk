#include <boost/random.hpp>

namespace randgen{
	class gen
	{
		public:
			static gen* Instance();
			static gen* Instance(uint32_t seed);
			boost::random::mt19937& getGen();
		private:
			boost::random::mt19937 generator;
			gen() {};
			gen(uint32_t seed): generator(seed) {};
			gen(gen const& copy);
			gen& operator=(gen const& copy);

			static gen* pInstance;
	};
}
