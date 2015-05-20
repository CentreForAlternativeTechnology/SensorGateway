#ifndef NODEDATABASEEXCEPTION_HPP
#define	NODEDATABASEEXCEPTION_HPP

#include <stdexcept>

namespace cat { namespace snsrgtwy { namespace radio {

	class NodeDatabaseException : public std::runtime_error {
		public:
			NodeDatabaseException(const char* what) : std::runtime_error(what) {}
	};

}}}

#endif
