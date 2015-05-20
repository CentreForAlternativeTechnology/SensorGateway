#ifndef REQUESTEXCEPTION_HPP
#define	REQUESTEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web { namespace http {

	class RequestException : public std::runtime_error {
		public:
			RequestException(
				const char what[]
			) : std::runtime_error(what) {
				
			}
	};

}}}}

#endif
