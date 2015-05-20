#ifndef SQLITENODEDATABASEEXCEPTION_HPP
#define	SQLITENODEDATABASEEXCEPTION_HPP

#include <cat/snsrgtwy/radio/NodeDatabaseException.hpp>

namespace cat { namespace snsrgtwy { namespace radio {

	class SQLiteNodeDatabaseException : public NodeDatabaseException {
		public:
			SQLiteNodeDatabaseException(const char* what) : NodeDatabaseException(what) {}
	};

}}}

#endif
