#ifndef SQLITENODEDATABASE_HPP
#define SQLITENODEDATABASE_HPP

#include <cstddef>
#include <sqlite3.h>
#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>

namespace cat { namespace snsrgtwy { namespace radio {

	class SQLiteNodeDatabase : public NodeDatabase {
		public:
			static const size_t SIZE_OF_ADDRESS = 6;
			static const size_t SIZE_OF_KEY = DiffieHellman::SIZE;
			SQLiteNodeDatabase(const char* filename);
			virtual char* get_shared_key(const char* address);
			virtual char* get_address(const char* emoncms_id);
			virtual void set_shared_key(const char* address, const char* shared_key);
			virtual void set_emoncms_id(const char* address, const char* emoncms_id);
		protected:
			static const char* SQL_CREATE_TABLE_SHARED_KEYS;
			static const char* SQL_CREATE_TABLE_EMONCMS_IDS;
			static const char* SQL_SELECT_SHARED_KEYS;
			static const char* SQL_SELECT_EMONCMS_IDS;
			static const char* SQL_INSERT_OR_UPDATE_SHARED_KEYS;
			static const char* SQL_INSERT_OR_UPDATE_EMONCMS_IDS;
			
			/**
			 * Copies a given char array onto the heap.
			 * @param source the first character in the char array
			 * @param the length of the char array
			 * @return a pointer to the newly allocated char array
			 */
			static char* allocate(const char* source, size_t size);
			
			/**
			 * The file being used as the SQLite database.
			 */
			const char* filename;
			
			/**
			 * Opens an sqlite3 connection.
			 * @return an  sqlite3 connection.
			 */
			sqlite3* open();
			/**
			 * Executes a given statement on a given database connection
			 * @param db the database connection
			 * @param statement the statement to be executed
			 */
			void exec(sqlite3* db, const char* statement);
			/**
			 * Executes a select statement formatted to query a single column
			 * for a given primary key.
			 * @param statement the statement to execute
			 * @param primary_key the primary key to query by
			 * @return the value of the queried column
			 */
			char* select(const char* statement, const char* primary_key);
			/**
			 * Executes a insert or replace statement with given bounded values.
			 * @param statement the statement to execute
			 * @param values the values to be bounded
			 * @param size the number of values to be bounded
			 */
			void insert_or_update(
				const char* statement,
				const char* values[],
				size_t size
			);
			/**
			 * Closes the given sqlite3 connection.
			 */
			void close(sqlite3* db);
	};

}}}

#endif

