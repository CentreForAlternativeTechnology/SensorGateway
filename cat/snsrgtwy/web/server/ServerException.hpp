#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web { namespace server {

	class ServerException : public std::runtime_error {
		public:
			ServerException(const char* what) : std::runtime_error(what) {}
	};

}}}}

#endif
