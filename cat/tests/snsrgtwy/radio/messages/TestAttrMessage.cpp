#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cat/snsrgtwy/radio/messages/EncryptedMessage.hpp>
#include <cat/snsrgtwy/radio/messages/AttrMessage.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {
	
	class TestAttrMessage : public AttrMessage {
		public:
			TestAttrMessage(char message[]) : AttrMessage(message) {}
			Data get_last() {
				return this->get_last_data_item();
			}
			void decrypt_and_interpret() {
				this->before_respond_to(HEADER);
			}
	};
	
	BOOST_AUTO_TEST_SUITE(AttrMessageTestSuite)
	
	BOOST_AUTO_TEST_CASE(should_match_raw_message) {
		
		char attr_msg[] = {
			0x0E, 0x00, /* Size (16 bytes) */
			0x00, /* Status (OK) */
			0x05, /* 5 Items */

			/* Node ID */
			0x05, /* unsigned short */
			0xFF, 0xFF, /* 65535 */

			/* Group ID */
			0x05, /* unsigned short */
			0x01, 0x00, /* 1 */

			/* Attribute ID */
			0x05, /* unsigned short */
			0x0F, 0x0F, /* 3855 */

			/* Attribute Number */
			0x05, /* unsigned short */
			0xF0, 0xF0, /* 61680 */

			/* Attribute Value */
			0x02, /* char */
			0x55 /* 01010101 or 'U' in utf8 */
		};
		size_t size = sizeof(attr_msg);
		char* attr_msg_encrypted = new char[size]();
		memcpy(attr_msg_encrypted, attr_msg, size);
		EncryptedMessage::convert_to_encrypted_message(
			attr_msg_encrypted,
			size,
			SHARED_KEY
		);
		TestAttrMessage* m = new TestAttrMessage(attr_msg_encrypted);
		m->decrypt_and_interpret();
		BOOST_CHECK(m->get_node_id() == 0xFFFF);
		BOOST_CHECK(m->get_group_id() == 0x0001);
		BOOST_CHECK(m->get_attr_id() == 0x0F0F);
		BOOST_CHECK(m->get_attr_number() == 0xF0F0);
		BOOST_CHECK(m->get_last().signed_char == 0x55);
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
		
		TestAttrMessage* m = new TestAttrMessage(invalid_data_items);
		BOOST_CHECK_THROW(m->interpret(), CorruptMessageException);	
		BOOST_CHECK_THROW(m->get_node_id(), CorruptMessageException);
		BOOST_CHECK_THROW(m->get_group_id(), CorruptMessageException);
		BOOST_CHECK_THROW(m->get_attr_id(), CorruptMessageException);
		BOOST_CHECK_THROW(m->get_attr_number(), CorruptMessageException);
		BOOST_CHECK_THROW(m->get_last(), CorruptMessageException);
		delete m;
	}

	BOOST_AUTO_TEST_SUITE_END()
}}}}
			
