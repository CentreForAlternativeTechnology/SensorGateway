#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/SQLiteNodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>

namespace cat { namespace snsrgtwy { namespace radio {

	NodeDatabase* NodeDatabaseCreator::create() {
		return new SQLiteNodeDatabase ("./shared_keys.sqlite3");
	}
}}}

