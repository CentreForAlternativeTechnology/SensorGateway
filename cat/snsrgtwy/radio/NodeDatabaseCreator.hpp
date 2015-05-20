#ifndef NODEDATABASECREATOR_HPP
#define NODEDATABASECREATOR_HPP

#include <cstddef>
#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/SQLiteNodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	/**
	 * Instantiates a NodeDatabase that interfaces with a database to use
	 * universally within the system.
	 */
	class NodeDatabaseCreator {
		public:
			static const size_t SIZE_OF_ADDRESS = SQLiteNodeDatabase::SIZE_OF_ADDRESS;
			static const size_t SIZE_OF_KEY = SQLiteNodeDatabase::SIZE_OF_KEY;
			/**
			 * Creates an instance of a NodeDatabase on the heap and returns 
			 * its pointer.
			 * @return a pointer to a new NodeDatabase
			 */
			static NodeDatabase* create();
	};

}}}

#endif

