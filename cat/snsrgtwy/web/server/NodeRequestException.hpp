#ifndef NODEREQUESTEXCEPTION_HPP
#define NODEREQUESTEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace web { namespace server {

	class NodeRequestException : public std::runtime_error {
		public:
			NodeRequestException(const char* what) : std::runtime_error(what) {}
	};

}}}}

#endif
