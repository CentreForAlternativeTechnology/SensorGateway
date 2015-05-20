#ifndef EMONCMSAPICONFIGEXCEPTION_HPP
#define	EMONCMSAPICONFIGEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web {

	class EmoncmsApiConfigException : public std::runtime_error {
		public:
			EmoncmsApiConfigException() : std::runtime_error(
				"destination not yet set"
			) {
				
			}
	};

}}}

#endif
