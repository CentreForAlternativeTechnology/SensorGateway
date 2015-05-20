#ifndef REQUESTSSLCONFIGEXCEPTION_HPP
#define	REQUESTSSLCONFIGEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	class RequestSslConfigException : public std::runtime_error {
		public:
			RequestSslConfigException() : std::runtime_error(
				"certificate filename not yet set"
			) {
				
			}
	};

}}}}

#endif
