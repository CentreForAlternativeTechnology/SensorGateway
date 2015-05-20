#ifndef MESSAGEEXCEPTION_HPP
#define	MESSAGEEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	class MessageException : public std::out_of_range {
		public:
			MessageException(const char* what) : std::out_of_range(what) {}
	};

}}}}

#endif
