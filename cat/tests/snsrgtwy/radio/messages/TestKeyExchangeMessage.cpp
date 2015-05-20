#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <cstring>
#include <RF24Network/RF24Network.h>
#include <cat/snsrgtwy/radio/messages/DataMessage.hpp>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/messages/KeyExchangeMessage.hpp>
#include <cat/snsrgtwy/radio/NodeDatabase.hpp>
#include <cat/snsrgtwy/radio/NodeDatabaseCreator.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	class TestKeyExchangeMessage : public KeyExchangeMessage {
		public:
			TestKeyExchangeMessage(char message[]) : KeyExchangeMessage(
				message
			) {
				
			}
			void save_key(RF24NetworkHeader header) {
				this->before_respond_to(header);
			}
			
			bool get_resp_msg(char* &message, size_t &size) {
				return KeyExchangeMessage::get_response_message(message, size);
			}
	};
	
	BOOST_AUTO_TEST_SUITE(KeyExchangeMessageTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_save_key) {
		const size_t divider = 5;
		RF24NetworkHeader header;
		char* key_resp_msg;
		size_t key_resp_msg_size;
		NodeDatabase* db = NodeDatabaseCreator::create();
		char key_msg[] = {
			0x11, 0x00, /* Size (17 bytes) */
			0x00, /* Status (OK) */
			0x01, /* 1 Item */
			Data::U_LONG_LONG,
				0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
				0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE
		};
		header.from_node = 2;
		
		TestKeyExchangeMessage* m = new TestKeyExchangeMessage(key_msg);
		m->save_key(header);
		//BOOST_CHECK(strcmp(db->get_shared_key("2"), "FEDCBA9876543210FEDCBA9876543210") == 0);
		BOOST_CHECK(strlen(db->get_shared_key("2")) == 32);
		m->get_resp_msg(key_resp_msg, key_resp_msg_size);
		BOOST_CHECK(key_resp_msg_size == sizeof(key_msg));
		key_msg[divider] = '\0';
		key_resp_msg[divider] = '\0';
		BOOST_CHECK(strcmp(key_resp_msg, key_msg) == 0);
		delete db;
		delete m;
	}
	
	BOOST_AUTO_TEST_CASE(should_catch_invalid_data_items) {
		
		char invalid_data_items[] = {
			0x00, 0x0A, /* Size (10 bytes) */
			0x00, /* Status (OK) */
			0x04, /* 4 items (5 expected) */
			
			/* Node ID */
			0x02, /* signed char (incorrect) */
			'F',

			/* Group ID */
			0x02, /* signed char (incorrect) */
			'A',
			/* Attribute ID */
			0x02, /* signed char (incorrect) */
			'I',
			
			/* Attribute Number */
			0x02, /* signed char (incorrect) */
			'L'
			
			/* Missing Attribute Value */
		};
		
		KeyExchangeMessage* m = new KeyExchangeMessage(invalid_data_items);
		BOOST_CHECK_THROW(m->interpret(), CorruptMessageException);	
		delete m;
	}

	BOOST_AUTO_TEST_SUITE_END()
}}}}
			
