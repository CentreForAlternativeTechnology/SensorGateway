#include <boost/test/unit_test.hpp>
#include <cstring>
#include <string>
#include <gmp.h>
#include <cat/snsrgtwy/DiffieHellman.hpp>

#include <iostream>
namespace cat { namespace snsrgtwy {

	BOOST_AUTO_TEST_SUITE(DiffieHellmanTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_calculate_shared_session_key) {
		
		DiffieHellman dh;
		
		char generator[2] = {};
		char modulus[33] = {};
		char alice_secret[5] = {};
		char becky_secret[5] = {};
		
		strcpy(generator, "6");
		strcpy(modulus, "4341544D616A6F7250726F6A65637447");
		strcpy(alice_secret, "ABCD");
		strcpy(becky_secret, "BCDE");
		
		char* alice_public = dh.calculate(
			generator,
			alice_secret,
			modulus
		);
		char* becky_public = dh.calculate(
			generator,
			becky_secret,
			modulus
		);
		char* alice_shared = dh.calculate(
			becky_public,
			alice_secret,
			modulus
		);
		char* becky_shared = dh.calculate(
			alice_public,
			becky_secret,
			modulus
		);
		
		BOOST_CHECK(std::strcmp(alice_public, becky_public) != 0);
		BOOST_CHECK(std::strcmp(alice_shared, becky_shared) == 0);
		
		delete[] alice_public;
		delete[] becky_public;
		delete[] alice_shared;
		delete[] becky_shared;
	}
	
	BOOST_AUTO_TEST_CASE(should_generate_random) {
		DiffieHellman dh;
		char* random_1 = dh.generate_random();
		char* random_2 = dh.generate_random();
		BOOST_CHECK(std::strcmp(random_1, random_2) != 0);
	}
	
	BOOST_AUTO_TEST_SUITE_END()

}}
