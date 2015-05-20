#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/web/EmoncmsApiConfig.hpp>
#include <cat/snsrgtwy/web/http/RequestSslConfig.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE snsrgtwy

#include <boost/test/unit_test.hpp>

/* Global varibales for cryptography. */
RF24NetworkHeader HEADER;
char SHARED_KEY[] = {
	0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
	0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
};
char SHARED_KEY_STR[(cat::snsrgtwy::radio::Data::MAX_SIZE*2)+1];

BOOST_AUTO_TEST_SUITE(setup)
/* Setup database for cryptography. */
BOOST_AUTO_TEST_CASE(setup_node_database) {
	HEADER.from_node = 1;
	cat::snsrgtwy::radio::Data::to_hexadecimal_string(
		SHARED_KEY_STR,
		SHARED_KEY,
		sizeof(SHARED_KEY),
		false
	);
	cat::snsrgtwy::radio::NodeDatabase* db;
	db = cat::snsrgtwy::radio::NodeDatabaseCreator::create();
	db->set_shared_key(
		cat::snsrgtwy::radio::NodeDatabase::to_string(HEADER.from_node).c_str(),
		SHARED_KEY_STR
	);
	
	delete db;
}
/* Setup EmoncmsApiConfig to call local apache dummy server. */
BOOST_AUTO_TEST_CASE(setup_emoncms_api_config) {
	cat::snsrgtwy::web::EmoncmsApiConfig::set_destination("127.0.0.1:443/emoncms");
	cat::snsrgtwy::web::http::RequestSslConfig::set_certificate_filename("./127.0.0.1.cert");
}
BOOST_AUTO_TEST_SUITE_END()

#include "TestDiffieHellman.cpp"
//#include "radio/TestSQLitePublicKeyDatabase.cpp"
#include "radio/TestMessageFactory.cpp"
#include "radio/messages/TestLowPowerRadioCommsSpecMessage.cpp"
#include "radio/messages/TestDataMessage.cpp"
#include "radio/messages/TestKeyExchangeMessage.cpp"
#include "radio/messages/TestEncryptedMessage.cpp"
#include "radio/messages/TestAttrMessage.cpp"
#include "radio/messages/TestRegisterMessage.cpp"
#include "radio/messages/TestRegisterAttrMessage.cpp"
#include "radio/messages/TestPostMessage.cpp"
