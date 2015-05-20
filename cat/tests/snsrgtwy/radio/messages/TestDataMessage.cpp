#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstring>

#include <RF24Network/RF24Network.h>

#include <cat/snsrgtwy/radio/Data.hpp>
#include <cat/snsrgtwy/radio/messages/DataMessage.hpp>
#include <cat/snsrgtwy/radio/messages/CorruptMessageException.hpp>

#include <iostream>

namespace cat { namespace snsrgtwy { namespace radio { namespace messages {

	class TestDataMessage : public DataMessage {
		public:
			TestDataMessage(char message[]) : DataMessage (message) {}
			virtual size_t expected_total_data_items() const {
				return this->get_char_at(this->INDEX_OF_TOTAL_DATA_ITEMS);
			}
			virtual bool respond_to(RF24NetworkHeader header) {
				return true;
			}
			virtual bool get_response_message(
				char* &message,
				size_t &size
			) {
				return false;
			}
			Data get(size_t index) {
				return this->get_data_item_at(index);
			} 
	};
	
	BOOST_AUTO_TEST_SUITE(DataMessageTestSuite)

	BOOST_AUTO_TEST_CASE(should_interpret) {
		Data data;
		char long_long[(Data::MAX_SIZE*2)+1];
		char u_long_long[(Data::MAX_SIZE*2)+1];
		char message[] = {
			0x4E, 0x00, /* Size (78) */
			0x00, /* Status (OK)*/
			
			0x0B, /* 11 Items */
			
			Data::CHAR, 0xFF, /* -1 */
			Data::U_CHAR, '_',
			Data::SHORT, 0x01, 0x80, /* -32767 */
			Data::U_SHORT, 0x02, 0x00, /* 2 */
			Data::INT, 0x03, 0x00, 0x00, 0x80, /* -2147483645 */
			Data::U_INT, 0x04, 0x00, 0x00, 0x00, /* 4 */
			Data::LONG, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
			/* -9223372036854775803 */
			Data::U_LONG, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 6 */
			Data::FLOAT, 0x00, 0x00, 0x00, 0x00, /* 0.0 */
			Data::LONG_LONG,
				0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01,
				0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01,
			Data::U_LONG_LONG,
				0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
				0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE
		};
		
		TestDataMessage* m = new TestDataMessage(message);
		m->interpret();
		
		data = m->get(9);
		Data::to_hexadecimal_string(long_long, data.string, data.total_bytes(), true);
		data = m->get(10);
		Data::to_hexadecimal_string(u_long_long, data.string, data.total_bytes(), true);
			
		BOOST_CHECK(m->get(0).signed_char == -1);
		BOOST_CHECK(m->get(1).unsigned_char == '_');
		BOOST_CHECK(m->get(2).signed_short == -32767);
		BOOST_CHECK(m->get(3).unsigned_short == 2);
		BOOST_CHECK(m->get(4).signed_integer == -2147483645);
		BOOST_CHECK(m->get(5).unsigned_integer == 4);
		BOOST_CHECK(m->get(6).signed_long == (long) 0x8000000000000005);
		BOOST_CHECK(m->get(7).unsigned_long == 6);
		BOOST_CHECK(m->get(8).floating_point == 0);
		BOOST_CHECK(strcmp(long_long, "0123456789ABCDEF0123456789ABCDEF") == 0);
		BOOST_CHECK(strcmp(u_long_long, "FEDCBA9876543210FEDCBA9876543210") == 0);
	}

	BOOST_AUTO_TEST_CASE(should_catch_corruption) {
		;
	}

	BOOST_AUTO_TEST_SUITE_END()
	
}}}}
