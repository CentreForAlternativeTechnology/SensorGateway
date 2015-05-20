#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <cstring>
#include <cat/snsrgtwy/radio/SQLiteNodeDatabase.hpp>

namespace cat { namespace snsrgtwy { namespace radio {

	BOOST_AUTO_TEST_SUITE(SQLiteNodeDatabaseTestSuite)
	
	const char* dbname = "./test.sqlite3";
	
	BOOST_AUTO_TEST_CASE(should_set_and_get_and_reset_and_reset_keys) {
		SQLiteNodeDatabase db(dbname);
		const char* actual_key;
		const char* address1 = "addr1";
		const char* address2 = "addr2";
		const char* public_key1 = "public_key1";
		const char* public_key2 = "public_key2";
		const char* private_key1 = "private_key1";
		const char* private_key2 = "private_key2";
		
		db.set_all_keys_for(address1, public_key1, private_key1);
		actual_key = db.get_public_key_of(address1);
		BOOST_CHECK(std::strcmp(actual_key, public_key1) == 0);
		delete[] actual_key;
		actual_key = db.get_private_key_for(address1);
		BOOST_CHECK(std::strcmp(actual_key, private_key1) == 0);
		delete[] actual_key;

		db.set_all_keys_for(address2, public_key2, private_key2);
		actual_key = db.get_public_key_of(address2);
		BOOST_CHECK(std::strcmp(actual_key, public_key2) == 0);
		delete[] actual_key;
		actual_key = db.get_private_key_for(address2);
		BOOST_CHECK(std::strcmp(actual_key, private_key2) == 0);
		delete[] actual_key;

		db.set_all_keys_for(address1, public_key2, private_key2);
		actual_key = db.get_public_key_of(address2);
		BOOST_CHECK(std::strcmp(actual_key, public_key2) == 0);
		delete[] actual_key;
		actual_key = db.get_private_key_for(address2);
		BOOST_CHECK(std::strcmp(actual_key, private_key2) == 0);
		delete[] actual_key;
	}
	
	BOOST_AUTO_TEST_CASE(should_return_null_for_nonexistent_address) {
		SQLiteNodeDatabase db(dbname);
		const char* nonexistent_address = "__NONEXISTENT__";
		BOOST_CHECK(db.get_public_key_of(nonexistent_address) == NULL);
	}

	BOOST_AUTO_TEST_SUITE_END()

}}}
