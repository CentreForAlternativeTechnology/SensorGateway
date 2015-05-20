#ifndef EMONCMSAPIEXCEPTION_HPP
#define	EMONCMSAPIEXCEPTION_HPP

#include <stdexcept>
#include <iostream>

namespace cat { namespace snsrgtwy { namespace web {

	class EmoncmsApiException : public std::runtime_error {
		public:
			EmoncmsApiException(const char body[]) : std::runtime_error(
				body
			) {
				
			}
	};

}}}

#endif
