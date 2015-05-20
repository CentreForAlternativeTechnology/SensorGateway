#include <cstddef>
#include <cstring>
#include <fstream>

#include <sqlite3.h>

#include <cat/snsrgtwy/DiffieHellman.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/SQLiteNodeDatabaseException.hpp>
#include <cat/snsrgtwy/radio/SQLiteNodeDatabase.hpp>

namespace cat { namespace snsrgtwy { namespace radio {
	
	const char* SQLiteNodeDatabase::SQL_CREATE_TABLE_SHARED_KEYS =
		"CREATE TABLE IF NOT EXISTS shared_keys (" \
		" address TEXT NOT NULL," \
		" shared_key TEXT NOT NULL," \
		" PRIMARY KEY (address)" \
		");"
	;
	const char* SQLiteNodeDatabase::SQL_CREATE_TABLE_EMONCMS_IDS =
		"CREATE TABLE IF NOT EXISTS emoncms_ids (" \
		" emoncms_id TEXT NOT NULL," \
		" address TEXT NOT NULL," \
		" PRIMARY KEY (emoncms_id),"
		" FOREIGN KEY (address) REFERENCES shared_keys(address)" \
		");"
	;
	const char* SQLiteNodeDatabase::SQL_SELECT_SHARED_KEYS =
		"SELECT shared_key" \
		" FROM shared_keys" \
		" WHERE address = ?;"
	;
	const char* SQLiteNodeDatabase::SQL_SELECT_EMONCMS_IDS =
		"SELECT address" \
		" FROM emoncms_ids" \
		" WHERE emoncms_id = ?"
	;
	const char* SQLiteNodeDatabase::SQL_INSERT_OR_UPDATE_SHARED_KEYS =
		"INSERT OR REPLACE INTO shared_keys (" \
		" address," \
		" shared_key" \
		") VALUES (?, ?);"
	;
	const char* SQLiteNodeDatabase::SQL_INSERT_OR_UPDATE_EMONCMS_IDS =
		"INSERT OR REPLACE INTO emoncms_ids (" \
		" address," \
		" emoncms_id" \
		") VALUES (?, ?);"
	;
		
	SQLiteNodeDatabase::SQLiteNodeDatabase(const char* filename) {
		this->filename = filename;
		
		sqlite3* db;
		std::ifstream file_exists(this->filename);
		if (!file_exists) {
			db = this->open();
			this->exec(db, SQL_CREATE_TABLE_SHARED_KEYS);
			this->exec(db, SQL_CREATE_TABLE_EMONCMS_IDS);
			this->close(db);
		}
	}
	
	char* SQLiteNodeDatabase::select(
		const char* statement,
		const char* primary_key
	) {
		char* value;
		
		sqlite3* db = this->open();
		sqlite3_stmt* select;
		int error_code = sqlite3_prepare_v2(
			db,
			statement,
			-1,
			&select,
			NULL
		);
		if (error_code != SQLITE_OK) {
			throw SQLiteNodeDatabaseException(
				sqlite3_errmsg(db)
			);
		}
		sqlite3_bind_text(select, 1, primary_key, -1, SQLITE_TRANSIENT);
		if (sqlite3_step(select) == SQLITE_ROW) {
			value = (char*) sqlite3_column_text(select, 0);
			value = this->allocate(value, strlen(value)+1);
		} else {
			value = NULL;
		}
		sqlite3_finalize(select);
		this->close(db);
		return value;
	}
	
	char* SQLiteNodeDatabase::get_shared_key(const char* address) {
		return this->select(SQL_SELECT_SHARED_KEYS, address);
	}
	
	char* SQLiteNodeDatabase::get_address(const char* emoncms_id) {
		return this->select(SQL_SELECT_EMONCMS_IDS, emoncms_id);
	}
	
	void SQLiteNodeDatabase::insert_or_update(
		const char* statement,
		const char* values[],
		size_t values_size
	) {
		sqlite3* db = this->open();
		sqlite3_stmt* insert_or_update;
		int error_code = sqlite3_prepare_v2(
			db,
			statement,
			-1,
			&insert_or_update,
			NULL
		);
		if (error_code != SQLITE_OK) {
			throw SQLiteNodeDatabaseException(
				sqlite3_errmsg(db)
			);
		}
		for (size_t i = 0; i < values_size; i++) {
			sqlite3_bind_text(insert_or_update, i+1, values[i], -1, SQLITE_TRANSIENT);
		}
		if (sqlite3_step(insert_or_update) != SQLITE_DONE) {
			throw SQLiteNodeDatabaseException(
				sqlite3_errmsg(db)
			);
		}
		sqlite3_finalize(insert_or_update);
		this->close(db);
	}
	
	void SQLiteNodeDatabase::set_shared_key(
		const char* address,
		const char* shared_key
	) {
		const size_t size = 2;
		const char* values[size] = {address, shared_key};
		this->insert_or_update(SQL_INSERT_OR_UPDATE_SHARED_KEYS, values, size);
	}
	
	void SQLiteNodeDatabase::set_emoncms_id(
		const char* address,
		const char* emoncms_id
	) {
		const size_t size = 2;
		const char* values[size] = {address, emoncms_id};
		this->insert_or_update(SQL_INSERT_OR_UPDATE_EMONCMS_IDS, values, size);
	}
	
	sqlite3* SQLiteNodeDatabase::open() {
		sqlite3* db;
		sqlite3_open(this->filename, &db);
		return db;
	}
	
	void SQLiteNodeDatabase::exec(sqlite3* db, const char* statement) {
		char* error_message;
		int error_code = sqlite3_exec(
			db,
			statement,
			NULL,
			NULL,
			&error_message
		);
		if (error_code != SQLITE_OK) {
			throw SQLiteNodeDatabaseException(error_message);
		}
	}
	
	char* SQLiteNodeDatabase::allocate(const char* source, size_t size) {
		char* destination = new char[size]();
		memcpy(destination, source, size);
		return destination;
	}
	
	void SQLiteNodeDatabase::close(sqlite3* db) {
		sqlite3_close(db);
	}
}}}

